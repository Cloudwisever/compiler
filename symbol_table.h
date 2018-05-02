#ifndef _TABLE_H
#define _TABLE_H
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_tree.h"


#define TABLE_SIZE  0x3ff

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_
{
	enum {BASIC, ARRAY, STRUCTURE} kind;
	union
	{
		int basic;//int:0 float:1 structure not declared: 2
		struct {Type elem; int size;} array;
		FieldList structure;
	}u;
};

struct FieldList_
{
	char* name;
	Type type;
	FieldList next;
};

typedef struct SymbolItem_* SymbolItem;

struct SymbolItem_
{
	char name[32];
	Type SymbolType;
	int initialized;
	enum {VARIABLE, STRUCTNAME, STRUCTFIELD} kind;//variable: 0 structure name: 1 field type: 2
	int lineno; 
	SymbolItem next;
};

typedef struct HashItem_* HashItem;
typedef struct HashItem_ Hashdef;
struct HashItem_
{
	int current_num;
	SymbolItem symbol;
	HashItem next;
};


HashItem symbol_table[TABLE_SIZE];


unsigned int hash_pjw(char* name);
HashItem SymbolTable_Find(HashItem *symbol_tab, char* name);
int SymbolTable_Add(HashItem* symbol_tab, SymbolItem sym);

HashItem SymbolTable_Create(Syntax_Leaf* root);
void SymbolTable_Print(HashItem* Symbol_Table);
void SymbolItem_Print(SymbolItem sym);

SymbolItem HandleVarDec(Syntax_Leaf* var_root, Type decType, int deflist_type);
SymbolItem HandleDec(Syntax_Leaf* root, Type decType, int deflist_type);
FieldList HandleDecList(Syntax_Leaf* root, FieldList tail, Type decType, int deflist_type);
FieldList HandleDefList(Syntax_Leaf* root, FieldList tail, int deflist_type);
Type HandleSpecifier(Syntax_Leaf* spe_root);

#endif
