#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE  0x3ff

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_
{
	enum {BASIC, ARRAY, STRUCTURE} kind;
	union
	{
		int basic;
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

unsigned int hash_pjw(char* name)
{
	unsigned val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3ff)
			val = (val ^ (i >> 12)) & 0x3ff;
	}
	return val;
}

HashItem SymbolTable_Find(HashItem *symbol_tab, char* name)
{
	int index = hash_pjw(name);
	HashItem head = NULL;
	head = symbol_tab[index];
	
	while(head != NULL)
	{
		if(strcmp(head->symbol->name, name) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}

int SymbolTable_Add(HashItem* symbol_tab, SymbolItem sym)
{
	int returnval = 0;
	int index = hash_pjw(sym->name);
	HashItem head = symbol_tab[index];
	
	HashItem find = SymbolTable_Find(symbol_tab, sym->name);
	if(find == NULL)
	{
		while(head != 0)
		{
			if(strcmp(sym->name, head->symbol->name) < 0)
			{
				if(head -> next == NULL || strcmp(sym->name, head->next->symbol->name) > 0)
				{
					HashItem newone = (HashItem)malloc(sizeof(struct HashItem_));
					memset(newone, 0, sizeof(struct HashItem_));
					newone->symbol = sym;
					head->next = newone;
					return 1;
				}
			}
			else if(strcmp(sym->name, head->symbol->name) > 0)
			{
				HashItem newone = (HashItem)malloc(sizeof(struct HashItem_));
				memset(newone, 0, sizeof(struct HashItem_));
				newone->symbol = sym;
				newone->next = head;
				symbol_tab[index] = newone;
				return 1;
			}
			else
			{
				printf("should not be here.\n");
				returnval = 3;
			}
		}
	}
	else
	{
		return 2;
	}
}

				
	
		
		
		
		
		
		
