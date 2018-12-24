#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "pl0_lex.h"
#include "../error/pl0_error.h"

const char * TOKEN_RESERVED_WORDS[NRW] = {"var", "const", "procedure", "begin", "end", "if", "then", "do", "while", "call", "odd","return","else","elif","print"};
const char * TOKEN_SYMBOLS[NSYM] = { "+", "-", "*", "/", "=", "!=", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=" ,"!","&&","||"};

CharType chartypetable[128]={\
	ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,\
	TAB,NEWLINE,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,\
	ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,\
	ILLEGAL,SPACE,EXCLAMA,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,AND,ILLEGAL,LPAREN,RPAREN,TIMES,PLUS,\
	COMMA,MINUS,PERIOD,SLASH,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,DIGIT,\
	COLON,SEMICOLON,LES,EQU,GTR,ILLEGAL,ILLEGAL,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,\
	LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,\
	LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,ILLEGAL,ILLEGAL,ILLEGAL,ILLEGAL,\
	LETTER,ILLEGAL,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,\
	LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,LETTER,\
	LETTER,LETTER,ILLEGAL,OR,ILLEGAL,ILLEGAL,ILLEGAL\
};

State statetable[N_CHARTYPE-1][N_STATE_NO_AC]={\
	//N_CHARTYPE-1:first chartype "illegal" don't need translate
	{IN_ID,SLASH_AC,LES_AC,GTR_AC,NUM_AC,IN_ID,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{IN_NUM,SLASH_AC,LES_AC,GTR_AC,IN_NUM,IN_ID,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{START,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{START,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{START,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,START,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{PLUS_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{MINUS_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{TIMES_AC,IN_BLOCKCOMMENTS,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,TO_END_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{SLASH_SEL,IN_LINECOMMENTS,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,START,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{EQU_AC,SLASH_AC,LEQ_AC,GEQ_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,BECOMES_AC,NEQ_AC,ERROR2,ERROR3},\
	{LES_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{GTR_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{LPAREN_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{RPAREN_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{COMMA_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{SEMICOLON_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{PERIOD_AC,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{COLON_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{EXCLAMA_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,ERROR3},\
	{AND_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,AND_AC,ERROR3},\
	{OR_SEL,SLASH_AC,LES_AC,GTR_AC,NUM_AC,ID_AC,IN_LINECOMMENTS,IN_BLOCKCOMMENTS,IN_BLOCKCOMMENTS,ERROR1,NOT_AC,ERROR2,OR_AC}\
};

int state_posback[N_STATE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1};
int symbols_len[NSYM] = {1,1,1,1,1,2,1,2,1,2,1,1,1,1,1,2,1,2,2};

PL0Lex * PL0Lex_create(PL0Compiler * parent)
{
	PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
	lex->compiler = parent;
	lex->line = 0;
	lex->pos = 0;
	memset(lex->linebuf,0,256);
	return lex;
}

BOOL PL0Lex_destroy(PL0Lex * lex)
{
	free(lex);
	return TRUE;
}

char ch = 0;
int endoffile = 0;
BOOL PL0Lex_get_token(PL0Lex * lex)
{
	if(endoffile){
		lex->last_token_type = TOKEN_NULL;
		return FALSE;
	}
	State state = START;
	CharType chartype;

	int i;

	int num = 0;
	int num_len = 0;

	int id_len = 0;
	char id[MAX_ID_LEN + 1] = {};

	while(1){

		ch = lex->linebuf[lex->pos++];
		if(ch == 0){//get a new line
			if(!fgets(lex->linebuf,256,fin)){
				endoffile = 1;
				lex->last_token_type = TOKEN_NULL;
				break;//end of file
			}
				
            lex->pos = 0;
            lex->line++;
            ch = lex->linebuf[lex->pos++];
		}


		chartype = chartypetable[ch];//get char type

		if(chartype == ILLEGAL){
			if(state != IN_BLOCKCOMMENTS && state != IN_LINECOMMENTS){
				PL0_print_error(ERR_UNRECOGNIZED_CHAR,lex->linebuf,lex->line,lex->pos);
				continue;//continue to get a token
			}
		}
		else{
			state = statetable[chartype][state];//state translate
		}

		//printf("[%d/%d/%d]",ch,chartype,state);

		if(state == IN_NUM){
			if(num_len <= MAX_NUM_LEN)
				num = num * 10 + ch - '0';
			num_len++;
		}
		else if(state==IN_ID){
			if(id_len <= MAX_ID_LEN)
				id[id_len] = ch;
			id_len++;
		}
		else if(state >= N_STATE_NO_AC){//accept state
			if(state < NUM_AC){//accept a symbol
				lex->last_token_type = TOKEN_SYMBOL + state - N_STATE_NO_AC + 1;
				lex->last_token_len = symbols_len[state - N_STATE_NO_AC];
			}
			else if(state == NUM_AC){//accept a number
				lex->last_token_type = TOKEN_NUMBER;
				if(num_len > MAX_NUM_LEN){
					PL0_print_error(ERR_TOO_LONG_IDEN_OR_NUM,lex->linebuf,lex->line,lex->pos);
					num_len = MAX_NUM_LEN;
				}
				lex->last_token_len = num_len;
				lex->last_num = num;
				//-----
				num_len = 0;
				num = 0;
				//-----
				if(chartypetable[ch] == LETTER) //error: the ch after a num is LETTER
					PL0_print_error(ERR_WRONG_IDEN_FORMAT,lex->linebuf,lex->line,lex->pos);
			}
			else if(state == ID_AC){//accept a id or reserved word
				if(id_len > MAX_ID_LEN){
					PL0_print_error(ERR_TOO_LONG_IDEN_OR_NUM,lex->linebuf,lex->line,lex->pos);
					id_len = MAX_ID_LEN;
				}
				id[id_len] = '\0';
				//-----
				for(i = 0;i < NRW;i++)
					if(!strcmp(TOKEN_RESERVED_WORDS[i],id))
						break;
				if(i != NRW){
					lex->last_token_type = TOKEN_RESWORDS + i + 1;
					strcpy(lex->last_id,TOKEN_RESERVED_WORDS[i]);
				}
				else{
					lex->last_token_type = TOKEN_IDENTIFIER;
					strcpy(lex->last_id,id);
				}
				lex->last_token_len = strlen(lex->last_id);
				//-----
				memset(id,0,MAX_ID_LEN + 1);
				id_len = 0;
				//-----
			}
			else if(state == ERROR1){
				PL0_print_error(ERR_COLON_EXPECT_EQUAL,lex->linebuf,lex->line,lex->pos);
				lex->last_token_type = TOKEN_BECOMES; //return ":=" but len is 1
				lex->last_token_len = 1;
			}
			else if(state == ERROR2){
				PL0_print_error(ERR_EXPECT_AND,lex->linebuf,lex->line,lex->pos);
				lex->last_token_type = TOKEN_AND;	//return "&&" but len is 1
				lex->last_token_len = 1;
			}
			else if(state == ERROR3){
				PL0_print_error(ERR_EXPECT_OR,lex->linebuf,lex->line,lex->pos);
				lex->last_token_type = TOKEN_OR;   //return "||" but len is 1
				lex->last_token_len = 1;
			}

			if(state_posback[state]){
				//if(lex->pos)//if pos == 0, it means a new line, don't back
				lex->pos--;
			}
			return TRUE;
		}
	}
	//-----end of file
	if(state == IN_BLOCKCOMMENTS)
		PL0_print_error(ERR_MISS_END_FOR_COMMENT,lex->linebuf,lex->line,lex->pos);
	return FALSE;
}
