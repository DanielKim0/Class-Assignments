#include "macros.h"

bool run_macro(String* macro, String** info, int argnum, MacroArr* m, FILE* stream) {
    if (equal(info[0]->string, "expandafter")) {
        return process_macro(macro, info, argnum, 2, m, stream, &expandafter);
    } else if (equal(info[0]->string, "include")) {
        return process_macro(macro, info, argnum, 1, m, stream, &include);
    } else if (equal(info[0]->string, "if")) {
        return process_macro(macro, info, argnum, 3, m, stream, &ifmacro);
    } else { 
        return custom(macro, info, argnum, m, stream);
    }
}

bool process_macro(String* macro, String** info, int argnum, int argreq, MacroArr* m, FILE* stream, bool(*func)(String*, String**, MacroArr* m, FILE* stream)) {
    bool err = argcheck(argnum, argreq);
    if (err) {
        return err;
    }
    String* after = argsave(macro, info, argreq);
    err = func(macro, info, m, stream);
    if (!err) {
        cat(macro, after);
    }
    free_string(after);
    return err;
}

bool process_custom(String* macro, String** info, int argnum, int argreq, MacroArr* m, FILE* stream, bool(*func)(String*, String**, MacroArr*, FILE*, int)) {
    int loc = macro_in_custom(info[1], m);
    bool err = argcheck(argnum, argreq);
    if (err) {
        return err;
    }
    String* after = argsave(macro, info, argreq);
    err = func(macro, info, m, stream, loc);
    if (!err) {
        cat(macro, after);
    }
    free_string(after);
    return err;
}

bool expandafter (String* macro, String** info, MacroArr* m, FILE* stream) {
    bool err = parse_string(info[2], m, stream, false);
    if (!err) {
        cat(info[1], info[2]);
        cpy(macro, info[1]);
    }
    return err;
}

bool include (String* macro, String** info, MacroArr* m, FILE* stream) {
    String* s = file_to_str(fopen(info[1]->string, "r"));
    cpy(macro, s);
    free_string(s);
    return false;
}

bool ifmacro (String* macro, String** info, MacroArr* m, FILE* stream) {
    if (info[1]->currlen > 0) {
        cpy(macro, info[2]);
    } else {
        cpy(macro, info[3]);
    }
    return false;
}

bool def (String* macro, String** info, MacroArr* m, FILE* stream, int loc) {
    bool err = false;
    if (loc != -1) {
        return true;
    }
    err = checkname(info[1]);
    if (err) {
        return err;
    }
    if (m->currlen + 1 == m->maxlen) {
        double_macro_arr(m);
    }
    cpy(m->names[m->currlen], info[1]);
    cpy(m->vals[m->currlen], info[2]);
    (m->currlen)++;
    strcpy(macro->string, "");
    macro->currlen = 0;
    return false;
}

bool undef (String* macro, String** info, MacroArr* m, FILE* stream, int loc) {
    if (loc == -1) {
        return true;
    }
    if (loc != m->currlen - 1) {
        cpy(m->names[loc], m->names[m->currlen-1]);
        cpy(m->vals[loc], m->vals[m->currlen-1]);
    }
    (m->currlen)--;
    strcpy(macro->string, "");
    macro->currlen = 0;
    return false;
}

bool ifdef (String* macro, String** info, MacroArr* m, FILE* stream, int loc) {
    if (loc != -1) {
        cpy(macro, info[2]);
    } else {
        cpy(macro, info[3]);
    }
    return false;
}

bool generic (String* macro, String** info, MacroArr* m, FILE* stream, int loc) {
    loc = macro_in_custom(info[0], m);
    if (loc == -1) {
        return true;
    }
    cpy(macro, m->vals[loc]);
    replace_hash(macro, info[1]);
    return false;
}

bool custom (String* macro, String** info, int argnum, MacroArr* m, FILE* stream) {
    if (equal(info[0]->string, "def")) {
        return process_custom(macro, info, argnum, 2, m, stream, &def);
    } else if (equal(info[0]->string, "undef")) {
        return process_custom(macro, info, argnum, 1, m, stream, &undef);
    } else if (equal(info[0]->string, "ifdef")) {
        return process_custom(macro, info, argnum, 3, m, stream, &ifdef);
    } else {
        return process_custom(macro, info, argnum, 1, m, stream, &generic);
    }
}

