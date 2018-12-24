#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common/common.h"
#include "lex/pl0_lex.h"
#include "syntax/pl0_tax.h"

void show_help(const char * arg0)
{
	printf("Usage: %s <src>\n\n", arg0);
}

int main(int argc, char * argv[])
{
	/* Parse arguments */
	if (argc != 2)
	{
		show_help(argv[0]);
		exit(-1);
	}
	fin = fopen(argv[1], "rb");
	if (!fin)
	{
		printf("Fatal: Cannot open file for reading: %s\n", argv[1]);
		exit(-1);
	}

	/* Start */
	printf("=== This is the program for PL/0 testing ===\n");

	/* Setup */
	compiler = PL0Compiler_create();

	/* Lexical Analysis */
	printf("--- Lexical Analysis testing ---\n");
	PL0Lex * lex = compiler->lex;
	/*while (PL0Lex_get_token(lex))
	{
		assert(lex->last_token_type != TOKEN_NULL);
		assert(lex->last_token_type != TOKEN_RESWORDS);
		assert(lex->last_token_type != TOKEN_SYMBOL);
		if (lex->last_token_type == TOKEN_IDENTIFIER)
		{
			printf("Identifier:    %-10s %d:%d-%d\n", lex->last_id,lex->line,lex->pos+1-lex->last_token_len,lex->pos);
		}
		else if (lex->last_token_type == TOKEN_NUMBER)
		{
			printf("Number:        %-10d %d:%d-%d\n", lex->last_num,lex->line,lex->pos+1-lex->last_token_len,lex->pos);
		}
		else if (lex->last_token_type > TOKEN_RESWORDS)
		{
			printf("Reserved word: %-10s %d:%d-%d\n", TOKEN_RESERVED_WORDS[lex->last_token_type - TOKEN_RESWORDS - 1],lex->line,lex->pos+1-lex->last_token_len,lex->pos);
		}
		else
		{
			printf("Symbol:        %-10s %d:%d-%d\n", TOKEN_SYMBOLS[lex->last_token_type - TOKEN_SYMBOL - 1],lex->line,lex->pos+1-lex->last_token_len,lex->pos);
		}
	}*/

	printf("--- Lexical Analysis testing end. ---\n");

	/*test syntax ayalysis*/
	//fseek(fin,0,SEEK_SET);
	PL0Lex_get_token(lex);
	program(lex);//change from calling "program_block" to calling "program"

	/* Cleanup */
	fclose(fin);

	/* Finish */
	printf("=== Normal end for testing. ===\n");
	return 0;
}
