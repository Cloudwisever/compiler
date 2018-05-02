
#include "symbol_table.h"


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
			head = head->next;
		}
	}
	else
	{
		return 2;
	}
}

HashItem symbol_table[TABLE_SIZE];
		
HashItem SymbolTable_create(Syntax_Leaf* Syntax_Root)
{}

Type HandleSpecifier(Syntax_Leaf* spe_root)
{	
	Type newone = (Type)malloc(sizeof(struct Type_));
	memset(newone, 0, sizeof(struct Type_));
	
	if (spe_root->children[0]->type == TYPE_)
	{
		newone->kind = BASIC;
		if(strcmp(spe_root->content, "int") == 0)
		{
			newone->u.basic = 0;
		}
		else if (strcmp(spe_root->content, "float") == 0)
		{
			newone->u.basic = 1;
		}
		else
		{
			printf("unknown type: %s\n", spe_root->content);
		}
	}
	else if (spe_root->children[0]->type == TOKEN_ && strcmp(spe_root->children[1]->name, "OptTag") == 0)//StructSpecifier, declare a structure
	{
		newone->kind = STRUCTURE;
	
		Syntax_Leaf* deflist = spe_root->children[3];//DefList
		int i;
		
		FieldList newfield = (FieldList)malloc(sizeof(struct FieldList_));
		memset(newone, 0, sizeof(struct FieldList_));
			
		newone->u.structure = newfield;
		
		HandleDefList(spe_root->children[3], newone->u.structure, STRUCTUREFIELD);
		
		char* struct_name = NULL;
		if(spe_root->children[1] == NULL)//OptTag is empty
		{	
		}
		else if(spe_root->children[1] != NULL)
		{
			struct_name = spe_root->children[1]->content;
			//if the structure has a name, build a symbol table item for it.
			SymbolItem newitem = (SymbolItem)malloc(sizeof(struct SymbolItem_));
			memset(newitem, 0, sizeof(struct SymbolItem_));
			
			strcpy(newitem->name, struct_name);
			newitem->SymbolType = newone->u.structure->type;
			newitem->kind = STRUCTNAME;
			newitem->lineno = spe_root->children[1]->lineno;
			
			int rec = SymbolTable_Add(symbol_table, newitem);
			if(rec == 2)//error type 16
			{
				printf("Error type 16 at Line %i: Duplicated structure name \" %s \".\n", newitem->lineno, newitem->name);
			}
		}
		//head node stores the name of the structure, if it has one.
		newfield->name = struct_name;
		//headnode stores the type of the whole structure.
		newfield->type = newone;
		
		/*
		for(i = 0; i < deflist->childrennum; i++)
		{
			FieldList newfield = (FieldList)malloc(sizeof(struct FieldList_));
			memset(newone, 0, sizeof(struct FieldList_));
			
			if(newone->u.structure == NULL)
			{
				tail = newfield;
				newone->u.structure = newfield;
			}
			else
			{
				tail->next = newfield;
				tail = newfield;
			}
			newfield->type = HandleSpecifier(deflist->children[i]->children[0]);//the first child of Def is Specifier, also the type.
			newfield->name = struct_name;	
		}*/
	}
	else if (spe_root->children[0]->type == TOKEN_ && strcmp(spe_root->children[1]->name, "Tag") == 0)//StructSpecifier, use a structure already declared.
	{
		
	}
	else
	{
		printf("unknown Specifier's child: %s\n", spe_root->children[1]->name);
	}
	return newone;
}