bool parse_string(String* string, MacroArr* m, FILE* stream, bool remove) {
    // replace parsed macro with whatever returned in-place
    // freeing is determined by the functions that use the string, not this one
    char curr = string->string[0];
    int i = 0;
    int comment_start = 0;
    enum string_state status = text;
    enum comment_state comment = none;
    bool can_add = true;
    bool err = false;

    while (curr != '\0') {
        if (comment == orig) {
            if (curr == '\n') {
                comment = new;
            }
        } else if (comment == new) {
            if (curr != ' ' && curr != '\t') {
                char* temp = (char*) malloc(sizeof(char) * string->maxlen);
                strncpy(temp, string->string, comment_start);
                temp[comment_start] = '\0';
                strcat(temp, &(string->string[i]));
                free(string->string);
                string->string = temp;
                string->currlen = string->currlen - (i - comment_start);
                i = comment_start;
                comment = none;
            }
        } else {
            if (status == text) {
                if (curr == '%') {
                    // shuffle(string, "", i);
                    comment = orig;
                    comment_start = i;
                } else if (curr == '\\') {
                    status = backslash;
                }
            } else if (status == backslash) {
                if (check_char(curr, false)) { // escape character
                    status = text;
                    if (remove || curr != '%') {
                        shuffle(string, &(string->string[i]), i-1);
                        i--;
                    }
                } else if (check_char(curr, true)) { // alphahnumeric, which means macro
                    status = text;
                    shuffle(string, &(string->string[i]), i-1);
                    i--;
                    
                    String* macro = create_string();
                    String* old = create_string();
                    assign_string(macro, &(string->string[i]));
                    assign_string(old, &(string->string[i]));
                    err = parse_macro(macro, m, stream);
                    if (err) {
                        free_string(macro);
                        free_string(old);
                        return err;
                    } else {
                        replace(string, old, macro);
                        free_string(macro);
                        free_string(old);
                    }

                    i--;
                } else {
                    status = text;
                }
            }
        }

        if (string->string[i+1] == '\0' && can_add) {
            if (comment != none) {
                can_add = add_line(string, stream);
            }
        }

        i++;
        curr = string->string[i];
    }
    return err;
}

bool parse_macro(String* macro, MacroArr* m, FILE* stream) {
    // accepts a macro and any after
    // parse macro and then replace text in-place, do not malloc and return string
    InfoArr* info = create_InfoArr();
    enum macro_state status = name;
    bool backslash = false;
    int sep = 0;
    int brackets = 0;
    int i = 0;
    bool can_add = true;
    bool err = false;

    while (i < macro->currlen) {
        if (macro->string[i] == '{' && !backslash) {
            if (status == name) {
                add_to_info(macro->string, i, info);
                sep = i;
            } else if (brackets == 0) {
                sep = i;
            }
            status = arg;
            backslash = false;
            brackets++;
        } else if (macro->string[i] == '}' && !backslash) {
            if (brackets != 0) {
                brackets--;
                if (brackets == 0) {
                    add_to_info(&(macro->string[sep+1]), i-sep-1, info);
                }
            }
            status = other;
            backslash = false;
        } else if (macro->string[i] == '\\' && !backslash && status != name) {
            backslash = true;
        } else if (status == name && !check_char(macro->string[i], true)) {
            free_InfoArr(info);
            return true;
        } else if (brackets == 0 && macro->string[i] != '{' && status != name) {
            break;
        } else {
            backslash = false; 
        }

        if (macro->string[i+1] == '\0' && can_add) {
            if (brackets != 0) {
                can_add = add_line(macro, stream);
            }
        }
        
        i++;
    }
    if (brackets > 0) {
        free_InfoArr(info);
        return true;
    }

    if (info->currlen < 1) {
        free_InfoArr(info);
        return true;
    }
    
    String* old = create_string();
    String* new = create_string();
    ncpy(old, macro, i);
    ncpy(new, macro, i);
    err = run_macro(new, info->info, info->currlen-1, m, stream);
    if (!err) {
        replace(macro, old, new);
    }
    free_string(old);
    free_string(new);
    free_InfoArr(info);
    return err;
}
