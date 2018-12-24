#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "../lex/pl0_lex.h"
#include "pl0_tax.h"
#include "../error/pl0_error.h"
PL0Lex * lex;

int level = 0;//for token
int tree_level = 0;//for syntax tree

int pro_flag = 0;//is in procedure
int idcount = 0;//global id number


//----------------------------------------------------------------

set TokenPower[39]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,\
						0x1000,0x2000,0x4000,0x8000,0x10000,0x20000,0x40000,0x80000,\
						0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,\
						0x10000000,0x20000000,0x40000000,0x80000000,0x100000000,0x200000000,0x400000000,0x800000000,\
						0x1000000000,0x2000000000,0x4000000000
						};
_Bool inset(set s){
	return s & TokenPower[lex->last_token_type];
}
set mergeset(set s1, set s2){
	return s1 | s2;
}
set t2s(PL0TokenType token){ //token to a set elem
	return TokenPower[token];
}
						
set phi = 0x0;
set first_block,first_declaration,first_statement,first_factor; //first
set relation_op;


void gettoken(){
	printtreenode1();//print last token
	PL0Lex_get_token(lex);
}
_Bool match(PL0TokenType token){
	return lex->last_token_type == token;
}

void check(set s1,set s2,char *errmsg){
	set s;
	if (!inset(s1)){
		err(errmsg);
		s = mergeset(s1, s2);
		while(!inset(s))
			gettoken();
	}
} 
void err(char *errmsg){
	PL0_print_error(errmsg, lex->linebuf, lex->line, lex->pos);
}
//----------------------------------------------------------------
void printtreenode1(){
/*	int i;
	for(i = 0; i < tree_level - 1; i++)
		printf("         │");
	if(tree_level > 0) printf("         ├");	
	if(match(TOKEN_IDENTIFIER)){
		printf("%10s\n", lex->last_id);
	}
	else if(match(TOKEN_NUMBER)){
		printf("%10d\n", lex->last_num);
	}
	else if(lex->last_token_type > TOKEN_RESWORDS){
		printf("%10s\n", TOKEN_RESERVED_WORDS[lex->last_token_type - TOKEN_RESWORDS - 1]);
	}
	else {
		printf("%10s\n", TOKEN_SYMBOLS[lex->last_token_type - TOKEN_SYMBOL - 1]);
	}*/
}
void printtreenode2(char *s){
/*	int i;
	for(i = 0; i < tree_level - 1; i++)
		printf("         │");
	if(tree_level > 0) printf("         ├");
	printf("%10s\n", s);*/
}

//----------------------------------------------------------------
//PL0 virtual machine: 1. 'stack'(just for calculate here)  2. 'memory' 3. 'codeheap'
//'runstack' is based on 'memory', for the function calling

/*
--------------men[0]
|null
|runstack
|..
|..			<-- runstack's top(REG_SP)
|
|reg4 //top
|reg3 //base 
|reg2
|reg1
--------------men[1023]


a record in runstack (running)
--------------low address  
|return value
|return address(in codeheap)
|para
|old base					<---base (REG_BP) , +offset to get local id , -offset to get parameter
|local id(var/const/procedure)					
--------------high address  <---top (REG_SP)

*/



void gen(char *ins, int para){
	strcpy(codes[codeaddr].ins, ins);
	codes[codeaddr++].para = para;
}

void gen_savereg(REG regid){//save stack[top] to a reg value,regid >= 1
	gen("ST",MAX_MEMORY_SIZE - regid);
}
void gen_loadreg(REG regid){//load a reg value to stack[top],regid >= 1
	gen("LD",MAX_MEMORY_SIZE - regid);
}

void gen_runstack_push(){//stack[top] to runstack[++runstacktop]
	gen_loadreg(REG_SP);
	gen("PUT",1);
	gen("ADD",NONE);
	gen_savereg(REG_SP);
	//runstacktop++
	gen_loadreg(REG_SP);
	gen("ST",NONE);
	//runstack[runstacktop] = stack[top]
}
void gen_runstack_push_a_val(int val){//val to runstack[++runstacktop]
	gen_loadreg(REG_SP);
	gen("PUT",1);
	gen("ADD",NONE);
	gen_savereg(REG_SP);
	//runstacktop++
	gen("PUT",val);
	gen_loadreg(REG_SP);
	gen("ST",NONE);
	//runstack[runstacktop] = val
}
void gen_runstack_push_from_reg(REG regid){//reg[i] to runstack[++runstacktop]
	gen_loadreg(REG_SP);
	gen("PUT",1);
	gen("ADD",NONE);
	gen_savereg(REG_SP);
	//runstacktop++
	gen_loadreg(regid);
	gen_loadreg(REG_SP);
	gen("ST",NONE);
	//runstack[runstacktop] = reg[i]
}

