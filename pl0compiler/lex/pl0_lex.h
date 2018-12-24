#ifndef __PL0_LEX_H__
#define __PL0_LEX_H__


#define NRW			15	/* Number of reserved words */
#define NSYM		19	/* Number of symbols */
#define MAX_NUM_LEN	9	/* Maximum length of a number, set to 9 to fit in range of C (-2147483648 to 2147483647) */
#define MAX_ID_LEN	10	/* Maximum length of an identifier */

#define N_STATE 37 //Number of state
#define N_STATE_NO_AC 13 //Number of state(no accept)
#define N_CHARTYPE 22 //Number of CharType

struct _tPL0Compiler;

typedef enum _tPL0TokenType {
	TOKEN_NULL,
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	TOKEN_SYMBOL,	/* Below are accepted symbols */
	TOKEN_PLUS,		/* + */
	TOKEN_MINUS,	/* - */
	TOKEN_TIMES,	/* * */
	TOKEN_SLASH,	/* / */
	TOKEN_EQU,  	/* = */
	TOKEN_NEQ,  	/* != */
	TOKEN_LES,  	/* < */
	TOKEN_LEQ,  	/* <= */
	TOKEN_GTR,  	/* > */
	TOKEN_GEQ,  	/* >= */
	TOKEN_LPAREN,	/* ( */
	TOKEN_RPAREN,	/* ) */
	TOKEN_COMMA,	/* , */
	TOKEN_SEMICOLON,/* ; */
	TOKEN_PERIOD,	/* . */
	TOKEN_BECOMES,	/* := */
	TOKEN_NOT,		/* ! */
	TOKEN_AND,		/* && */
	TOKEN_OR,		/* || */
	TOKEN_RESWORDS, /* Below are reserved words */
	TOKEN_VAR,
	TOKEN_CONST,
	TOKEN_PROCEDURE,
	TOKEN_BEGIN,
	TOKEN_END,
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_DO,
	TOKEN_WHILE,
	TOKEN_CALL,
	TOKEN_ODD,
	TOKEN_RETURN,
	TOKEN_ELSE,
	TOKEN_ELIF,
	TOKEN_PRINT
} PL0TokenType;

extern const char * TOKEN_RESERVED_WORDS[NRW];
extern const char * TOKEN_SYMBOLS[NSYM];

typedef enum _tCharType{
	LETTER, /* A-Za-z_ */
	DIGIT,  /* 0-9 */
	SPACE,  /* */
	TAB,	/* \t */
	NEWLINE,/* \n */
	PLUS,   /* + */
	MINUS,	/* - */
	TIMES,	/* * */
	SLASH,	/* / */
	EQU,	/* = */
	LES,	/* < */
	GTR,	/* > */
	LPAREN,	/* ( */
	RPAREN,	/* ) */
	COMMA,	/* , */
	SEMICOLON,/* ; */
	PERIOD,	/* . */
	COLON,	/* : */
	EXCLAMA,/* ! */
	AND,	/* & */
	OR,		/* | */
	ILLEGAL	/* other */
} CharType;


typedef enum _tState{
	START,
	SLASH_SEL,
	LES_SEL,
	GTR_SEL,
	IN_NUM,
	IN_ID,
	IN_LINECOMMENTS,
	IN_BLOCKCOMMENTS,
	TO_END_BLOCKCOMMENTS,
	COLON_SEL,
	EXCLAMA_SEL,
	AND_SEL,
	OR_SEL,
	PLUS_AC,
	MINUS_AC,
	TIMES_AC,
	SLASH_AC,
	EQU_AC,
	NEQ_AC,
	LES_AC,
	LEQ_AC,
	GTR_AC,
	GEQ_AC,
	LPAREN_AC,
	RPAREN_AC,
	COMMA_AC,
	SEMICOLON_AC,
	PERIOD_AC,
	BECOMES_AC,
	NOT_AC,
	AND_AC,
	OR_AC,
	NUM_AC,
	ID_AC,
	ERROR1,// ":" expect "="
	ERROR2,//expect "&&" but find "&"
	ERROR3	//expect "||" but find "|"
} State;


typedef struct _tPL0Lex {
	/* Parent return pointer */
	struct _tPL0Compiler * compiler;
	/* For output */
	PL0TokenType last_token_type;
	char last_id[MAX_ID_LEN + 1];
	int last_num;
	int last_level;

	/** --------------------------
	 * TODO: Your variables here
	*/
	int line;
	int pos;
	char linebuf[256];
	int last_token_len;

} PL0Lex;


PL0Lex * PL0Lex_create(struct _tPL0Compiler * parent);
BOOL PL0Lex_destroy(PL0Lex * lex);

BOOL PL0Lex_get_token(PL0Lex * lex);

#endif /* pl0_lex.h */
