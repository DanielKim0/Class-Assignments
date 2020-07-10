// proj1.h                                          Stan Eisenstat (09/17/15)
//
// System header files and macros for proj1

#define _GNU_SOURCE
#define BUFFER 16
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Write message to stderr using format FORMAT
#define WARN(format,...) fprintf (stderr, "proj1: " format "\n", __VA_ARGS__)

// Write message to stderr using format FORMAT and exit.
#define DIE(format,...) WARN(format,__VA_ARGS__), exit (EXIT_FAILURE)

// Double the size of an allocated block PTR with NMEMB members and update
// NMEMB accordingly.  (NMEMB is only the size in bytes if PTR is a char *.)
#define DOUBLE(ptr,nmemb) realloc (ptr, (nmemb *= 2) * sizeof(*ptr))

enum string_state {text, backslash};
enum macro_state {name, arg, other, err};
enum comment_state {orig, new, none};

typedef struct String { 
   size_t currlen;
   size_t maxlen;
   char* string;
} String;

typedef struct MacroArr {
   int currlen;
   int maxlen;
   String** names;
   String** vals;
} MacroArr;

typedef struct InfoArr {
   int currlen;
   int maxlen;
   String** info;
} InfoArr;

String* create_string();
void cpy (String* a, String* b);
void cat (String* a, String* b);
void charcat(String* a, char* b);
void ncpy (String* a, String* b, int n);
void assign_string(String* a, char* b);
void shuffle(String* a, char* b, int i);
void null_string(String* s);
void free_string(String* s);
bool equal(char* a, char* b);
bool argcheck(int argnum, int required);
String* argsave(String* macro, String** info, int argnum);
int macro_in_custom(String* macro, MacroArr* m);
bool check_char(char c, bool alpha);
String** double_string_arr(String** arr, int i);
String** create_string_arr();
void double_macro_arr(MacroArr* macro);
MacroArr* create_macro_arr();
void free_string_arr(String** arr, int len);
void free_macro_arr(MacroArr* m);
void replace(String* string, String* find, String* replace);
void replace_hash(String* string, String* replace);
String* file_to_str(FILE* f);
bool checkname (String* s);
bool add_line(String* s, FILE* stream);
InfoArr* create_InfoArr();
void free_InfoArr(InfoArr* info);
void add_to_info(char* s, int n, InfoArr* info);
