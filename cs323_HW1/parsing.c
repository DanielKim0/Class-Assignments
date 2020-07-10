#include "parsing.h"

String* create_string() {
    String* s = malloc(sizeof(String));
    char* c = malloc(BUFFER * sizeof(char));
    *s = (String) {.currlen = 0, .maxlen = BUFFER, .string = c};
    return s;
}

void cpy (String* a, String* b) {
    while (b->currlen >= a->maxlen) {
        a->string = DOUBLE(a->string, a->maxlen);
    }
    strcpy(a->string, b->string);
    a->currlen = b->currlen;
}

void cat (String* a, String* b) {
    while (b->currlen + a->currlen >= a->maxlen) {
        a->string = DOUBLE(a->string, a->maxlen);
    }
    strcat(a->string, b->string);
    a->currlen += b->currlen;
}

void charcat(String* a, char* b) {
    int len = strlen(b);
    while (len + a->currlen >= a->maxlen) {
        a->string = DOUBLE(a->string, a->maxlen);
    }
    strcat(a->string, b);
    a->currlen += len;
}

void ncpy (String* a, String* b, int n) {
    while (n >= a->maxlen) {
        a->string = DOUBLE(a->string, a->maxlen);
    }
    strncpy(a->string, b->string, n);
    a->string[n] = '\0';
    a->currlen = n;
}

void assign_string(String* a, char* b) {
    int len = strlen(b);
    while (len >= a->maxlen) {
        a->string = DOUBLE(a->string, a->maxlen);
    }
    strcpy(a->string, b);
    a->currlen = len;
}

void shuffle(String* a, char* b, int i) {
    String* buffer = create_string();
    ncpy(buffer, a, i);
    charcat(buffer, b);
    cpy(a, buffer);
    free_string(buffer);
}

void null_string(String* s) {
    free(s->string);
    s->string = NULL;
    s->currlen = 0;
}

void free_string(String* s) {
    free(s->string);
    free(s);
}

bool equal(char* a, char* b) {
    return strcmp(a, b) == 0;
}

bool argcheck(int argnum, int required) {
    if (argnum < required) {
        return true;
    }
    return false;
}

String* argsave(String* macro, String** info, int argnum) {
    String* result = create_string();
    int len = -2;
    for (int i = 0; i < argnum+1; i++) {
        len += info[i]->currlen;
        len += 2;
    }
    assign_string(result, &(macro->string[len]));
    return result;
}

int macro_in_custom(String* macro, MacroArr* m) {
    for (int i = 0; i < m->currlen; i++) {
        if (equal(macro->string, m->names[i]->string)) {
            return i;
        }
    }
    return -1;
}

bool check_char(char c, bool alpha) {
    char alphanumeric[100] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890\0";
    char escape[10] = "%%\\#{}\0";
    if (alpha) {
        return (strchr(alphanumeric, c) != NULL);
    } else {
        return (strchr(escape, c) != NULL);
    }   
}

String** double_string_arr(String** arr, int i) {
    arr = DOUBLE(arr, i);
    for (int j = i/2; j < i; j++) {
        arr[j] = create_string();
    }
    return arr;
}

String** create_string_arr() {
    String** newarr = malloc(BUFFER * sizeof(String*));
    for(int i = 0; i < BUFFER; i++) {
        newarr[i] = create_string();
    }
    return newarr;
}

void double_macro_arr(MacroArr* macro) {
    int i = macro->maxlen;
    macro->names = double_string_arr(macro->names, i);
    macro->vals = double_string_arr(macro->vals, i);
    macro->maxlen = macro->maxlen * 2;
}

MacroArr* create_macro_arr() {
    MacroArr* macro = malloc(sizeof(MacroArr));
    macro->currlen = 0;
    macro->maxlen = BUFFER;
    macro->names = create_string_arr();
    macro->vals = create_string_arr();
    return macro;
}

void free_string_arr(String** arr, int len) {
    for(int i = 0; i < len; i++) {
        free_string(arr[i]);
    }
    free(arr);
}

void free_macro_arr(MacroArr* m) {
    free_string_arr(m->names, m->maxlen);
    free_string_arr(m->vals, m->maxlen);
    free(m);
}


void replace(String* string, String* find, String* replace) {
    String* buffer = create_string();
    String* temp = create_string();
    char* found;
    if ((found = strstr(string->string, find->string)) != NULL) {
        assign_string(temp, found);
        ncpy(buffer, string, found-string->string);
        cat(buffer, replace);
        charcat(buffer, temp->string + find->currlen);
        cpy(string, buffer);
    }
    free_string(buffer);
    free_string(temp);
}


void replace_hash(String* string, String* replace) {
    char curr = string->string[0];
    int i = 0;
    bool backslash = false;

    String* buffer = create_string();
    while (curr != '\0') {
        if (curr == '\\') {
            backslash = true;
        } else if (!backslash && curr == '#') {
            // replace #, at point i, with replace string
            ncpy(buffer, string, i);
            cat(buffer, replace);
            charcat(buffer, &(string->string[i+1]));
            cpy(string, buffer);
        } else {
            backslash = false;
        }
        i++;
        curr = string->string[i];
    }
    free_string(buffer);
}

String* file_to_str(FILE* f) {
    String* string = create_string();
    String* final = create_string();
    int lines = 0;
    while (getline(&(string->string), &(string->maxlen), f) != -1) {
        string->currlen = strlen(string->string);
        if (lines == 0) {
            assign_string(final, string->string);
        } else {
            cat(final, string);
        }
        lines++;
    }
    free_string(string);
    return final;
}

bool checkname (String* s) {
    if (s->currlen == 0) {
        return true;
    }
    for (int i = 0; i < s->currlen; i++) {
        if (!check_char(s->string[i], true)) {
            return true;
        }
    }
    return false;
}

bool add_line(String* s, FILE* stream) {
    String* string = create_string();
    bool success;
    if (getline(&(string->string), &(string->maxlen), stream) != -1) {
        string->currlen = strlen(string->string);
        cat(s, string);
        success = true;
    } else {
        success = false;
    }
    free_string(string);
    return success;
}

InfoArr* create_InfoArr() {
    InfoArr* newarr = malloc(sizeof(InfoArr));
    String** c = create_string_arr();
    *newarr = (InfoArr) {.currlen = 0, .maxlen = BUFFER, .info = c};
    return newarr;
}

void free_InfoArr(InfoArr* info) {
    free_string_arr(info->info, info->maxlen);
    free(info);
}

void add_to_info(char* s, int n, InfoArr* info) {
    if (info->currlen + 1 >= info->maxlen) {
        info->info = double_string_arr(info->info, info->maxlen);
        info->maxlen *= 2;
    }
    
    String* temp = create_string();
    assign_string(temp, s);
    ncpy(info->info[info->currlen], temp, n);
    free_string(temp);
    info->currlen++;
}