// abash_lex.h                                      Stan Eisenstat (09/18/19)
//
// Header file for lexer and history mechanism used in History.
#ifndef _ABASH_LEX_H_
#define _ABASH_LEX_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// A token is one of
//
//  1) a maximal contiguous sequence of nonblank printing characters other
//       than the metacharacters <, >, ;, &, |, (, and ).
//
//  2) a redirection symbol (<, <<, >, >>, or |);
//
//  3) a command separator (;, &, &&, or ||)
//
//  4) a left or right parenthesis (used to group commands).
//
// The token type is specified by the symbolic constants (and the corresponding
// values) defined below.
//
// Note:  Your source files should use only the symbolic names for the types,
// not their values.  These values were chosen so that each is a power of two
// (<< is the shift operator) and thus corresponds to a unique bit in an int.
// This may be of value when testing whether a token belongs to one of a group
// of types (e.g., a redirection symbol).

enum {
    SIMPLE        = (1 << 0),   // Maximal contiguous sequence ... (as above)

    REDIR_IN      = (1 << 1),   // <    Redirect stdin to file
    REDIR_HERE    = (1 << 2),   // <<   Redirect stdin to HERE document

    REDIR_OUT     = (1 << 3),   // >    Redirect stdout to file
    REDIR_APP     = (1 << 4),   // >>   Append stdout to file

    REDIR_PIPE    = (1 << 5),   // |

    SEP_AND       = (1 << 6),   // &&
    SEP_OR        = (1 << 7),   // ||

    SEP_END       = (1 << 8),   // ;
    SEP_BG        = (1 << 9),   // &

    PAREN_LEFT    = (1 << 10),  // (
    PAREN_RIGHT   = (1 << 11),  // )
};

#define METACHARS "<>;&|()"     // Chars that start a non-SIMPLE


// A token list is a headless linked list of typed tokens.  All storage is
// allocated by malloc() / realloc().

typedef struct token {          // Struct for each token in linked list
  char *text;                   //   String containing token
  int type;                     //   Corresponding type
  struct token *next;           //   Pointer to next token in linked list
} token;


// lex() breaks the string LINE into a headless linked list of typed tokens
// and returns a pointer to the first token (or NULL if none were found).
token *lex (char * const line);


// Print list of tokens LIST together with their types.
void printList (token *list);


// Free list of tokens LIST (implemented in mainHistory.c)
void freeList (token *list);

////////////////////////////////////////////////////////////////////////////////

// Initialize the history mechanism so that the index of the last command
// stored is equal to 0 (since no commands are stored), and return the index
// (= 1) of the next command.

int hInit (void);


// Apply history expansion to LINE and return a copy of the expanded line.
// Set *STATUS to 1 if all substitutions were successful, to 0 if there were
// no substitutions, and to -1 if at least one substitution failed.

char *hExpand (char * const line, int *status);


// Add (a copy of) the command represented by the token list LIST to the list
// of remembered commands, and return the index of the next command.

int hSave (token *list);


// Write the most recent N remembered commands by increasing number to the
// standard output using something equivalent to
//
//   printf ("%6d  %s %s ... %s\n", icmd, token0, token1, ..., tokenLast)
//
// where ICMD is the number of the command and TOKEN0, ..., TOKENLAST are its
// tokens.

void hList (int n);

#endif /* _ABASH_LEX_H_ */
