#include "parsing.h"

bool run_macro(String* macro, String** info, int argnum, MacroArr* m, FILE* stream);
bool process_macro(String* macro, String** info, int argnum, int argreq, MacroArr* m, FILE* stream, bool(*func)(String*, String**, MacroArr*, FILE*));
bool process_custom(String* macro, String** info, int argnum, int argreq, MacroArr* m, FILE* stream, bool(*func)(String*, String**, MacroArr*, FILE*, int));
bool expandafter (String* macro, String** info, MacroArr* m, FILE* stream);
bool include (String* macro, String** info, MacroArr* m, FILE* stream);
bool ifmacro (String* macro, String** info, MacroArr* m, FILE* stream);
bool def (String* macro, String** info, MacroArr* m, FILE* stream, int loc);
bool undef (String* macro, String** info, MacroArr* m, FILE* stream, int loc);
bool ifdef (String* macro, String** info, MacroArr* m, FILE* stream, int loc);
bool generic (String* macro, String** info, MacroArr* m, FILE* stream, int loc);
bool custom (String* macro, String** info, int argnum, MacroArr* m, FILE* stream);
bool parse_string(String* string, MacroArr* m, FILE* stream, bool remove);
bool parse_macro(String* macro, MacroArr* m, FILE* stream);
int macro_in_custom(String* macro, MacroArr* m);