void gen_runstack_pop(){
	gen_loadreg(REG_SP);
	gen("PUT",1);
	gen("SUB",NONE);
	gen("PUT",0);
	gen("SUB",NONE);
	gen_savereg(REG_SP);
	//runstacktop--;
}
void gen_runstack_pop_to_reg(REG regid){//runstack[top--] to reg[i]
	gen_loadreg(REG_SP);
	gen("LD",NONE);
	gen_savereg(regid);
	//reg[i] = runstack[runstacktop]
	gen_loadreg(REG_SP);
	gen("PUT",1);
	gen("SUB",NONE);
	gen("PUT",0);
	gen("SUB",NONE);
	gen_savereg(REG_SP);
	//runstacktop--;
}



void gen_exchangestacktop(){ //stack[top]<->stack[top-1]
	gen_savereg(REG_AX);
	gen_savereg(REG_BX);
	gen_loadreg(REG_AX);
	gen_loadreg(REG_BX);	
}
void gen_int2bool(){ //stack[top]
	gen("PUT",0);
	gen("EQ",NONE); 
	gen("NOT",NONE); //translate any value on stack[top] to 0/1
}
void gen_id_addr(int index)//index is given from intable() ,index : >0 is a var/const/procedure id; <0 is a parameter;
{
	if(index > idcount){//loacl id
		gen_loadreg(REG_BP);//load base
		gen("PUT",index - idcount);//id addr: base + index - idcount
		gen("ADD",NONE);
	}
	else if(index > 0){ //global id
		gen("PUT",index);//id addr: base + index 
	}
	else if(index < 0){ //parameter
		gen_loadreg(REG_BP);
		gen("PUT",index);//id addr: base + index = base - |index|
		gen("ADD",NONE);
	}
}
//----------------------------------------------------------------
/*operations for token table*/
void table_append(int kind) {
	if(!pro_flag)
		idcount++;
	strcpy(token_table[table_index].name, lex->last_id);
	token_table[table_index].kind = kind;
	switch (kind) {
		case ID_CONSTANT:
			if (lex->last_num > MAXADDRESS) {
				err(ERR_NUM_TOO_GREAT);
				lex->last_num = 0;
			}
			token_table[table_index].value = lex->last_num;
			token_table[table_index].level = level;
			gen_runstack_push_a_val(lex->last_num);
			break;
		case ID_VARIABLE:
			token_table[table_index].level = level;
			gen_runstack_push_a_val(0);
			break;
		case ID_PROCEDURE:
			token_table[table_index].level = level;
			token_table[table_index].address = 0;
			gen_runstack_push_a_val(0);//0 will be replace by the true address of the start of procedure later
			break;
	} //switch
	table_index += 1;
} //table_append


/*operations for parameter table*/
void table2_append() {
	strcpy(parameter_table[table2_index].name, lex->last_id);
	table2_index += 1;
}


int intable(char *idname){//return a offset.
//0 not found; >0 is a var/const/procedure id; <0 is a parameter;
	int i;
	for(i = table_index - 1;i >= 1;i--){
		if(strcmp(token_table[i].name,idname) == 0)
			return i;
	}
	for(i = table2_index - 1;i >= 1;i--){
		if(strcmp(parameter_table[i].name,idname) == 0)
			return -i;//-i
	}
	return 0;
}

