#ifndef _TREE_H
#define _TREE_H
enum{TOKEN_, LEX_, ID_, TYPE_, INT_, FLOAT_};
struct Syntax_Leaf
{
	char* name;
	int lineno;
	int type;
		
	int childrennum;
	struct Syntax_Leaf* children[7];
	struct Syntax_Leaf* father;
	char content[32];
};
typedef struct Syntax_Leaf Syntax_Leaf;

int tree_error;

struct Error_Node
{
	char* name;
	int type;
	struct Error_Node* next;
};
typedef struct Error_Node Error_Node;

Syntax_Leaf* Syntax_Root;
Syntax_Leaf* Syntax_Leaf_New(char* Name, int LineNo, int Type);
void Syntax_Tree_Print(Syntax_Leaf* root);

#endif
