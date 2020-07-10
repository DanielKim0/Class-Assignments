#include "proj1.h"

int main(int argc, char *argv[]) {
    MacroArr* m = create_macro_arr();
    if (argc < 2) {
        parse_file(stdin, m);
    } else {
        for (int i = 1; i < argc; i++) {
            parse_file(fopen(argv[i], "r"), m);
        }
    }
    free_macro_arr(m);
    return 0;
}

void parse_file(FILE* stream, MacroArr* m) {
    String* string = create_string();
    bool err;
    while (getline(&(string->string), &(string->maxlen), stream) != -1) {
        string->currlen = strlen(string->string);
        err = parse_string(string, m, stream, true);
        if (err == true) {
            DIE("%s", "Fatal Error!");
        }
        printf("%s", string->string);
    }
    free_string(string);
}