//----------------------------------------------------------------
void or_expression(set follows){
	printtreenode2("OR_EXPR");
	tree_level++;
	
	set follows_and_expr = follows | t2s(TOKEN_OR);
	and_expression(follows_and_expr); //only a expr, do not need translate to bool
	while(match(TOKEN_OR)){
		//expr1 or expr2
		gen_int2bool();//translate expr1 in top to bool
		
		gen("PUT",1);
		gen("EQ",NONE);
		int cd1 = codeaddr;
		gen("JPC",NONE);//short circuit, expr1 is true(1)
		gen("PUT",0);//short circuit,jpc will remove the stack[top] EVEN IF NOT jump,so PUT 0

		gettoken();
		and_expression(follows_and_expr);
		gen_int2bool();
		gen("OR",NONE);

		int cd2 = codeaddr;
		gen("JMP",NONE);
		codes[cd1].para = codeaddr;
		gen("PUT",1);//short circuit
		codes[cd2].para = codeaddr;
	}
	
	tree_level--;
}
void and_expression(set follows){
	printtreenode2("AND_EXPR");
	tree_level++;
	
	set follows_rel_expr = follows | t2s(TOKEN_AND);
	relation_expression(follows_rel_expr);//only a expr, do not need translate to bool
	while(match(TOKEN_AND)){
		//expr1 and expr2
		gen_int2bool();

		gen("PUT",0);
		gen("EQ",NONE);
		int cd1 = codeaddr;
		gen("JPC",NONE);//short circuit,expr1 is false(0)
		gen("PUT",1);//short circuit,jpc will remove the stack[top] EVEN IF NOT jump,so PUT 1

		gettoken();
		relation_expression(follows_rel_expr);
		gen_int2bool();
		gen("AND",NONE);

		int cd2 = codeaddr;
		gen("JMP",NONE);
		codes[cd1].para = codeaddr;
		gen("PUT",0);//short circuit
		codes[cd2].para = codeaddr;
	}	
	tree_level--;
}
void relation_expression(set follows) {
	printtreenode2("REL_EXPR");
	tree_level++;
	
	set follows_ari_expr = follows | relation_op;
	arithmetic_expression(follows_ari_expr);
	if(inset(relation_op)){
		int OP = lex->last_token_type;
		gettoken();
		arithmetic_expression(follows_ari_expr);
		gen_exchangestacktop();//exchange two op num because the instr of virtual machine is reversed order
		switch(OP){
			case TOKEN_LEQ:{gen("LE",NONE);break;} 
			case TOKEN_LES:{gen("LT",NONE);break;} 
			case TOKEN_GEQ:{gen("GE",NONE);break;} 
			case TOKEN_GTR:{gen("GT",NONE);break;} 
			case TOKEN_EQU:{gen("EQ",NONE);break;} 
			case TOKEN_NEQ:{gen("EQ",NONE);gen("NOT",NONE);break;}
		}
	}

	tree_level--;
}

void arithmetic_expression(set follows) {
	printtreenode2("ARITH_EXPR");
	tree_level++;

	set follows_term = follows | t2s(TOKEN_PLUS) | t2s(TOKEN_MINUS);
	term(follows_term);
	while(match(TOKEN_PLUS) || match(TOKEN_MINUS)){
		int OP = lex->last_token_type;
		gettoken();
		term(follows_term);
		if(OP == TOKEN_PLUS) gen("ADD",NONE);
		else{
			gen("SUB",NONE);
			gen("PUT",0);
			gen("SUB",NONE);//reverse
		} 
	}
	
	tree_level--;
}

void term(set follows) {
	printtreenode2("TERM");	
	tree_level++;
	
	set follows_factor = follows | t2s(TOKEN_TIMES) | t2s(TOKEN_SLASH);
	factor(follows_factor);
	
	while(match(TOKEN_TIMES) || match(TOKEN_SLASH))
	{
		int OP = lex->last_token_type;
		gettoken();
		factor(follows_factor);
		gen_exchangestacktop();
		if(OP == TOKEN_TIMES) gen("MUL",NONE);
		else gen("DIV",NONE);
	}
	
	tree_level--;
}

