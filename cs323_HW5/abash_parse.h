// abash_parse.h                                        Stan Eisenstat (11/19/19)
//
// Header file for command line parser used in Parse
//
// Bash version based on recursive descent parse tree
#ifndef _ABASH_PARSE_H_
#define _ABASH_PARSE_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#ifndef METACHARS
#include "abash_lex.h"
#endif

/////////////////////////////////////////////////////////////////////////////

// Token types used by parse()

enum {

      NONE   = -1,          // Nontoken: Did not find a token
      ERROR  = -2,          // Nontoken: Encountered an error
      PIPE   = REDIR_PIPE,  // Nontoken: CMD struct for pipe
      SUBCMD = -4           // Nontoken: CMD struct for subcommand
};


// String containing all characters that may appear in variable names
#define VARCHR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789"


// Macro that checks whether a token is a redirection symbol
#define REDIR_OP(type) (type == REDIR_IN  || type == REDIR_HERE || \
			type == REDIR_OUT || type == REDIR_APP)

/////////////////////////////////////////////////////////////////////////////
//
// The syntax for a command is
//
//  <local>    = VARIABLE=VALUE
//  <red_op>   = < / << / > / >>
//  <redirect> = <red_op> FILENAME
//  <prefix>   = <local> / <redirect> / <local> <prefix> / <redirect> <prefix>
//  <suffix>   = SIMPLE / <redirect> / SIMPLE <suffix> / <redirect> <suffix>
//  <redList>  = <redirect> / <redirect> <redList>
//  <simple>   = SIMPLE / <prefix> SIMPLE / SIMPLE <suffix>
//                      / <prefix> SIMPLE <suffix>
//  <subcmd>   = (<command>) / <prefix> (<command>) / (<command>) <redList>
//                           / <prefix> (<command>) <redList>
//  <stage>    = <simple> / <subcmd>
//  <pipeline> = <stage> / <stage> | <pipeline>
//  <and-or>   = <pipeline> / <pipeline> && <and-or> / <pipeline> || <and-or>
//  <command>  = <and-or> / <and-or> ; <command> / <and-or> ;
//                        / <and-or> & <command> / <and-or> &
//
// parse() parses a command into a tree of CMD structs containing its <simple>
// commands and the "operators" | (= PIPE), && (= SEP_AND), || (= SEP_OR), ;
// (= SEP_END), & (= SEP_BG), and SUBCMD.  The tree corresponds to how the
// command is parsed by a recursive descent parser for the grammar above (see
// https://en.wikipedia.org/wiki/Recursive_descent_parser).
//
// The tree for a <simple> is a single struct of type SIMPLE that specifies its
// arguments (argc, argv[]); its local variables (nLocal, locVar[], locVal[]);
// and whether and where to redirect its standard input (fromType, fromFile)
// and its standard output (toType, toFile).  The left and right children are
// NULL.
//
// The tree for a <stage> is either the tree for a <simple> or a CMD struct of
// type SUBCMD (which may have local variables and redirection) whose left
// child is the tree representing a <command> and whose right child is NULL.
// Note that I/O redirection is associated with the first/last stage, not the
// pipeline).
//
// The tree for a <pipeline> is either the tree for a <stage> or a CMD struct
// of type PIPE whose left child is the tree representing the <stage> and whose
// right child is the tree representing the rest of the <pipeline>.
//
// The tree for an <and-or> is either the tree for a <pipeline> or a CMD struct
// of type && (= SEP_AND) or || (= SEP_OR) whose left child is the tree
// representing the <pipeline> and whose right child is the tree representing
// the <and-or>.
//
// The tree for a <command> is either the tree for an <and-or> or a CMD struct
// of type ; (= SEP_END) or & (= SEP_BG) whose left child is the tree
// representing the <and-or> and whose right child is either NULL or the tree
// representing the <command>.


// Examples (where A, B, C, D, and E are <simple>):                          //
//                                                                           //
//   < A B | C | D | E > F           PIPE                                    //
//                                  /    \                                   //
//                                B <A    PIPE                               //
//                                       /    \                              //
//                                      C      PIPE                          //
//                                            /    \                         //
//                                           D      E >F                     //
//                                                                           //
//   A && B || C && D              &&                                        //
//                                /  \                                       //
//                               A    ||                                     //
//                                   /  \                                    //
//                                  B    &&                                  //
//                                      /  \                                 //
//                                     C    D                                //
//                                                                           //
//   A ; B & C ; D || E            ;                                         //
//                                / \                                        //
//                               A   &                                       //
//                                  / \                                      //
//                                 B   ;                                     //
//                                    / \                                    //
//                                   C   ||                                  //
//                                      /  \                                 //
//                                     D    E                                //
//                                                                           //
//   (A ; B &) | (C || D) && E                 &&                            //
//                                            /  \                           //
//                                        PIPE    E                          //
//                                       /    \                              //
//                                 SUBCMD      SUBCMD                        //
//                                  /           /                            //
//                                 ;          ||                             //
//                                / \        /  \                            //
//                               A   &      C    D                           //
//                                  /                                        //
//                                 B                                         //
//                                                                           //
//   A ;                           ;                                         //
//                                /                                          //
//                               B                                           //
//                                                                           //

typedef struct cmd {
  int type;             // Node type: SIMPLE, PIPE, SEP_AND, SEP_OR, SEP_END,
			//   SEP_BG, SUBCMD, or NONE (default)

  int argc;             // Number of command-line arguments
  char **argv;          // Null-terminated argument vector or NULL

  int nLocal;           // Number of local variable assignments
  char **locVar;        // Array of local variable names and the values to
  char **locVal;        //   assign to them when the command executes

  int fromType;         // Redirect stdin: NONE (default), REDIR_IN (<), or
			//   REDIR_HERE (<<)
  char *fromFile;       // File to redirect stdin. contents of here document,
			//   or NULL (default)

  int toType;           // Redirect stdout: NONE (default), REDIR_OUT (>), or
			//   REDIR_APP (>>)
  char *toFile;         // File to redirect stdout or NULL (default)

  struct cmd *left;     // Left subtree or NULL (default)
  struct cmd *right;    // Right subtree or NULL (default)
} CMD;

// Note:  In a <stage> with a HERE document, fromFile should point to a string
// containing the lines in that document.
//


// Allocate, initialize, and return a pointer to an empty command structure
CMD *mallocCMD (void);


// Print the command data structure CMD as a tree whose root is at level LEVEL
void printTree (CMD *exec, int level);


// Free the command structure CMD
void freeCMD (CMD *cmd);


// Parse a token list into a command structure and return a pointer to
// that structure (NULL if errors found).
CMD *parse (token *tok);

#endif /* _ABASH_PARSE_H_ */
