#include "symbol_table.h"

HashItem symbol_table[TABLE_SIZE];

static struct Type_ int_ = {BASIC, 0};
static struct Type_ float_ = {BASIC, 1};
static Type stdINT = &int_;
static Type errINT = &int_;
static Type stdFLOAT = &float_;

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
	//printf("name:%s, hashval: %i\n", name, index);
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
	//printf("adding:%s\n", sym->name);
	
	//printf("inadd: name:%s, hashval: %i\n", sym->name, index);
	HashItem find = SymbolTable_Find(symbol_tab, sym->name);
	
	int addplace = 0;
	
	if(find == NULL)
	{
		//printf("adding: %s\n", sym->name);
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
					
					HashItem temp = head->next;
					head->next = newone;
					newone->next = temp;
				
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
			addplace++;
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
	/*
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
	else if(sym->kind == FUNC)
	{rec->name : a
Error type 3 at Line 12:Redefined variable " p1 ".
Error type 3 at Line 10:Redefined variable " d2 ".

		printf("FUNC\t");
	}
	else
		printf("BAD kind: %i\t", sym->kind);
	*/	
	
	switch(sym->kind)
	{
	case VARIABLE:	printf("VAR\t");break;
	case STRUCTNAME:	printf("STRUCT\t");break;
	case STRUCTFIELD:printf("FIELD\t");break;
	case FUNC:	printf("FUNC\t");break;
	case FUNCDECPARAM:	printf("FUNCDECPARAM\t");break;
	case FUNCDEFPARAM:	printf("FUNCDEFPARAM\t");break;
	default: printf("BAD kind: %i\t", sym->kind);
	}
	
	switch(sym->SymbolType->kind)
	{
	case BASIC:	
		if(sym->SymbolType->u.basic == 1)
			printf("FLOAT\t");
		else
			printf("INT\t");
		break;
	case ARRAY:	printf("ARRAY\t");break;
	case STRUCTURE:printf("SRUCTURE\t");break;
	}
	/*
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
	*/
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

void SymbolTable_Print(HashItem* Symbol_Table, int opt)
{
	int i;
	if(opt == 1)
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
		//if(Symbol_Table[i]!=NULL)
			//printf("%i : name: %s\n", i, Symbol_Table[i]->symbol->name);
		while(cur != NULL)
		{
			//printf("cur: %p, cur->sym->name: %s cur->sym->kind: %i\n",cur, cur->symbol->name, cur->symbol->kind);
			if(cur->symbol->kind == FUNC)
			{
				if(cur->symbol->initialized != 1)//declared but not defined
				{
					printf("Error type 18 at Line %i: Undefined function \"%s\".\n", cur->symbol->lineno, cur->symbol->name);
				}
			}
			if(opt == 1)
			{	SymbolItem_Print(cur->symbol);
			}
			cur = cur->next;
		}
	}
}