void factor(set follows) {
	printtreenode2("FACTOR");	
	tree_level++;
	//the possible or_expression first token will be detected in the factor function
	check(first_factor,follows,ERR_WRONG_BEGIN_OF_FACTOR);
	if(match(TOKEN_IDENTIFIER)){
		int index = intable(lex->last_id);
		if(index > 0){
			if(token_table[index].kind == ID_VARIABLE){
				gen_id_addr(index);
				gen("LD",NONE);
			}
			else if(token_table[index].kind == ID_CONSTANT){
				gen_id_addr(index);
				gen("LD",NONE);
			}
			else{
				err(ERR_EXPR_INCLUDE_PROC_ID);//Procedure identifier can not be in an expression
			}
		}
		else if(index < 0){//is parameter
			gen_id_addr(index);
			gen("LD",NONE);
		}
		else{
			err(ERR_UD_IDEN);//Undeclared identifier
		}
		gettoken();
	}
	else if(match(TOKEN_NUMBER)){
		gen("PUT",lex->last_num);
		gettoken();
	}
	else if(match(TOKEN_MINUS)){
		gettoken();
		factor(follows);
		gen("PUT",0);
		gen("SUB",NONE);
	}
	else if(match(TOKEN_NOT)){
		gettoken();
		factor(follows);
		gen_int2bool();
		gen("NOT",NONE);
	}
	else if(match(TOKEN_ODD)){
		gettoken();
		factor(follows);
	}
	else if(match(TOKEN_LPAREN)){
		gettoken();
		set follows_or_expr = follows | t2s(TOKEN_RPAREN);
		or_expression(follows_or_expr);
		if(match(TOKEN_RPAREN)){
			gettoken();
		}
		else{
			err(ERR_RIGHT_PARENTHESIS_MISS);//Missing ')'.
		}
	}
	else if(match(TOKEN_CALL)){
		set follows_or_expr;
		gettoken();
		if(match(TOKEN_IDENTIFIER))
		{
			int index = intable(lex->last_id);
			if(index <= 0){
				err(ERR_UD_IDEN);
				gettoken();		
			}
			else if (token_table[index].kind == ID_PROCEDURE)
			{
				int paracount = 0;
				gettoken();
				if (match(TOKEN_LPAREN))
				{
					gettoken();
					follows_or_expr=t2s(TOKEN_COMMA)|t2s(TOKEN_RPAREN)|follows;
					or_expression(follows_or_expr);
					gen_runstack_push();
					paracount++;
					while(match(TOKEN_COMMA))
					{
						gettoken();
						or_expression(t2s(TOKEN_COMMA)|t2s(TOKEN_RPAREN)|follows);
						gen_runstack_push();
						paracount++;
					}
					if(match(TOKEN_RPAREN))
						gettoken();
					else
						err(ERR_RIGHT_PARENTHESIS_MISS);
				}
				gen_runstack_push_from_reg(REG_BP);//save runstackbase value to runstack[++runstacktop]
				gen_loadreg(REG_SP);//load runstacktop value 
				gen_savereg(REG_BP);//new base = runstacktop

				gen("PUT",codeaddr + 3 + (index > idcount ? 3 : 1));//the return address , +3 : to skip the 3 ins (PUT LD JMP) ,
				gen_id_addr(index);
				gen("LD",NONE);//load procedure start address at codeheap
				gen("JMP",NONE);

				gen_loadreg(REG_BP);//<---procedure will return here, 
				gen_savereg(REG_SP); //recover runstacktop
				gen_runstack_pop_to_reg(REG_BP);//recover runstackbase
				while(paracount--)
					gen_runstack_pop();//throw the parameters in runstack
			}
			else
			{
				err(ERR_ILLEGAL_CALL);
				gettoken();

			}			
		}
		else{
			err(ERR_CALL_EXPECT_IDEN);//There must be an identifier to follow 'call'.
		}
	}
	check(follows,phi,ERR_FACTOR_FOLLOW);//The symbol can not be followed by a factor.

	tree_level--;
}