FieldList HandleDefList(Syntax_Leaf* root, FieldList tail, int deflist_type)
{
	Syntax_Leaf* cur_deflist = root;

	while(cur_deflist != NULL)
	{
		Syntax_Leaf* def = cur_deflist->children[0];
		Type defType = HandleSpecifier(def->children[0]);
		
		tail = HandleDecList(def->children[1], tail, defType, deflist_type);
		
		cur_deflist = cur_deflist->children[1];
	}
	return tail;
}
FieldList HandleDecList(Syntax_Leaf* root, FieldList tail, Type decType, int deflist_type)
{
	Syntax_Leaf* cur_declist = root;
	do{
		SymbolItem dec = HandleDec(cur_declist->children[0], decType, deflist_type);
		
		if(deflist_type == STRUCTUREFIELD || tail != NULL)
		{
			dec->type_num = STRUCTFIELD;
			
			FieldList newfield = (FieldList)malloc(sizeof(struct FieldList_));
			memset(newfield, 0, sizeof(struct FieldList_));
		
			newfield->name = dec->name;
			newfield->type = dec->SymbolType;
		
			tail->next = newfield;
			tail = newfield;
		}
		else
			dec->type_num = VARIABLE;
		
		cur_declist = cur_declist->children[2];
	}while(cur_declist != NULL);
	
	return tail;
}			

SymbolItem HandleDec(Syntax_Leaf* root, Type decType, int deflist_type)
{
	Syntax_Leaf*var_root = root->children[0];
	SymbolItem var = HandleVarDec(var_root, decType,deflist_type);
	if(root->childrennum == 1)//Dec
	{
		var->initialized = 0;
	}
	else
	{
		var->initialized = 1;
		
		if(deflist_type == STRUCTREFIELD)//error 15: trying to initialize field in structure
		{
			printf("Error type 15 at Line %i: trying to initialize field \" %s \" in structure.\n", var->lineno, var->name);
		}
	}
	return var;
}

SymbolItem HandleVarDec(Syntax_Leaf* var_root, Type decType, int deflist_type)
{
	Syntax_Leaf* root = var_root;
	Type cur_type = decType;
	while(root->children[0]->type == TOKEN_)
	{
		Type newone = (Type)malloc(sizeof(struct Type_));
		memset(newone, 0, sizeof(struct Type_));
		
		newone->kind = ARRAY;
		newone->u.array.elem = cur_type;
		
		int array_size = 0;
		if(root->children[2]->type == FLOAT_)//error 12
		{
			printf("Error type 12 at line %i: \" %s \" is not an integer.\n", root->children[2]->lineno, root->children[2]->name);
			array_size = 1;
		}
		else if(root->children[2]->type == INT_)
		{
			array_size = atoi(root->children[2]->content);
		}
		newone->u.array.size = array_size;
		
		root = root->children[0];
		cur_type = newone;
	}
	
	SymbolItem newid = (SymbolItem)malloc(sizeof(struct SymbolItem_));
	memset(newid, 0, sizeof(struct SymbolItem_));
	if(root->children[0]->type == ID_)
	{
		
		strcpy(newid->name, root->children[0]->content);
		newid->SymbolType = cur_type;
		newid->lineno = root->children[0]->lineno;

		int rec = SymbolTable_Add(symbol_table, newid);
		if(rec == 2)
		{
			SymbolItem conflict = find(symbol_table, newid->name);
			if(deflist_type == STRUCTUREFIELD)
			{
				if(conflict->kind == VARIABLE)
				{
					printf("Error type 15 at Line %i:Field \" %s \"'s name has been defined as a variable.\n",root->children[0]->lineno, newid->name);
				}
				else if(conflict->kind == STRUCTNAME)
				{
					printf("Error type 15 at Line %i:Field \" %s \"'s name has been defined as a structure name.\n",root->children[0]->lineno, newid->name);
				}
				else
					printf("Error type 15 at Line %i:Field \" %s \"'s name has been defined as a field.\n", root->children[0]->lineno, newid->name);
			}
			else
			{
					printf("Error type 3 at Line %i:Redefined variable \" %s \".\n", root->children[0]->lineno, newid->name);
			}
		}
	}
	else
	{
		printf("Unknown VarDec children type : %i\n", root->children[0]->type);
	}
	return newid;
}
		
		


