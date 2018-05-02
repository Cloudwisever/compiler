%{
	#include "lex.yy.c"
	#include <stdlib.h>
	#include"syntax_tree.h"
	
	int yycolumn = 1;
	
	extern Syntax_Leaf* Syntax_Root;
	
	void yyerror(const char*str)
	{
		tree_error = 1;
		fprintf(stderr, "Error type B at Line: %d: %s\n", yylineno, str);
	}
%}

%union{
	struct Syntax_Leaf* type_leaf;
	struct Error_Node* type_error;
}
%locations
%error-verbose

%token <type_leaf> SEMI
%token <type_leaf> COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT 
%token <type_leaf> TYPE
%token <type_leaf> LP RP LB RB LC RC
%token <type_leaf> STRUCT RETURN IF ELSE WHILE
%token <type_leaf> ID
%token <type_leaf> INT
%token <type_leaf> FLOAT

%type <type_leaf> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
Program : ExtDefList {
	Syntax_Leaf* leaf = Syntax_Leaf_New("Program", @$.first_line, 0);
	leaf->children[leaf->childrennum++] = $1;
	$$ = leaf;
	Syntax_Root = leaf;
	}
 ;
ExtDefList : ExtDef ExtDefList  {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDefList", @$.first_line, 0);
 	if($1 != NULL)
 	{
 		$1->father = leaf;
 		leaf->children[leaf->childrennum++] = $1;
 	}
 	if($2 != NULL)
 	{
 		$2->father = leaf;
 		leaf->children[leaf->childrennum++] = $2;
 	}
 	$$ = leaf;
 	
 	//$1->father = leaf;
 	//$2->father = leaf;
 	}
 |{$$ = NULL;}
 ; 
ExtDef : Specifier ExtDecList SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDef", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Specifier SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDef", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 | Specifier FunDec CompSt {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDef", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Specifier FunDec SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDef", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;
ExtDecList : VarDec {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDecList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | VarDec COMMA ExtDecList {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ExtDecList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;

Specifier : TYPE{
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Specifier", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | StructSpecifier {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Specifier", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;
StructSpecifier : STRUCT OptTag LC DefList RC {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("StructSpecifier", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	leaf->children[leaf->childrennum++] = $5;
 	$$ = leaf;
 	}
 | STRUCT Tag {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("StructSpecifier", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 ;
OptTag : {$$ = NULL;}
 | ID {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("OptTag", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;
Tag : ID {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Tag", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;

VarDec : ID {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("VarDec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | VarDec LB INT RB {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("VarDec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	$$ = leaf;
 	}
 ;
FunDec : ID LP VarList RP {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("FunDec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	$$ = leaf;
 	}
 | ID LP RP {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("FunDec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;
VarList : ParamDec COMMA VarList {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("VarList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | ParamDec {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("VarList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;
ParamDec : Specifier VarDec {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("ParamDec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 ;

CompSt : LC DefList StmtList RC {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("CompSt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	$$ = leaf;
 	}
 ;
StmtList : Stmt StmtList {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("StmtList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 |{$$ = NULL;}
 ;
Stmt : Exp SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 | CompSt {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | RETURN Exp SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | IF LP Exp RP Stmt {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	leaf->children[leaf->childrennum++] = $5;
 	$$ = leaf;
 	}
 | IF LP Exp RP Stmt ELSE Stmt {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	leaf->children[leaf->childrennum++] = $5;
 	leaf->children[leaf->childrennum++] = $6;
 	leaf->children[leaf->childrennum++] = $7;
 	$$ = leaf;
 	}
 | WHILE LP Exp RP Stmt {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Stmt", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	leaf->children[leaf->childrennum++] = $5;
 	$$ = leaf;
 	}
 ;
DefList : Def DefList {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("DefList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 |{$$ = NULL;}
 ;
Def : Specifier DecList SEMI {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Def", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;
DecList : Dec {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("DecList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | Dec COMMA DecList {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("DecList", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;
Dec : VarDec {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Dec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | VarDec ASSIGNOP Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Dec", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 ;

Exp : Exp ASSIGNOP Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp AND Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp OR Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp RELOP Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp PLUS Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp MINUS Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp STAR Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp DIV Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | LP Exp RP {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | MINUS Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 | NOT Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	$$ = leaf;
 	}
 | ID LP Args RP {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	$$ = leaf;
 	}
 | ID LP RP {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp LB Exp RB {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	leaf->children[leaf->childrennum++] = $4;
 	$$ = leaf;
 	}
 | Exp DOT ID {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | ID {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | INT {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 | FLOAT {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;
Args : Exp COMMA Args {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Args", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	leaf->children[leaf->childrennum++] = $2;
 	leaf->children[leaf->childrennum++] = $3;
 	$$ = leaf;
 	}
 | Exp {
 	Syntax_Leaf* leaf = Syntax_Leaf_New("Exp", @$.first_line, 0);
 	leaf->children[leaf->childrennum++] = $1;
 	$$ = leaf;
 	}
 ;
 
 ExtDef : Specifier error SEMI {printf("using recover rule 1\n");}
 | error ExtDecList SEMI {printf("using recover rule 2\n");}
 ;
 StructSpecifier : STRUCT OptTag LC error RC {printf("using recover rule 4\n");}
 | STRUCT error LC error RC {printf("using recover rule 5\n");}
 ;
 
 FunDec : ID LP error RP {printf("using recover rule 7\n");}
 ;
 CompSt : LC error RC {printf("using recover rule 8\n");}
 ;
 Stmt: error SEMI {printf("using recover rule 9\n");}
 | RETURN error SEMI {printf("using recover rule 10\n");}
 | IF LP error RP Stmt {printf("using recover rule 11\n");}
 | IF LP error RP error ELSE Stmt {printf("using recover rule 12\n");}
 | WHILE LP error RP Stmt {printf("using recover rule 13\n");}
 ;
%%