void SymbolTable_Create(Syntax_Leaf* root)
{
	if(root == NULL)
		return;
	if(strcmp(root->name, "ExtDef") == 0)
	{
		HandleExtDef(root);
		return;
	}
	else
	{
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
}

int CheckTypeEq(Type T1, Type T2)
{
	if(T1 == NULL && T2 == NULL)
		return 1;
	if(T1->kind != T2->kind)
	{
		return 0;
	}
	else
	{
	
		//printf("checking \n");
		if(T1->kind == BASIC)
		{
			//printf("\nT1 basic %i T2 %i\n", T1->u.basic, T2->u.basic);
			if(T1->u.basic == T2->u.basic)
				return 1;
			else
				return 0;
		}
		else if(T1->kind == ARRAY)
		{
			int dim1 = 0, dim2 = 0;
			Type curType1 = T1->u.array.elem, curType2 = T2->u.array.elem;
			while(curType1->kind == ARRAY && curType2->kind == ARRAY)
			{
				curType1 = curType1->u.array.elem;
				curType2 = curType2->u.array.elem;
			}
			if(curType1->kind == curType2->kind)
				return 1;
			else
				return 0;
		}
		else if(T1->kind == STRUCTURE)
		{
			FieldList cur1 = T1->u.structure->next, cur2 = T2->u.structure->next;
			while(1)
			{
				if(strcmp(cur1->name, cur2->name) != 0)
				{
					return 0;
				}
				//printf("check struct\n");
				//printf("cur1 basic %i cur2 %i\n", cur1->u.basic, cur2->u.basic);
				if(CheckTypeEq(cur1->type,cur2->type) != 1)
					return 0;
				cur1 = cur1->next;
				cur2 = cur2->next;
				if(cur1 == NULL && cur2 == NULL)
					return 1;
				else if(cur1 == NULL || cur2 == NULL)
					return 0;
			}
			return 1;
		}
		else
		{
			printf("Unknown kind: %i\n", T1->kind);
			return 0;
		}
	}
}	
		
int CheckFunDec(SymbolItem F1, SymbolItem F2)
{
	if(F1 == NULL && F2 == NULL)
	{	
		//printf("checking1\n");
		return 1;
	}
	else if(F1 == NULL || F2 == NULL)
	{
		//printf("checking0\n");
		return 0;
	}
	/*
	SymbolItem ppp = F1;
	while(ppp != NULL)
	{
		printf("F1typekind %i, ", ppp->SymbolType->kind);
		ppp = ppp->next;
	}
	printf("\n");
	ppp = F2;
	while(ppp != NULL)
	{
		printf("F2typekind %i, ", ppp->SymbolType->kind);
		ppp = ppp->next;
	}
	printf("\n");
	*/
	
	//printf("checking F1 kind %i F2 kind %i\n", F1->kind, F2->kind);
	
	if(F1->kind != F2->kind)
	{
	
		//printf("checking3 F1->kind %i F2kind %i\n", F1->kind, F2->kind);
		return 3;
	}
	while(F1 != NULL && F2 != NULL)
	{
		//printf("F1typekind %i, basic %i, f2typekind %i, basic %i\n", F1->SymbolType->kind,F1->SymbolType->u.basic, F2->SymbolType->kind, F2->SymbolType->u.basic);
		if(CheckTypeEq(F1->SymbolType, F2->SymbolType) != 1)
		{
			//printf("checking2\n");
			return 2;
		}
		F1 = F1->next; F2 = F2->next;
	}
	if(F1 == NULL && F2 == NULL)
		return 1;
	else
		return 0;
}

Type HandleExp(Syntax_Leaf* root)
{
	if(root->children[0]->type == ID_)
	{
		Syntax_Leaf* ID_root = root->children[0];
		
		if(root->children[1] == NULL)//Exp : ID
		{
			//printf("ID:%s\n", root->children[0]->content);
			HashItem idhash = SymbolTable_Find(symbol_table, ID_root->content);
			if(idhash == NULL)
			{
				printf("Error type 1 at Line %i: Undefined variable \"%s\".\n", ID_root->lineno, ID_root->content);
				return errINT;
			}
			else
			{
				return idhash->symbol->SymbolType;
			}
		}
		else if(strcmp(root->children[1]->name, "LP") == 0)//function
		{
			HashItem idhash = SymbolTable_Find(symbol_table, ID_root->content);
			if(idhash == NULL)
			{
				printf("Error type 2 at Line %i: Undefined function \"%s\".\n", root->lineno, ID_root->content);
				return errINT;	
			}
			else
			{
				SymbolItem funcsym = idhash->symbol;
				if(idhash->symbol->kind != FUNC)
				{
					printf("Error type 11 at Line %i: symbol \"%s\" is not a function.\n", ID_root->lineno, ID_root->content);
					return errINT;
				}
				else
				{
					if(strcmp(root->children[2]->name, "Args") == 0)
					{
						if(CheckArgs(root->children[2], funcsym->next) != 1)
						{
							printf("Error type 9 at Line %i: Function \"%s\" is not applicable for given arguments.\n", ID_root->lineno, ID_root->content);
							return funcsym->SymbolType;
						}
						else
							return funcsym->SymbolType;
					}
					else//function without param
					{
						if(funcsym->next != NULL)
						{
							printf("Error type 9 at Line %i: Function \"%s\" is not applicable for no arguments.\n", ID_root->lineno, ID_root->content);
							return errINT;
						}
						else
						{
							return funcsym->SymbolType;
						}
					}
				}
			}
		}
		else
		{
			printf("bad expression.\n");
			return errINT;
		}
	}
	else if(root->children[0]->type == INT_)
		return stdINT;
	else if(root->children[0]->type == FLOAT_)
	{
		//printf("returning float: type %i\n", stdFLOAT->u.basic);	
		return stdFLOAT;
	}
	else if(root->children[0]->type == LEX_||root->children[0]->type == ARITH_)//NOT Exp, MINUS Exp
	{
		return HandleExp(root->children[1]);
	}
	else if(root->children[0]->type == TOKEN_)//Exp : Exp ...
	{
		//printf("exop:exp\n");
		//printf("children1 name %s type %i\n", root->children[1]->name, root->children[1]->type);
		Syntax_Leaf* exp1_root = root->children[0], *exp2_root = root->children[2];
		if(strcmp(root->children[1]->name, "LB") == 0)//exp[exp]
		{
			//printf("dealing exp[exp]\n");
			Type exp1Type = HandleExp(exp1_root);
			if(exp1Type->kind != ARRAY)
			{
				printf("Error type 10 at Line %i: \"%s\" is not an array.\n", exp1_root->lineno, ExpGetName(exp1_root));
				return errINT;
			}
			else
			{
				Type exp2Type = HandleExp(exp2_root);
				//printf("inside type:%i\n", exp2Type->u.basic);
				if(exp2Type->kind != BASIC || exp2Type->u.basic != 0)
				{
					printf("Error type 12 at Line %i: \"%s\" is not an integer.\n", exp2_root->lineno, ExpGetName(exp2_root));
					return errINT;
				}
				else
				{
					return exp1Type->u.array.elem;
				}
			}
		}
		else if(strcmp(root->children[1]->name, "DOT") == 0)//Exp : Exp DOT ID
		{
			Type exp1Type = HandleExp(exp1_root);
			if(exp1Type->kind != STRUCTURE)
			{
				printf("exp1Type kind = %i\n", exp1Type->kind);
				printf("Error type 13 at Line %i: Illegal use of \"%c\".\n", exp1_root->lineno, '.');
				return errINT;
			}
			else
			{
				HashItem field = SymbolTable_Find(symbol_table, root->children[2]->content);
				if(field == NULL || field->symbol->kind != STRUCTFIELD)
				{
					printf("Error type 14 at Line %i: Non-existent field \"%s\".\n", root->children[2]->lineno, root->children[2]->content);
					return errINT;
				}
				else
				{
					//printf("field master:%s\n", field->symbol->field_master);
					//printf("exp1Type name:%s\n", exp1Type->u.structure->name);
					if(strcmp(field->symbol->field_master, exp1Type->u.structure->name) != 0)
					{
						printf("Error type 14 at Line %i: %s visiting other structure %s's field \"%s\".\n", root->children[2]->lineno, exp1Type->u.structure->name, field->symbol->field_master, root->children[2]->content);
						return errINT;
					}
					return field->symbol->SymbolType;
				}
			}
		}
		else if(root->children[1]->type == ASSIGNOP_ )
		{
			//check left
			//printf("assigning. good? at line %i\n", root->children[1]->lineno);
			int good = 0;
			if(exp1_root->children[0]->type == ID_&&exp1_root->children[1] == NULL)//exp is ID, not func
			{
				//printf("Line %i, good because left is id\n",exp1_root->children[0]->lineno);
				good = 1;
			}
			else if((strcmp(exp1_root->children[0]->name, "Exp") == 0)&&(strcmp(exp1_root->children[1]->name, "LB") == 0)&&(strcmp(exp1_root->children[2]->name, "Exp") == 0)&&(strcmp(exp1_root->children[3]->name, "RB") == 0))
			{
				//printf("Line %i, good because left is Exp LB Exp RB\n",exp1_root->children[0]->lineno);
				good = 1;
			}
			else if((strcmp(exp1_root->children[0]->name, "Exp") == 0)&&(strcmp(exp1_root->children[1]->name, "DOT") == 0)&&(strcmp(exp1_root->children[2]->name, "ID") == 0))
			{
				//printf("Line %i, good because left is Exp DOT ID\n",exp1_root->children[0]->lineno);
				good = 1;
			}
			//printf("good = %i\n", good);
			if(good != 1)
			{
				printf("Error type 6 at Line %i: The left-hand side of an assignment nust be a variable.\n", exp1_root->children[0]->lineno);
				return errINT;
			}
			//check type
			//printf("ass exp1: before\n"); 
			Type exp1Type = HandleExp(exp1_root);
			//printf("ass exp1: after\n"); 
			//printf("\nass exp2: before\n"); 
			Type exp2Type = HandleExp(exp2_root);
			//printf("ass exp2: after\n");
			//according to the c-- defination, only comeletely equal type can assign.
			
			//printf("check type eq = %i\n", CheckTypeEq(exp1Type, exp2Type));
			if(CheckTypeEq(exp1Type, exp2Type) != 1)
			{
				printf("Error type 5 at Line %i: Type mismatched for assignment.\n", exp1_root->lineno);
				return errINT;
			}		
			else
				return exp1Type;
		}
		else if(root->children[1]->type == LOGICAL_)//only int
		{
			Type exp1Type = HandleExp(exp1_root);
			Type exp2Type = HandleExp(exp2_root);
			if(exp1Type->kind != BASIC || exp1Type->kind != exp2Type->kind || exp1Type->u.basic != exp2Type->u.basic || exp1Type->u.basic != 0)
			{
				printf("Error type 7 at Line %i: Type mismatched for oprands.\n", exp1_root->lineno);
				return errINT;
			}
			else
			{
				return exp1Type;
			}
		}
		else if(root->children[1]->type == ARITH_)
		{
			//printf("arith:\n");
			Type exp1Type = HandleExp(exp1_root);
			Type exp2Type = HandleExp(exp2_root);
			if(exp1Type->kind != BASIC || exp1Type->kind != exp2Type->kind || exp1Type->u.basic != exp2Type->u.basic)	
			{
				printf("Error type 7 at Line %i: Type mismatched for oprands.\n", exp1_root->lineno);
				return errINT;
			}
			else
				return exp1Type;
		}
		else
		{
			printf("unknown expression : Exp : %s %s %s \n", root->children[0]->name, root->children[1]->name, root->children[2]->name);
			return errINT;
		}
	}
	else
	{
		printf("here?Exp type = %i\n",root->children[0]->type);
		printf("unknown expression : Exp : %s %s %s \n", root->children[0]->name, root->children[1]->name, root->children[2]->name);
		return errINT;
	}
}

char* ExpGetName(Syntax_Leaf* root)
{
	while(root->type != ID_ && root->type != INT_ && root->type != FLOAT_)
		root = root->children[0];
	return root->content;
}

int CheckArgs(Syntax_Leaf* root, SymbolItem param)
{
	Syntax_Leaf* cur = root, *exp = root->children[0];
	do
	{
		exp = cur->children[0];
		if(param == NULL)
		{
			return 0;
		}
		Type ret = HandleExp(cur->children[0]);
		//printf("ret_type.kind = %i, param-type.kind = %i\n", ret->kind, param->SymbolType->kind); 
		if(CheckTypeEq(ret, param->SymbolType) != 1)
		{
		//printf("ret_type.kind = %i, param-type.kind = %i\n", ret->kind, param->SymbolType->kind); 
			return 0;
		}
		cur = cur->children[2];
		param = param->next;
	}while(cur != NULL);
	if(param == NULL)
	{
		return 1;
	}
	else
	{
		printf("prama name = %s\n,return here\n", param->name);
		return 0;
	}
}
void HandleExtDecList(Syntax_Leaf* root, Type decType)
{
	Syntax_Leaf* cur = root, *vardec = root->children[0];
	do
	{
		vardec = cur->children[0];
		
		HandleVarDec(vardec, decType, VARIABLE);
		
		cur = cur->children[2];
	}while(cur != NULL);
}

void HandleExtDef(Syntax_Leaf* root)
{
	
	Type extdefType = HandleSpecifier(root->children[0]);
	if(strcmp(root->children[1]->name, "ExtDecList") == 0)
	{
		//since the ExtDecList is same as DecList, we can just use the HandleDecList() to fill the symbol table.IT IS WRONG!!!!!!
		//TRUTH IS: DecList->Dec->VarDec, HOWEVER, ExtDecList->VarDec!!!!!!
		//HandleDecList(root->children[1], NULL, extdefType, VARIABLE);
		HandleExtDecList(root->children[1], extdefType);
	}
	else if(strcmp(root->children[1]->name, "SEMI") == 0)
	{
		//to add the structure into the symbol table, the HandleSpecifier() has done that. So no need to do anything.
	}
	else if(strcmp(root->children[1]->name, "FunDec") == 0 && strcmp(root->children[2]->name, "SEMI") == 0)//Function declare
	{
		SymbolItem func = HandleFunDec(root->children[1], extdefType, FUNCDECPARAM);
		//printf("FunDec handled.\n");
	}
	else if(strcmp(root->children[1]->name, "FunDec") == 0 && strcmp(root->children[2]->name, "CompSt") == 0)//Function Defination
	{
	
		if(symbol_table[747]!=NULL)
		{
			HashItem fun = SymbolTable_Find(symbol_table, "func");
			if(fun == NULL)
				printf("\nno func aready.\n");
			else
				printf("fun sym: %s\n", fun->symbol->name);
			printf("what in 747: %s, kind: %i, id : %s\n", symbol_table[747]->symbol->name, symbol_table[747]->symbol->kind, root->children[0]->name);
		}
		SymbolItem func = HandleFunDec(root->children[1], extdefType, FUNCDEFPARAM);
		func->initialized = 1;
		
		Type returnType = HandleCompSt(root->children[2], func->SymbolType);
		if(CheckTypeEq(func->SymbolType, returnType) != 1)
		{
			printf("Error type 8 at Line %i: Type mismatched for return.\n", root->children[1]->lineno);
		}
	}
}

Type HandleCompSt(Syntax_Leaf* root, Type funcType)//returns the return type of CompSt
{
	HandleDefList(root->children[1], NULL, VARIABLE);
	return HandleStmtList(root->children[2], funcType);
}
Type HandleStmt(Syntax_Leaf* root, Type funcType)
{
	int i = 0;
	Syntax_Leaf* stmt = root;
	if(strcmp(stmt->children[0]->name,"CompSt") == 0)
	{
		HandleCompSt(stmt->children[0], funcType);
	}
	if(strcmp(stmt->children[0]->name,"RETURN") == 0)
	{
		Type thisreturn = HandleExp(stmt->children[1]);
		//printf("thisreturn.basic = %i\n", thisreturn->u.basic);
		if(CheckTypeEq(funcType, thisreturn) != 1)
		{
			printf("Error type 8 at Line %i: Type mismatched for return.\n", stmt->lineno);
		}
	}
	else
	{
		while(i < stmt->childrennum)
		{
			if(stmt->children[i] == NULL)
			{
				i++;
				continue;
			}
				if(strcmp(stmt->children[i]->name,"Exp") == 0)
					HandleExp(stmt->children[i]);
				else if(strcmp(stmt->children[i]->name,"Stmt") == 0)
					HandleStmt(stmt->children[i], funcType);
				i++;
		}
	}
}
Type HandleStmtList(Syntax_Leaf* root, Type funcType)
{
	Syntax_Leaf* stmtlist = root, *stmt;
	while(stmtlist != NULL)
	{
		stmt = stmtlist->children[0];
		//printf("name: %s\n",stmtlist->children[0]->name);
		HandleStmt(stmt, funcType);
		stmtlist = stmtlist->children[1];
	}
	return funcType;
}	

SymbolItem HandleFunDec(Syntax_Leaf* root, Type returnType, int funparam_type)
{
	//printf("handling :\n");
	Syntax_Leaf* id = root->children[0];
	
	SymbolItem newitem = (SymbolItem)malloc(sizeof(struct SymbolItem_));
	//printf("Newitemp:%pn", newitem);
	memset(newitem, 0, sizeof(struct SymbolItem_));
		
	newitem->kind = FUNC;
	strcpy(newitem->name, id->content);
	newitem->lineno = id->lineno;
	newitem->SymbolType = returnType;
	
	if(root->children[2]->type == TOKEN_)//varlist
	{
		newitem->next = HandleVarList(root->children[2], funparam_type);
		
		//printf("handled varlist\n");
	}
	else if(root->children[2]->type == LEX_)//no param
	{
		newitem->next = NULL;
	}
	/*
	SymbolItem ppp = newitem->next;
	while(ppp != NULL)
	{
		//printf("varlisttypekind %i, ", ppp->SymbolType->kind);
		ppp = ppp->next;
	}
	printf("\n");
	*/
	/*	if(symbol_table[747]!=NULL)
		{
			HashItem fun = SymbolTable_Find(symbol_table, "func");
			if(fun == NULL)
				printf("no func aready.\n");
			else
				printf("fun sym: %s\n", fun->symbol->name);
			printf("\nwhat in 747: %s, kind: %i, id : %s\n", symbol_table[747]->symbol->name, symbol_table[747]->symbol->kind, root->children[0]->name);
		}
		*/
	HashItem hashp = SymbolTable_Find(symbol_table, newitem->name);
	if(hashp != NULL)
	{
		if(hashp->symbol->kind == FUNC)
		{
			if(funparam_type == FUNCDEFPARAM)//defining a function
			{
				if(hashp->symbol->initialized == 1)//already defined
				{
					printf("Error type 4 at Line %i: Redefined function \"%s\".\n", root->lineno, newitem->name);
					//free(newitem);
					//printf("item freed\n");
					newitem = hashp->symbol;
					return newitem;
				}
				else if(CheckFunDec(hashp->symbol, newitem) != 1)
				{
					printf("Error type 19 at Line %i: Inconsisitent declarction of function \"%s\".\n", root->lineno, newitem->name);
					hashp->symbol->initialized = 1;
					//printf("checkfundec returns %i\n", CheckFunDec(hashp->symbol, newitem));
				}
				else
				{
					//free(newitem);
					//printf("item freed\n");
					newitem = hashp->symbol;
				}
			}
			else//declaring a function
			{	
				/*SymbolItem ppp = hashp->symbol->next;
				while(ppp != NULL)
				{
					printf("hashvarlisttypekind %i, ", ppp->SymbolType->kind);
					ppp = ppp->next;
				}
				printf("\n");
				printf("declare check:%s\n", newitem->name);
				
				*/
				if(CheckFunDec(hashp->symbol, newitem) != 1)
				{
					printf("Error type 19 at Line %i: Inconsisitent declarction of function \"%s\".\n", root->lineno, newitem->name);
				//printf("checkfundec returns %i\n", CheckFunDec(hashp->symbol, newitem));
				}
				else
				{
					//printf("hashp->symbol problem?%s\n", hashp->symbol->name);
					//free(newitem);
					//printf("item freed\n");
					//printf("hashp->symbol problem?%s\n", hashp->symbol->name);
					newitem = hashp->symbol;
				}
			}
		}
		else
		{
			printf("Error type 4 at Line %i: Function name is ocupied:\"%s\".\n", root->lineno, newitem->name);
		}
	}
	else
	{
		//printf("first add %s in line %i\n", newitem->name, root->lineno);
		SymbolTable_Add(symbol_table, newitem);
		
	}	
	return newitem;
}
SymbolItem HandleVarList(Syntax_Leaf* root, int funcparam_type)
{
	
	Syntax_Leaf* varlist = root, *para = root->children[0];
	SymbolItem head = NULL, tail = NULL;
	do
	{
		para = varlist->children[0];
		//printf("\npara.name = %s\n", para->name);
		SymbolItem newitem = HandleParamDec(para, funcparam_type);
		if(head == NULL)
		{
			head = newitem;
			tail = newitem;
		}
		else
		{
			tail->next = newitem;
			tail = newitem;
		}
		varlist = varlist->children[2];
	}while(varlist != NULL);
	
	return head;
}

SymbolItem HandleParamDec(Syntax_Leaf* root, int funparam_type)
{
	
	Type paraType = HandleSpecifier(root->children[0]);
	//printf("paraType handled.\n");
	SymbolItem rec = HandleVarDec(root->children[1], paraType, funparam_type);
	
	if(funparam_type == FUNCDECPARAM)
	{
		rec->kind = FUNCDECPARAM;
	}
	else if(funparam_type == FUNCDEFPARAM)
	{
		rec->kind = FUNCDEFPARAM;
	}
	else
	{
		printf("bad funparam type: %i\n", funparam_type);
	}	
	//printf("vardec ok.\n");
	return rec;
}


Type HandleSpecifier(Syntax_Leaf* root)//int, float, struct a: deal with memory leaking.
{	
	//printf("handling specifier at line %i\n", root->lineno);
	Type newone = (Type)malloc(sizeof(struct Type_));
	memset(newone, 0, sizeof(struct Type_));
	
	//printf("root->children[0]->type: %i name: %s\n", root->children[0]->type,root->children[0]->name);
	Syntax_Leaf* spe_root = root->children[0];
	
	if (root->children[0]->type == TYPE_)
	{
		newone->kind = BASIC;
		//printf("content: %s\n", spe_root->content);
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
		memset(newfield, 0, sizeof(struct FieldList_));
			
		newone->u.structure = newfield;
		
		HandleDefList(spe_root->children[3], newone->u.structure, STRUCTFIELD);
		
		//printf("u.structure.next type: %p %s\n", newone->u.structure->next,newone->u.structure->next->name);
		//u.structure is the head node of the fieldlist. have to skip the head node.
		FieldList tmp = newone->u.structure->next;
		//newone->u.structure->name = newone->u.structure->next->name;
		//newone->u.structure->type = newone->u.structure->next->type;
		//newone->u.structure->next = newone->u.structure->next->next;
		//free(tmp);
		char* struct_name = NULL;
		if(spe_root->children[1]->children[0] == NULL)//OptTag is empty
		{	
			printf("no opttag\n");
		}
		else if(spe_root->children[1]->children[0] != NULL)
		{
			struct_name = spe_root->children[1]->children[0]->content;
	
			FieldListFillMaster(tmp, struct_name);
			
			//head node of the Field List
			newone->u.structure->name = struct_name;
			newone->u.structure->type = newone;
			
			//if the structure has a name, build a symbol table item for it.
			SymbolItem newitem = (SymbolItem)malloc(sizeof(struct SymbolItem_));
			memset(newitem, 0, sizeof(struct SymbolItem_));
			
			strcpy(newitem->name, struct_name);
			//printf("type kind:%p\n", newone->u.structure->type);
			newitem->SymbolType = newone;
			newitem->kind = STRUCTNAME;
			newitem->lineno = spe_root->children[1]->children[0]->lineno;
			
			//printf("newitem symbol kind : %i, newone's kind : %i\n", newitem->SymbolType->kind, newone->kind);
			
			int rec = SymbolTable_Add(symbol_table, newitem);
			if(rec == 2)//error type 16
			{
				printf("Error type 16 at Line %i: Duplicated structure name \" %s \".\n", newitem->lineno, newitem->name);
			}
		}
	}
	else if (strcmp(spe_root->children[1]->name, "Tag") == 0)//StructSpecifier, use a structure already declared.
	{
		//printf("tag.\n");
		Syntax_Leaf* tag = spe_root->children[1];
		HashItem rechash = SymbolTable_Find(symbol_table, tag->children[0]->content);
		if(rechash == NULL)//error type 17
		{
			printf("Error type 17 at Line %i: Undefined structure \" %s \",\n", tag->lineno, tag->children[0]->content);
			//return a special basic type
			newone->kind = BASIC;
			newone->u.basic = 2;
		}
		else
		{
			SymbolItem rec = rechash->symbol;
			//printf("rec->name : %s\n", rec->name);
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
void FieldListFillMaster(FieldList field, char* master)
{
	//printf("filling master: %s\n", master);
	while(field != NULL)
	{
		HashItem tmp = SymbolTable_Find(symbol_table, field->name);
		if(tmp == NULL)
		{
			printf("field not in symbol table.\n");
			return;
		}
		tmp->symbol->field_master = master;
		field = field->next;
	}
}

		 

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
		//printf("deflist_type:%i\n", deflist_type);
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
		if(root->children[2]->type == INT_)
		{
			array_size = atoi(root->children[2]->content);
		}
		else//error 12
		{
			printf("Error type 12 at line %i: \" %s \" is not an integer.\n", root->children[2]->lineno, root->children[2]->content);
			array_size = 1;
		}
		
		newone->u.array.size = array_size;
		
		root = root->children[0];
		cur_type = newone;
	}
	SymbolItem newid = (SymbolItem)malloc(sizeof(struct SymbolItem_));
	memset(newid, 0, sizeof(struct SymbolItem_));
	if(root->children[0]->type == ID_)
	{
		
		if(symbol_table[747]!=NULL)
		{
			printf("what in 747: %s, id : %s\n", symbol_table[747]->symbol->name, root->children[0]->content);
		}
		strcpy(newid->name, root->children[0]->content);
		newid->SymbolType = cur_type;
		newid->lineno = root->children[0]->lineno;

		if(deflist_type == FUNCDECPARAM)//no need to add param in the declaration to symbol table 
		{	
			newid->kind = FUNCDECPARAM;
			return newid;
		}
		if(symbol_table[747]!=NULL)
		{
			printf("what in 747: %s, id : %s\n", symbol_table[747]->symbol->name, root->children[0]->content);
		}
		//printf("add vardec: newid :%s\n", newid->name);
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
		

