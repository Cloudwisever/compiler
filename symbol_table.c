
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
		printf("adding: %s\n", sym->name);
		if(head == NULL)
		{
			HashItem newone = (HashItem)malloc(sizeof(struct HashItem_));
			memset(newone, 0, sizeof(struct HashItem_));
			newone->symbol = sym;
			symbol_tab[index] = newone;
			return 1;
		}
		while(head != NULL)
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

void SymbolItem_Print(SymbolItem sym)
{
	//printf("test\n");
	//printf("printf in print %p %i %s\n", sym->SymbolType, sym->kind, sym->name);
	//printf("%i\n",sym->SymbolType->kind);
	printf("%s\t", sym->name);
	if(sym->kind == STRUCTNAME)
	{
		printf("STRUCT\t");
	}
	else if(sym->kind == VARIABLE)
	{
		printf("VAR\t");
	}
	else if(sym->kind == STRUCTFIELD)
	{
		printf("FIELD\t");
	}
	else
		printf("BAD kind: %i\t", sym->kind);
	
	if(sym->SymbolType->kind == BASIC)
	{
		printf("BASIC\t");
	}
	else if(sym->SymbolType->kind == ARRAY)
	{
		printf("ARRAY\t");
	}
	else if(sym->SymbolType->kind == STRUCTURE)
	{
		printf("STRUCTURE\t");
	}
	else
		printf("BAD symbol kind: %i\t", sym->SymbolType->kind);

	/*
	int k = sym->kind;
	printf("k :%i", k);
	switch(k)
	{
	case 0:	printf("VAR\t");break;
	case 1:	printf("STRUCT\t");break;
	case 2:printf("FIELD\t");break;
	default: printf("BAD kind: %i\t", sym->kind);
	}
	k = sym->SymbolType->kind;
	switch(k)
	{
	case 0:	printf("BASIC\t");break;
	case 1:	printf("ARRAY\t");break;
	case 2:printf("SRUCTURE\t");break;
	}*/
	
	//printf("BAD kind: %i\t", sym->kind);
	printf("\n");
}

void SymbolTable_Print(HashItem* Symbol_Table)
{
	int i;
	printf("NAME\tKIND\tTYPE\n");
	
	//HashItem ap = SymbolTable_Find(symbol_table, "p");
	/*if(ap==NULL)
	{
		printf("p not added.\n");
	}
	else
	{
		printf("printing:\n");
		SymbolItem_Print(ap->symbol);
	}*/
	for(i = 0; i < TABLE_SIZE; i++)
	{
		HashItem cur = Symbol_Table[i];
		while(cur != NULL)
		{
			//printf("cur: %p, cur->sym->name: %s cur->sym->kind: %i\n",cur, cur->symbol->name, cur->symbol->kind);
			SymbolItem_Print(cur->symbol);
			cur = cur->next;
		}
	}
}

		

HashItem SymbolTable_Create(Syntax_Leaf* root)
{
	if(strcmp(root->name, "Specifier") == 0)
	{
		HandleSpecifier(root);
	}
	int i = 0;
	while(i < root->childrennum)
	{
		if(root->children[i] == NULL)
		{
			i++;
			continue;
		}
		SymbolTable_Create(root->children[i++]);
	}
}



