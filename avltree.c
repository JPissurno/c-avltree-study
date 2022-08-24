#include "avltree.h"



struct AVLtree* avl_createTree(void){
	
	/* Creates a super avl tree */
	struct AVLtree *tree = calloc(1, sizeof(struct AVLtree));
	
	
	/* Creates all root types for that tree on heap */
	tree->int_root		= calloc(1, sizeof(struct AVLtree_sub));
	tree->uint_root		= calloc(1, sizeof(struct AVLtree_sub));
	tree->double_root	= calloc(1, sizeof(struct AVLtree_sub));
	tree->string_root	= calloc(1, sizeof(struct AVLtree_sub));
	
	
	/* Sets all roots to leaves */
	tree->int_root->isLeaf		= 1;
	tree->uint_root->isLeaf		= 1;
	tree->double_root->isLeaf	= 1;
	tree->string_root->isLeaf	= 1;
	
	
	/* Since they're roots, their parents are themselves */
	tree->int_root->parent		= tree->int_root;
	tree->uint_root->parent 	= tree->uint_root;
	tree->double_root->parent	= tree->double_root;
	tree->string_root->parent	= tree->string_root;
	
	
	/* Returns configured tree */
	return tree;
	
}



void avl_checkChild(struct AVLtree_sub* parent, char c_type){
	
	/* 	Allocates and configures a right child to parent, on heap,
		if it's to check for it and if parent doesn't have one */
	if(c_type == 'r' && !parent->Rchild){
		parent->Rchild = calloc(1, sizeof(struct AVLtree_sub));
		parent->Rchild->isLeaf = 1;
		parent->Rchild->parent = parent;
		return;
	}
	
	
	
	/* 	Allocates and configures a left child to parent, on heap,
		if it's to check for it and if parent doesn't have one */
	if(c_type == 'l' && !parent->Lchild){
		parent->Lchild = calloc(1, sizeof(struct AVLtree_sub));
		parent->Lchild->isLeaf = 1;
		parent->Lchild->parent = parent;
	}
	
}



void avl_tTraverse(void*** ID, void*** data, long* c, struct AVLtree_sub* node){
	
	/* Base case: returns if there's no node, or there's no ID nor data inside node */
	if(!node) return;
	if(!node->ID && !node->data) return;
	
	
	/* Recursively goes through node's children */
	if(node->Lchild) avl_tTraverse(ID, data, c, node->Lchild);
	if(node->Rchild) avl_tTraverse(ID, data, c, node->Rchild);
	
	
	/* Pointer array at index c now points to current node's ID and data */
	(*ID)[(*c)] = node->ID;
	(*data)[(*c)] = node->data;
	
	
	/* Increase counter and reallocates ID and data arrays */
	(*c)++;
	(*ID) = realloc((*ID), ((*c)+1)*sizeof(void*));
	(*data) = realloc((*data), ((*c)+1)*sizeof(void*));
	
}



void avl_removeNode(struct AVLtree* tree, struct AVLtree_sub* node){
	
	struct AVLtree_sub* aux;
	
	
	/* Frees data and ID, if it's not equal to data */
	if(node->ID != node->data)
		free(node->ID);
	free(node->data);
	node->data = NULL;
	node->ID = NULL;
	
	
	
	/* If node has both children: swaps current node with biggets under it and frees it */
	if(node->Lchild && node->Rchild){
		
		/* Gets node's biggest left child */
		struct AVLtree_sub *i_node = node->Lchild;
		while(1){
			if(i_node->Rchild) i_node = i_node->Rchild;
			else break;
		}
		
		
		/* Sets node's ID and data to point to those of the biggets left child */
		node->ID = i_node->ID;
		node->data = i_node->data;
		
		
		/* */
		if(i_node->parent->Lchild == i_node){
			
			if(i_node->Lchild){
				
				i_node->Lchild->parent = i_node->parent;
				i_node->parent->Lchild = i_node->Lchild;
				
			} else i_node->parent->Lchild = NULL;
			
		} else if(i_node->Lchild) {
			
			i_node->Lchild->parent = i_node->parent;
			i_node->parent->Rchild = i_node->Lchild;
			
		} else i_node->parent->Rchild = NULL;
		
		
		/* Frees node's biggets left child and returns */
		free(i_node);
		i_node = NULL;
		return;

	}
	
	
	
	/* If node has no children */
	if(!node->Lchild && !node->Rchild){
		
		/* If it's the root, simply turns it into a leaf again and returns */
		if(node->parent == node){
			node->isLeaf = 1;
			return;
		}
		
		
		/* Set its parent to point its child to NULL */
		if(node->parent->Lchild == node) node->parent->Lchild = NULL;
		else node->parent->Rchild = NULL;
		
		
		/* Frees node and returns */
		free(node);
		node = NULL;
		return;
		
	}
	
	
	
	/* Then node has either left or right child */
	if(node->Lchild){
		
		/* */
		if(node->parent == node){
			node->Lchild->parent = node->Lchild;
			aux = node->Lchild;
		} else node->Lchild->parent = node->parent;
	
		/* */
		if(node->parent->Lchild == node)
			node->parent->Lchild = node->Lchild;
		else node->parent->Rchild = node->Lchild;
		
		
	} else if(node->Rchild) {
		
		/* */
		if(node->parent == node){
			node->Rchild->parent = node->Rchild;
			aux = node->Rchild;
		} else node->Rchild->parent = node->parent;
		
		/* */
		if(node->parent->Lchild == node)
			node->parent->Lchild = node->Rchild;
		else node->parent->Rchild = node->Rchild;

	}
	
	
	/* If it was the root, points now to its left or right child */
	if(tree->int_root == node) tree->int_root = aux;
	else if(tree->uint_root == node) tree->uint_root = aux;
	else if(tree->double_root == node) tree->double_root = aux;
	else if(tree->string_root == node) tree->string_root = aux;
	
	
	/* Frees node */
	free(node);
	node = NULL;
	
}



void avl_free(struct AVLtree* tree){
	
	/* Calls helper function to free each root */
	avl_freeNode(tree->int_root);
	avl_freeNode(tree->uint_root);
	avl_freeNode(tree->double_root);
	avl_freeNode(tree->string_root);
	
	
	/* Frees the super tree iteslf */
	free(tree);
	tree = NULL;
	
}



void avl_freeNode(struct AVLtree_sub* node){
	
	/* Recursively calls itself to free its children */
	if(node->Rchild) avl_freeNode(node->Rchild);
	if(node->Lchild) avl_freeNode(node->Lchild);
	
	
	/* Frees data and ID, if it's not equal to data */
	if(node->ID != node->data)
		free(node->ID);
	free(node->data);
	node->data = NULL;
	node->ID = NULL;
	
	
	/* Frees the node itself */
	free(node);
	node = NULL;
	
}