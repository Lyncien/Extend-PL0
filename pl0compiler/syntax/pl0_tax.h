#ifndef __PL0_TAX_H__
#define __PL0_TAX_H__

#define TXMAX				500 		// max length of token table
#define MAXADDRESS 	32767		// max address

/*variables for token table*/
int table_index;


/*struct for token table*/
typedef struct {
	char name[MAX_ID_LEN + 1];
	int kind;
	int value;
	short level;
	short address;
} Table;

/*definition for token table*/
Table token_table[TXMAX];

/*identifier type*/
enum idtype {
	ID_CONSTANT,
	ID_VARIABLE,
	ID_PROCEDURE
};


/*operations for token table*/
void table_append(int kind);



/*variables for parameter table*/
int table2_index;

/*struct for parameter table*/
typedef struct {
	char name[MAX_ID_LEN + 1];
} Table2;
Table2 parameter_table[TXMAX];

/*operations for parameter table*/
void table2_append();


/* gen code */
#define NONE -99999999
#define MAX_MEMORY_SIZE 1024
#define MAX_CODEHEAP_SIZE 1024

typedef struct _code{
	char ins[4];
	int para;
}code;
code codes[MAX_CODEHEAP_SIZE];
int codeaddr;

typedef enum REG{
	REG_AX = 1,
	REG_BX,
	REG_BP, //reg store: base
	REG_SP, //reg store: runstacktop
	REG_TMP //use as a trash
}REG;

void gen(char *ins, int para);
void gen_savereg(REG regid);
void gen_loadreg(REG regid);
void gen_runstack_push();
void gen_runstack_push_a_val(int val);
void gen_runstack_push_from_reg(REG regid);
void gen_runstack_pop();
void gen_runstack_pop_to_reg(REG regid);
void gen_exchangestacktop();
void gen_int2bool();
void gen_id_addr(int index);

/* set */
typedef long long set;
_Bool inset(set s);
set mergeset(set s1, set s2);
set t2s(PL0TokenType token);






/*functions for syntax analysis*/
void or_expression(set follows);//analysis the or_expression
void and_expression(set follows);//analysis the and_expression
void relation_expression(set follows); //analysis the relation_expression
void arithmetic_expression(set follows); //analysis the arithmetic_expression
void term(set follows); //analysis the term
void factor(set follows); //analysis the factor

void statement(set follows); //analysis the statement
void program_block(set follows); //analysis the program block
void program(PL0Lex *_lex);



void gettoken();
_Bool match(PL0TokenType token);
void check(set s1,set s2,char *errmsg);
void err(char *errmsg);
void printtreenode1();
void printtreenode2(char *s);


#endif //pl0_tax.h