Type HandleSpecifier(Syntax_Leaf* root)//int, float, struct a: deal with memory leaking.
{	
	printf("handling specifier at line %i\n", root->lineno);
	Type newone = (Type)malloc(sizeof(struct Type_));
	memset(newone, 0, sizeof(struct Type_));
	
	printf("root->children[0]->type: %i name: %s\n", root->children[0]->type,root->children[0]->name);
	Syntax_Leaf* spe_root = root->children[0];
	
	if (root->children[0]->type == TYPE_)
	{
		newone->kind = BASIC;
		printf("content: %s\n", spe_root->content);
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
	else if ( strcmp(spe_root->children[1]->name, "OptTag") == 0)//StructSpecifier, declare a structure
	{
		newone->kind = STRUCTURE;
		
		//printf("declar a struct: %s\n", spe_root->children[1]->name);
	
		Syntax_Leaf* deflist = spe_root->children[3];//DefList
		int i;
		
		FieldList newfield = (FieldList)malloc(sizeof(struct FieldList_));
		memset(newone, 0, sizeof(struct FieldList_));
			
		newone->u.structure = newfield;
		
		//printf("u.structure before: %p\n", newone->u.structure);
		
		HandleDefList(spe_root->children[3], newone->u.structure, STRUCTFIELD);
		
		//printf("u.structure after: %p\n", newone->u.structure);
		
		//printf("u.structure.next type: %p %s\n", newone->u.structure->next,newone->u.structure->next->name);
		//u.structure is the head node of the fieldlist. have to skip the head node.
		FieldList tmp = newone->u.structure->next;
		newone->u.structure->name = newone->u.structure->next->name;
		newone->u.structure->type = newone->u.structure->next->type;
		newone->u.structure->next = newone->u.structure->next->next;
		free(tmp);
		char* struct_name = NULL;
		if(spe_root->children[1]->children[0] == NULL)//OptTag is empty
		{	
		}
		else if(spe_root->children[1]->children[0] != NULL)
		{
			struct_name = spe_root->children[1]->children[0]->content;
			//if the structure has a name, build a symbol table item for it.
			SymbolItem newitem = (SymbolItem)malloc(sizeof(struct SymbolItem_));
			memset(newitem, 0, sizeof(struct SymbolItem_));
			
			strcpy(newitem->name, struct_name);
			printf("type kind:%p\n", newone->u.structure->type);
			newitem->SymbolType = newone->u.structure->type;
			newitem->kind = STRUCTNAME;
			newitem->lineno = spe_root->children[1]->children[0]->lineno;
			
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
	else if (strcmp(spe_root->children[0]->children[1]->name, "Tag") == 0)//StructSpecifier, use a structure already declared.
	{
		Syntax_Leaf* tag = spe_root->children[1];
		HashItem rechash = SymbolTable_Find(symbol_table, tag->children[0]->content);
		SymbolItem rec = rechash->symbol;
		
		if(rec == NULL)//error type 17
		{
			printf("Error type 17 at Line %i: Undefined structure \" %s \",\n", tag->lineno, rec->name);
			//return a special basic type
			newone->kind = BASIC;
			newone->u.basic = 2;
		}
		else
		{
			newone->kind = rec->SymbolType->kind;
			newone->u = rec->SymbolType->u;
		}
	}
	else
	{
		printf("unknown StructSpecifier's child: %s, cmp : % i type: %i, TOKEN_ : %i\n", spe_root->children[1]->name, strcmp(spe_root->children[1]->name, "OptTag"), spe_root->children[0]->type, TOKEN_);
	}
	return newone;
}
/*
void HandleExtDef(Syntax_Leaf* root)
{
	
	Type extdeftype = HandleSpecifier(root->children[0]);
	if(strcmp(root->children[1]->name, "ExtDecList") == 0)
	{
		//since the ExtDecList is same as DecList, we can just use the HandleDecList() to fill the symbol table.
		HandleDecList(root->children[1], NULL, extdeftype, VARIABLE);
	}
	else if(strcmp(root->children[1]->name, "SEMI") == 0)
	{
		//to add the structure into the symbol table, the HandleSpecifier() has done that. So no need to do anything.
	}
	else if(strcmp(root->children[1]->name, "FunDec") == 0 && strcmp(root->children[2]->name, "SEMI") == 0)//Function declare
	{
		HandleFunDec(root->children[1]);
	}
	else if(strcmp(root->children[1]->name, "FunDec") == 0 && strcmp(root->children[2]->name, "CompSt") == 0)
	{
	}
}

void HandleFunDec(Syntax_Leaf* root, Type returnType)
{
	Syntax_Leaf* id = root->children[0];
}
*/
		
		

FieldList HandleDefList(Syntax_Leaf* root, FieldList tail, int deflist_type)
{
	//printf("handling deflist at line %i\n", root->lineno);
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
		
		if(deflist_type == STRUCTFIELD || tail != NULL)
		{
			dec->kind = STRUCTFIELD;
			
			FieldList newfield = (FieldList)malloc(sizeof(struct FieldList_));
			memset(newfield, 0, sizeof(struct FieldList_));
		
			newfield->name = dec->name;
			newfield->type = dec->SymbolType;
		
			tail->next = newfield;
			tail = newfield;
		}
		else
			dec->kind = VARIABLE;
		
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
		
		if(deflist_type == STRUCTFIELD)//error 15: trying to initialize field in structure
		{
			printf("Error type 15 at Line %i: trying to initialize field \" %s \" in structure.\n", var->lineno, var->name);
		}
	}
	return var;
}

SymbolItem HandleVarDec(Syntax_Leaf* var_root, Type decType, int deflist_type)
{
	//printf("handling vardec at line %i\n", var_root->lineno);
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
			HashItem conflicthash = SymbolTable_Find(symbol_table, newid->name);
			SymbolItem conflict = conflicthash->symbol;
			if(deflist_type == STRUCTFIELD)
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
		

		