/*functions for syntax analysis*/
void statement(set follows) {

	printtreenode2("STATEMENT");
	tree_level++;
	set follows_or_expr;
	set follows_state;
	check(first_statement,follows,ERR_WRONG_BEGIN_OF_STATEMENT);
	//printf("analysis the statement\n");
	if(match(TOKEN_IDENTIFIER)){
		int index = intable(lex->last_id);
		if(index == 0)
		{
			err(ERR_UD_IDEN);
		}
		else if (index < 0){//parameter

		} 
		else if (index > 0)
		{
			if(token_table[index].kind!=ID_VARIABLE) 
				err(ERR_ILLEGAL_ASSIGNMENT);
		}
		gettoken();
		if(match(TOKEN_BECOMES))
		{
			gettoken();
		}
		else
		{
			err(ERR_ASSIGN_EXPECT_IDEN);
		}
		follows_or_expr=follows;
		or_expression(follows_or_expr);
		gen_id_addr(index);
		gen("ST",NONE);
	}
	else if(match(TOKEN_CALL)){
		gettoken();
		if(match(TOKEN_IDENTIFIER))
		{
			int index = intable(lex->last_id);
			if(index <= 0){
				err(ERR_UD_IDEN);
				gettoken();		
			}
			else if (token_table[index].kind == ID_PROCEDURE)
			{
				int paracount = 0;//to pop the parameter;
				gettoken();
				if (match(TOKEN_LPAREN))
				{
					gettoken();
					follows_or_expr=t2s(TOKEN_COMMA)|t2s(TOKEN_RPAREN)|follows;
					or_expression(follows_or_expr);
					gen_runstack_push(); //runstack: push the parameter for calling function
					paracount++;
					while(match(TOKEN_COMMA))
					{
						gettoken();
						or_expression(t2s(TOKEN_COMMA)|t2s(TOKEN_RPAREN)|follows);
						gen_runstack_push();//runstack: push the parameter for calling function
						paracount++;
					}
					if(match(TOKEN_RPAREN))
						gettoken();
					else
						err(ERR_RIGHT_PARENTHESIS_MISS);
				}
				gen_runstack_push_from_reg(REG_BP);//save runstackbase value to runstack[++runstacktop]
				gen_loadreg(REG_SP);//load runstacktop value 
				gen_savereg(REG_BP);//new base = runstacktop

				gen("PUT",codeaddr + 3 + (index > idcount ? 3 : 1));//the return address , +3 : to skip the 3 ins (PUT LD JMP) ,
				gen_id_addr(index);
				gen("LD",NONE);//load procedure start address at codeheap
				gen("JMP",NONE);

				gen_loadreg(REG_BP);//<---procedure will return here, 
				gen_savereg(REG_SP); //recover runstacktop
				gen_runstack_pop_to_reg(REG_BP);//recover runstackbase
				while(paracount--)
					gen_runstack_pop();//throw the parameters in runstack
			}
			else
			{
				err(ERR_ILLEGAL_CALL);
				gettoken();

			}			
		}
		else{
			err(ERR_CALL_EXPECT_IDEN);//There must be an identifier to follow 'call'.
		}
	}
	else if(match(TOKEN_BEGIN)){
		gettoken();
		follows_state=follows|t2s(TOKEN_SEMICOLON)|t2s(TOKEN_END);
		statement(follows_state);
		while(match(TOKEN_SEMICOLON)|| inset(first_statement))
		{
			if(match(TOKEN_SEMICOLON))
			{
				gettoken();
				if (match(TOKEN_END))
					break;
			}
			else
			{
				err(ERR_SEMICOLON_MISS);
			}
			statement(follows_state);
		}
		if(match(TOKEN_END)){
			gettoken();
		}
		else{
			err(ERR_NEED_END_FOR_BEGIN);//'end' expected.
		}
	}
	else if(match(TOKEN_IF)){
		gettoken();
		follows_or_expr=follows|t2s(TOKEN_THEN);
		or_expression(follows_or_expr);
		if(match(TOKEN_THEN)){
			gen_int2bool();
			gen("NOT",NONE);
			int cd = codeaddr;
			gen("JPC",NONE);
			gettoken();
			follows_state=follows|t2s(TOKEN_ELIF)|t2s(TOKEN_ELSE);
			statement(follows_state);
			codes[cd].para = codeaddr;
		}
		else{
			err(ERR_NEED_THEN_FOR_IF);//'then' expected.
		}
		while(match(TOKEN_ELIF))
		{	
			gettoken();
			or_expression(follows_or_expr);
			if(match(TOKEN_THEN))
			{
				gen_int2bool();
				gen("NOT",NONE);
				int cd = codeaddr;
				gen("JPC",NONE);
				gettoken();
				follows_state=follows|t2s(TOKEN_ELIF)|t2s(TOKEN_ELSE);
				statement(follows_state);
				codes[cd].para = codeaddr;
			}
			else
			{
				err(ERR_NEED_THEN_FOR_IF);//'then' expected.
			}
		}
		if(match(TOKEN_ELSE))
		{
			gettoken();
			statement(follows);
		}
	}
	else if(match(TOKEN_WHILE)){
		int cd1 = codeaddr;
		gettoken();
		follows_or_expr=follows|t2s(TOKEN_DO);
		or_expression(follows_or_expr);
		if(match(TOKEN_DO)){
			gettoken();
		}
		else{
			err(ERR_NEED_DO_FOR_WHILE);//'do' expected.
		}
		gen_int2bool();	
		gen("NOT",NONE);
		int cd2 = codeaddr;		
		gen("JPC",NONE);
		statement(follows);
		gen("JMP",cd1);
		codes[cd2].para = codeaddr;
	}
	else if (match(TOKEN_DO))
	{
		int cd = codeaddr;
		gettoken();
		follows_or_expr=follows;
		follows_state=follows|t2s(TOKEN_WHILE);
		statement(follows);
		if(match(TOKEN_WHILE)){
			gettoken();
		}
		else{
			err(ERR_NEED_WHILE_FOR_DO);//'do' expected.
		}		
		or_expression(follows_or_expr);
		gen("JPC",cd);
	}
	else if (match(TOKEN_RETURN))
	{
		gettoken();
		or_expression(follows);
		gen_exchangestacktop();//exchange return value and return address
		gen("JMP",NONE);
	}
	else if(match(TOKEN_PRINT))
	{
		gettoken();
		or_expression(follows);
		gen("INT",0);
		gen_savereg(REG_TMP);//pop the top after print it;
	}
	tree_level--;
	check(follows,phi,ERR_STATEMENT_FOLLOW);
}

