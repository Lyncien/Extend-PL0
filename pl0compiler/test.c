#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common/common.h"
#include "lex/pl0_lex.h"
#include "syntax/pl0_tax.h"
void show_help(const char * arg0)
{
	printf("Usage: %s <src> <dest>\n\n", arg0);
}

int main(int argc, char * argv[])
{
	/* Parse arguments */
	if (argc != 3)
	{
		show_help(argv[0]);
		exit(-1);
	}
	fin = fopen(argv[1], "rb");
	fout = fopen(argv[2], "wb");
	if (!fin)
	{
		printf("Fatal: Cannot open file for reading: %s\n", argv[1]);
		exit(-1);
	}
	if (!fout)
	{
		printf("Fatal: Cannot open file for writing: %s\n", argv[2]);
		exit(-1);
	}

	/* Setup */
	compiler = PL0Compiler_create();
	PL0Lex * lex = compiler->lex;


	//syntax(lex,NULL);
	PL0Lex_get_token(lex);
	program(lex);//change from calling "program_block" to calling "program"

	/* Cleanup */
	fclose(fin);

	/* Finish */
	printf("=== Normal end for PL0Compiler. ===\n");
	return 0;
}
