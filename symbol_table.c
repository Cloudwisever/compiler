
#define TABLE_SIZE  100

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
		if (i = val & ~0x3fff)
			val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val % TABLE_SIZE;
}

void SymbolTable_Find(HashItem symbol_table, char* name)
{
	int index = hash_pjw(name);
	
 