void const_declaration(set follows) {
	//printf("analysis the const declaration\n");
	printtreenode2("CONST");		
	tree_level++;
	if (match(TOKEN_IDENTIFIER)) {
		gettoken();
		if (match(TOKEN_EQU) || match(TOKEN_BECOMES)) {
			if (match(TOKEN_BECOMES))
				//printf("found ':=' when expecting '='\n");
				err(ERR_IDEN_EXPECT_EQUAL);
			gettoken();
			int flag = 0;
			if(match(TOKEN_MINUS))
			{
				flag = 1;
				gettoken();
			}
			if (match(TOKEN_NUMBER)) {
				if(flag == 1)
					lex->last_num = - lex->last_num;
				table_append(ID_CONSTANT);
				gettoken();
			} else {
				//printf("There must be a number to follow '='\n");
				err(ERR_ILLEGAL_ASSIGNMENT);
			}
		} else {
			//printf("There must be an '=' to follow the identifier\n");
			err(ERR_IDEN_EXPECT_EQUAL);
		}
	} else {
		//printf("There must be an identifier to follow 'const'\n");
		err(ERR_CONST_EXPECT_IDEN);
	}

	tree_level--;
} //const_declaration

void var_declaration(){
	//printf("analysis the var declaration\n");
	printtreenode2("VAR");		
	tree_level++;
	if (match(TOKEN_IDENTIFIER)) {
		table_append( ID_VARIABLE);
		gettoken();
	} else {
		//printf("There must be an identifier to follow 'var'\n");
		err(ERR_VAR_EXPECT_IDEN);
	}
	tree_level--;
}

