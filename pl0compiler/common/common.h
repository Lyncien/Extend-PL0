#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>

struct _tPL0Lex;

typedef struct _tPL0Compiler {
	struct _tPL0Lex * lex;
} PL0Compiler;


typedef enum BOOLEAN {
	FALSE,
	TRUE,
} BOOL;


/* Global variables */
extern FILE * fin;
extern FILE * fout;
extern PL0Compiler * compiler;

/* Functions */
PL0Compiler * PL0Compiler_create();
BOOL PL0Compiler_destroy(PL0Compiler * compiler);

#endif /* common.h */
