#ifndef __PL0_ERROR_H__
#define __PL0_ERROR_H__
#include <stdio.h>

//Possible errors in lex
#define ERR_UNRECOGNIZED_CHAR "An unrecognized character appears."
#define ERR_WRONG_IDEN_FORMAT "Wrong identifier format: identifier cannot start with a number."
#define ERR_MISS_END_FOR_COMMENT "Missing a end for comment: has reached the EOF."
#define ERR_TOO_LONG_IDEN_OR_NUM "Too long identifier or number."
#define ERR_COLON_EXPECT_EQUAL "Expect an '=' after ':'"
#define ERR_EXCLAMATION_EXPECT_EQUAL "Expect an '=' after '!'"
#define ERR_EXPECT_AND "Expect an '&&' but find '&'"
#define ERR_EXPECT_OR "Expect an '||' but find '|'"

//Possible errors in syntax
#define ERR_SEMICOLON_MISS "Missing ';'."
#define ERR_DOT_MISS "Missing '.'."
#define ERR_COMMA_MISS "Missing ','."
#define ERR_RIGHT_PARENTHESIS_MISS "Right parenthesis Missing')'."
#define ERR_SEMICOLON_OR_COMMA_MISS "Missing ';' or ','."
#define ERR_CONST_EXPECT_IDEN "Expect an identifier after const."
#define ERR_VAR_EXPECT_IDEN "Expect an identifier after var."
#define ERR_PROC_EXPECT_IDEN "Expect an identifier after procedure."
#define ERR_CALL_EXPECT_IDEN "Expect an identifier after call."
#define ERR_IDEN_EXPECT_EQUAL "Expect an '=' after identifier."
#define ERR_ASSIGN_EXPECT_IDEN "Expect an ':=' after identifier."
#define ERR_ILLEGAL_ASSIGNMENT "Only a number can be assigned to '='"
#define ERR_NEED_END_FOR_BEGIN "'begin' needs to end with 'end'."
#define ERR_NEED_THEN_FOR_IF "'if' needs to end with 'then'."
#define ERR_NEED_DO_FOR_WHILE "Need a 'do' for the loop statement after the condition."
#define ERR_NEED_WHILE_FOR_DO "need a 'while' for the loop statement after the condition"
#define ERR_WRONG_OP_IN_BINARY_COND "Wrong op in a binary conditional expression: could only use '=', '!=', '<', '>', '<=', '>='."
#define ERR_WRONG_BEGIN_OF_PROC "Wrong beginning of procedure: could only use 'const', 'var', 'procdure', 'ident', 'call', 'begin', 'if', 'while', 'do', 'return'."
#define ERR_WRONG_BEGIN_OF_STATEMENT "Wrong beginning of statement: could only use 'ident', 'call', 'begin', 'if', 'while', 'do', 'if', 'return', 'print'."
#define ERR_WRONG_BEGIN_OF_COND "Wrong beginning of condition: could only use 'odd', '=', '!=', '<', '>', '<=', '>='."
#define ERR_WRONG_BEGIN_OF_FACTOR "Wrong beginning of factor: could only use 'iden', 'number', '-', '(', '!', 'odd'."
#define ERR_UD_IDEN "Undeclared identifier."
#define ERR_NUM_TOO_GREAT "The number is too great!"
#define ERR_EXPR_INCLUDE_PROC_ID "Procedure identifier can not be in an expression."
#define ERR_FACTOR_FOLLOW "The symbol can not be followed by a factor."
#define ERR_STATEMENT_FOLLOW "The symbol can not be followed by a statement."
#define ERR_ILLEGAL_CALL "The const or var can't be called."

void PL0_print_error(char * err_msg, char* cur_line_content, int cur_line_num, int offset);

#endif
