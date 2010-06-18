
/*
 * This file contains the path struct and it's functions.
 * It is used to keep an ant path. Nothing more than a binary tree
 */


struct path{
	struct path_tree * left;
	struct path_tree * right;
	unsigned short int value;
};

struct path * new_path(int value){
	struct path * p;
	p = (struct path *)malloc(sizeof(struct path));
	p->left = NULL;
	p->right = NULL;
	p->value = value;
	return p;
}

void destroy_path(struct path * p){
	if( p != NULL){
		if( p->left != NULL)
			destroy_path(p->left);
		if(p->right != NULL)
			destroy_path(p->right);
		free(p);
	}
}

void add_to_path(struct path * p, unsigned short int v){
	if( p == NULL )
		critical_error("Adding value to NULL tree");
	if(v == p->value)
		critical_error("Adding two times same value to tree");
	if(v < p->value)
		if(p->left == NULL)
			p->left = new_path(v)
		else
			add_to_path(p->left,v);
	if (v > p->value)
		if(p->right == NULL)
			p->right = new_path(v);
		else
			add_to_path(p->right);
}

//returns 0 (false) if not found, or 1(true) if found
int in_path(struct path * p, unsigned short int value){
	if(p != NULL){
		if ( (p->value == value) || (in_path(p->left) == 1) ||
					    (in_path(p->right) == 1)) 
			return 1;
		else
			return 0;
	}
	return 0;
}


void test_path(){
	struct path * a = new_path(1);
}