void program_block(set follows) {
	//printf("analysis the program block\n");
	set state_follow,block_follow;
	printtreenode2("BLOCK");		
	tree_level++;

	int old_table_index = table_index;
	int old_table2_index = table_index;
	check(first_block,follows,ERR_WRONG_BEGIN_OF_PROC);
	do {
		if(match(TOKEN_CONST)) {
			gettoken();
			do {
				const_declaration(follows);
				while (match(TOKEN_COMMA)) {
					gettoken();
					const_declaration(follows);
				}
				if (match(TOKEN_SEMICOLON)) {
					gettoken();
				}
				else {
					err(ERR_SEMICOLON_OR_COMMA_MISS);
				}
			} while (match(TOKEN_IDENTIFIER));
			
		}
		else if (match(TOKEN_VAR)){
			gettoken();
			do {
				var_declaration();
				while (match(TOKEN_COMMA)) {
					gettoken();
					var_declaration();
				}
				if (match(TOKEN_SEMICOLON)) {
					gettoken();
				}
				else {
					err(ERR_SEMICOLON_OR_COMMA_MISS);
					
				}
			} while (match(TOKEN_IDENTIFIER));	
		
		}
		else if (match(TOKEN_PROCEDURE))
		{
			int cd0,cd1;
			
			gettoken();
			if (match(TOKEN_IDENTIFIER)) {
				table_append(ID_PROCEDURE);
				cd0 = codeaddr - 3;
				//the instr that "PUT val"(val=the start addr of the procedure) in table_append() in gen_runstack_push_val()
				//val(the start addr of the procedure) will be ensure after scan the parameter
				gettoken();
				
			}
			else{
				err(ERR_PROC_EXPECT_IDEN);//There must be an identifier to follow 'proc'
			}
			pro_flag=1;
			
			if (match(TOKEN_LPAREN))
			{
					gettoken();
					if(match(TOKEN_IDENTIFIER))
					{
						table2_append();
						gettoken();
					}
					else
						err("ERR_PROC_PARA_EXPECT_IDEN");
					while(match(TOKEN_COMMA))
					{
						gettoken();
						if(match(TOKEN_IDENTIFIER))
						{
							table2_append();
							gettoken();
						}
						else
							err("ERR_PROC_PARA_EXPECT_IDEN");
					}
					if(match(TOKEN_RPAREN))
						gettoken();
					else
						err(ERR_RIGHT_PARENTHESIS_MISS);
			}
			
			cd1 = codeaddr;
			gen("JMP",NONE);//to skip the procedure

			codes[cd0].para = codeaddr;//the codeaddr now is the start addr of procedure
			if (match(TOKEN_SEMICOLON)) {
				gettoken();
				level++;
				
				block_follow = mergeset(follows,t2s(TOKEN_SEMICOLON));
				program_block(block_follow);
				gen("JMP",NONE);
				level--;
				if (match(TOKEN_SEMICOLON)){
					gettoken();
				}
				else{
					err(ERR_SEMICOLON_MISS);//';' expected.
				}
			}
			else{
				err(ERR_SEMICOLON_MISS);//';' expected.
				block_follow = mergeset(follows,t2s(TOKEN_SEMICOLON));
				program_block(block_follow);
				level--;
				if (match(TOKEN_SEMICOLON)){
					gettoken();
				}
				else{
					err(ERR_SEMICOLON_MISS);//';' expected.
				}
			}
			pro_flag=0;
			codes[cd1].para = codeaddr;
		}
		check(first_block,follows,"need_statement_or_declaration_or_proc");
	} while(match(TOKEN_CONST) || match(TOKEN_VAR) || match(TOKEN_PROCEDURE));
	state_follow=follows;
	statement(state_follow);

	while(table_index > old_table_index) 
		table_index--;
	while(table2_index > old_table2_index) 
		table2_index--;

	tree_level--;
} //program_block

void program(PL0Lex *_lex){
	lex = _lex;
	codeaddr = 0;
	table_index = 1;
	table2_index = 1;

	relation_op = t2s(TOKEN_LES) | t2s(TOKEN_LEQ) | t2s(TOKEN_GTR) | t2s(TOKEN_GEQ) | t2s(TOKEN_EQU) | t2s(TOKEN_NEQ);
	first_declaration = t2s(TOKEN_CONST) | t2s(TOKEN_VAR) | t2s(TOKEN_PROCEDURE);
	first_statement = t2s(TOKEN_IF) | t2s(TOKEN_WHILE) | t2s(TOKEN_DO) | t2s(TOKEN_CALL) | t2s(TOKEN_BEGIN) | t2s(TOKEN_RETURN) | t2s(TOKEN_IDENTIFIER) | t2s(TOKEN_PRINT);
	first_block = mergeset(first_declaration, first_statement);
	first_factor = t2s(TOKEN_IDENTIFIER) | t2s(TOKEN_NUMBER) | t2s(TOKEN_MINUS) | t2s(TOKEN_NOT) | t2s(TOKEN_LPAREN) | t2s(TOKEN_ODD)|t2s(TOKEN_CALL);
	
	printtreenode2("PROGRAM");
	tree_level++;
	

	gen("PUT",0);
	gen_savereg(REG_SP);
	gen("PUT",0);
	gen_savereg(REG_BP);
	program_block(first_block|t2s(TOKEN_PERIOD)|t2s(TOKEN_NULL));
	
	if(!match(TOKEN_PERIOD))
		err(ERR_DOT_MISS);//ERR_DOT_MISS
	
	gen("INT",1);
	tree_level--;

	
	for(int i = 0; i < codeaddr; i++){
		if(codes[i].para == NONE)
			fprintf(fout,"%s\n",codes[i].ins);
		else
			fprintf(fout,"%s %d\n",codes[i].ins,codes[i].para);
	}

}