%{
	#include "syntax.tab.h"
	#include <stdlib.h>
	#include <string.h>
	#include"syntax_tree.h"
	
	#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng - 1; \
	yycolumn += yyleng;
	
	extern int yycolumn;
%}
%option yylineno

LINECOMMENT "//"


BLOCKCOMMENT "/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" 

SEMI ;
RELOP \>|\<|">="|"<="|"=="|"!="
COMMA ,
ASSIGNOP =
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR "||"
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
INT 0|[1-9][0-9]*
FLOAT [-]?([0-9]*((\.?[0-9]+)|([0-9]+\.)))
ID [_a-zA-Z][_0-9a-zA-Z]*
SPACE " "+|\t+

EOL \n


%%

{LINECOMMENT} {
	char c = input();
	while(c!='\n') c = input();
	}
	
{BLOCKCOMMENT}

{SEMI} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("SEMI", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return SEMI;
	}
{RELOP} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("RELOP", yylineno, LEX_);
	yylval.type_leaf = leaf;
	//printf("RELOP = %s\n", yytext);
	return RELOP;
	}
{COMMA} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("COMMA", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return COMMA;
	}
{ASSIGNOP} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("ASSIGNOP", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return ASSIGNOP;
	}

{PLUS} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("PLUS", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return PLUS;
	}
{MINUS} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("MINUS", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return MINUS;
	}
{STAR} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("STAR", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return STAR;
	}
{DIV} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("DIV", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return DIV;
	}
{AND} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("AND", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return AND;
	}
{OR} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("OR", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return OR;
	}
{DOT} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("DOT", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return DOT;
	}
{NOT} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("NOT", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return NOT;
	}
	
{TYPE} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("TYPE", yylineno, TYPE_);
	strcpy(leaf->content, yytext);
	yylval.type_leaf = leaf;
	return TYPE;
	}
{LP} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("LP", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return LP;
	}
{RP} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("RP", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return RP;
	}
{LB} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("LB", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return LB;
	}
{RB} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("RB", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return RB;
	}
{LC} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("LC", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return LC;
	}
{RC} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("RC", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return RC;
	}
{STRUCT} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("STRUCT", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return STRUCT;
	}
{RETURN} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("RETURN", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return RETURN;
	}
{IF} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("IF", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return IF;
	}
{ELSE} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("ELSE", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return ELSE;
	}
{WHILE} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("WHILE", yylineno, LEX_);
	yylval.type_leaf = leaf;
	return WHILE;
	}
{INT} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("INT", yylineno, INT_);
	strcpy(leaf->content, yytext);
	yylval.type_leaf = leaf;
	return INT;
	}
{FLOAT} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("FLOAT", yylineno, FLOAT_);
	strcpy(leaf->content, yytext);
	yylval.type_leaf = leaf;
	return FLOAT;
	}
{ID} {
	Syntax_Leaf* leaf = Syntax_Leaf_New("ID", yylineno, ID_);
	strcpy(leaf->content, yytext);
	yylval.type_leaf = leaf;
	return ID;
	}

{SPACE}
{EOL} { yycolumn = 1;}
. {printf("Error type A at line %d: Mysterious characters : %s\n It is the %02x %02x %02x %02x %02x\n", yylineno, yytext,yytext[0],yytext[1],yytext[2],yytext[3],yytext[4]);}

%%
