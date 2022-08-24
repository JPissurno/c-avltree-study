#ifndef __GNUC__
	#error This library requires GCC compiler for it uses typeof() function
#endif
#if __STDC_VERSION__ < 201112L
	#error This library requires C11 or newer for it uses _Generic() function
#endif



#ifndef __AVL_TREE__
#define __AVL_TREE__



#include <string.h>
#include <stdlib.h>





/**	@Description
 *		This structure is a node of a generic avl tree,
 *		so ID and data shall be heap allocated.
 *
 *		It may store data from stack as long as all tree
 *		operations are contained inside the same function scope,
 *		and does not need to return to some caller, since stack
 *		data from functions is discarted after its completion.
 *
 *	@Members
 *		void* ID:						a pointer to any kind of primitive variable,
 *										i.e. int & variations, unsigned int & variations,
 *										float & variations, or string (char*).
 *										It's used as a comparator within this library's
 *										functions, to search for and retrieve the data;
 *					
 *		void* data:						a pointer to any kind of data, i.e. primitives,
 *										structures, pointers, etc. While ID is to identify
 *										the node so it's possible to search for it, data
 *										refers to the data itself;
 *					
 *		char isLeaf:					0 if node is not a leaf, 1 otherwise;
 *				
 *		charbalance:					current balance of the node. Used to reorganize the tree
 *										and maintain O(log(n)) operation cost;
 *
 *		struct AVLtree_sub* Lchild:		pointer to node's left child. NULL if it hasn't one;
 *
 *		struct AVLtree_sub* Rchild:		pointer to node's right child. NULL if it hasn't one;
 *
 *		struct AVLtree_sub* parent:		pointer to node's parent. Points to itself if it's the root.
 *		
 */
struct AVLtree_sub{
	
	void *ID;
	void *data;
	char isLeaf;
	char balance;
	struct AVLtree_sub *Lchild;
	struct AVLtree_sub *Rchild;
	struct AVLtree_sub *parent;
	
};


/**	@Description
 *		This structure is the super avl tree that has
 *		a pointer to a root of each main primitive type.
 *
 *		It's an abstraction that ensures all nodes of
 *		one root have the same ID primitive, so functions
 *		can properly compare node's ID members to find a node.
 *
 *	@Members
 *		struct AVLtree_sub* int_root:		a pointer to an avl tree that holds only
 *											int & variations (char, short, long, long long)
 *											primitives for ID;
 *	
 *		struct AVLtree_sub* uint_root:		a pointer to an avl tree that holds only
 *											unsigned int & variations (char, short, long, long long)
 *											primitives for ID;
 *
 *		struct AVLtree_sub* double_root:	a pointer to an avl tree that holds only
 *											float & variations (double, long double)
 *											primitives for ID;
 *
 *		struct AVLtree_sub* string_root:	a pointer to an avl tree that holds only
 *											strings (char*) for ID.
 *
 */
struct AVLtree{
	
	struct AVLtree_sub *int_root;
	struct AVLtree_sub *uint_root;
	struct AVLtree_sub *double_root;
	struct AVLtree_sub *string_root;
	
};





/**	@Functionality
 *		Creates a super avl tree allocated on heap, a
 *		pointer to a struct AVLtree, and prepares its
 *		roots: allocates them into heap, sets them to
 *		leaves and sets their parents to themselves.
 *
 *		The tree must be freed afterwards using
 *		avl_free() function.
 *
 *	@Argument
 *		None
 *
 *	@Return
 *		Unconditionally:	a pointer to an AVLtree structure,
 *							a super avl tree, allocated on heap,
 *							ready to use
 *
 */
struct AVLtree* avl_createTree(void);



/**	@Functionality
 *		Removes a node from a super avl tree,
 *		freeing its ID and data members, as well as itself.
 *
 *		This is a helper function of avl_remove() macro function.
 *
 *	@Arguments
 *		struct AVLtree* tree:		a pointer to an AVLtree structure, a super avl tree,
 *									properly created with avl_createTree() function;
 *
 *		struct AVLtree_sub* node:	which node to remove from whichever root type
 *									of the super avl tree.
 *
 *	@Return
 *		None
 *
 */
void avl_removeNode(struct AVLtree* tree, struct AVLtree_sub* node);



/**	@Functionality
 *		Checks if node has left or right child and, if not,
 *		allocates one on heap and configures it to that node.
 *
 *		This is a helper function of avl_forward() macro function.
 *
 *	@Arguments
 *		struct AVLtree_sub* parent:	a pointer to an avl tree node;
 *
 *		char c_type:				child type to check for node ('r' if right, 'l' if left).
 *
 *	@Return
 *		None
 *
 */
void avl_checkChild(struct AVLtree_sub* parent, char c_type);



/**	@Functionality
 *		Recursively gets all IDs and datas from a
 *		starting node of an avl tree, normally a root,
 *		inside a heap allocated ID and data arrays.
 *
 *		It first gets all left children, then all
 *		right children ID and data of a node,
 *		then the node's ID and data themselves.
 *
 *		ID and data must be freed by the caller, only
 *		the arrays, not their contents (free(ID) and
 *		free(data)), since their contents just point
 *		to those of the nodes, it's not a copy on heap.
 *
 *		This is a helper function of avl_traverse() macro function
 *
 *	@Arguments
 *		void*** ID:					pointer to a matrix of void pointers, on heap,
 *									so it's possible to resize it internally. This
 *									will have all IDs pointers inside an avl tree;
 *
 *		void*** data:				pointer to a matrix of void pointers, on heap,
 *									so it's possible to resize it internally. This
 *									will have all datas pointers inside an avl tree;
 *
 *		int* c:						a pointer to an int, which will count how many
 *									nodes there are in an avl tree;
 *
 *		struct AVLtree_sub* node:	a pointer to an avl tree node, normally a root.
 *
 *	@Return
 *		None
 *
 */
void avl_tTraverse(void*** ID, void*** data, long* c, struct AVLtree_sub* node);



/**	@Functionality
 *		Frees all data from an AVLtree structure,
 *		a super avl tree, namely: all nodes' ID
 *		and data. Also calls its helper function,
 *		avl_freeNode(), for each root, then frees
 *		the AVLtree structure itself.
 *
 *		If data is not a primitive type or string
 *		and it has some members on heap, one shall
 *		call avl_traverse function to get said members
 *		and free them manually.
 *
 *		For example: a structure with two string members,
 *		each heap allocated. In that case, there'll be
 *		three variables on heap: the two strings and
 *		the structure itself. If the avl_free function
 *		is called without the avl_traverse function
 *		and manual free of the two members beforehand,
 *		the two strings will be leaked.
 *
 *	@Argument
 *		struct AVLtree* tree:	a pointer to an AVLtree structure,
 *								the super avl tree to be freed.
 *
 *	@Return
 *		None
 *
 */
void avl_free(struct AVLtree* tree);



/**	@Functionality
 *		Frees node's ID and data, calling this
 *		function recursively to its children,
 *		and then frees the node itself.
 *
 *		This is a helper function of avl_free() function.
 *
 *	@Argument
 *		struct AVLtree_sub* node:	a pointer to an avl tree node,
 *									to be freed, as well as all of
 *									its children.
 *
 *	@Return
 *		None
 *
 */
void avl_freeNode(struct AVLtree_sub* node);





/**	@Functionality
 *		Returns respective super avl tree's
 *		root structure pointer, according
 *		to 'id' primitive type.
 *
 *		It needs at least C11 to work, for it uses
 *		_Generic() function.
 *
 *		It is a macro function because otherwise,
 *		'id' would need to be a void* to support a
 *		generic type when calling the function,
 *		and so _Generic() would not work to
 *		distinguish which root it should return.
 *
 *	@Arguments
 *		struct AVLtree* sAVL:	a pointer to an AVLtree structure, a super avl tree,
 *								properly created with avl_createTree() function;
 *
 *		? id:					a variable which primitive type identifies
 *								to which root type it refers.
 *
 *	@Returns
 *		On success:	a pointer to one of super avl tree's root,
 *					depending on 'id' primitive type:	int_root	-> 	int, short int,
 *																		char, signed char,
 *																		long, long long;
 *
 *														uint_root	->	unsigned int, unsigned short int,
 *																		unsigned char, unsigned long,
 *																		unsigned long long;
 *
 *														double_root	->	float, double, long double;
 *
 *														string_root ->	char*;
 *
 *		On failure:	NULL (if 'id' is not a primitive type)
 *
 */
#define avl_getRootType(sAVL, id) 																		\
		_Generic((id),	int:			sAVL->int_root,		unsigned int:			sAVL->uint_root,	\
						char:			sAVL->int_root,		unsigned char:			sAVL->uint_root,	\
						long:			sAVL->int_root,		unsigned long:			sAVL->uint_root,	\
						long long:		sAVL->int_root,		unsigned long long:		sAVL->uint_root,	\
						short int:		sAVL->int_root,		unsigned short int:		sAVL->uint_root,	\
						signed char: 	sAVL->int_root,		char*:					sAVL->string_root,	\
						double: 		sAVL->double_root,	long double:			sAVL->double_root,	\
						float:			sAVL->double_root,	default:				NULL				\
				)





/**	@Functionality
 *		Compares some identifier, 'id', with
 *		node's ID. If it's greater, auxiliary
 *		node points to current node's right child,
 *		otherwise points to current node's left child.
 *		If the root is a string root, uses strcmp() function
 *		for comparison.
 *
 *		It is a macro function because otherwise,
 *		'id' would need to be a void* to support a
 *		generic type when calling the function,
 *		and so typeof() function would not work to
 *		dinamically cast node's ID to id's type.
 *
 *		This is a helper function of avl_insert() macro function.
 *
 *	@Arguments
 *		struct AVLtree_sub* node:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root's node, an avl tree;
 *
 *		? id:						identifier to search for in the avl tree,
 *									such as 5, or "scarf";
 *
 *		char type:					to identify what root type it refers to.
 *									'c' to string root, 'a' otherwise.
 *
 *	@Return
 *		None, since it's a macro function, but alters what
 *		node argument points to, when called
 *
 */
#define avl_forward(node, id, type)								\
		do {													\
																\
			/* If id is a string, uses strcmp for comparison */	\
			if(type == 'c'){									\
				if(strcmp(id, (char*)node->ID) > 0){			\
					avl_checkChild(node, 'r');					\
					node = node->Rchild;						\
				} else {										\
					avl_checkChild(node, 'l');					\
					node = node->Lchild;						\
				}												\
				break;											\
			}                                               	\
																\
																\
			/* 	If id is not a string, casts node ID to id's	\
				type and compares it directly */				\
			if(id > *(typeof(id)*)node->ID){               		\
				avl_checkChild(node, 'r');                  	\
				node = node->Rchild;                        	\
			} else {                                        	\
				avl_checkChild(node, 'l');                  	\
				node = node->Lchild;                        	\
			}                                               	\
																\
																\
		} while(0)





/**	@Functionality
 *		Inserts identifier and data into an avl tree.
 *		The identifier is copied into avl tree, but
 *		data member just points to data passed as
 *		argument.
 *
 *		Identifier may be stack allocated, but data must be
 *		heap allocated and shall not be freed anywhere else.
 *
 *		It is a macro function because otherwise,
 *		'id' would need to be a void* to support a
 *		generic type when calling the function,
 *		and so typeof() function would not work to
 *		dinamically cast node's ID to id's type.
 *
 *	@Arguments
 *		struct AVLtree_sub* root:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root, an avl tree;
 *
 *		? DATA:						pointer to the data to be stored into
 *									the avl tree. Must be heap allocated
 *									and shall not be freed anywhere else
 *									apart from avl_free() function;
 *
 *		? id:						identifier used to search for this node's
 *									data inside the avl tree, such as 5, or
 *									"scarf". It may be stack allocated, since
 *									it's copied and heap allocated internally.
 *
 *	@Return
 *		None
 *
 */
#define avl_insert(root, DATA, id)										\
		do {															\
																		\
			/* Gets super avl tree's root depending on id */			\
			char type;													\
			struct AVLtree_sub *node;									\
			if(!(node = avl_getRootType(root, id))) break;				\
																		\
																		\
			/* 	Runs through the root until								\
				it finds a node thas is a leaf	*/						\
			type = (root->string_root == node) ? 'c' : 'a';				\
			while(!node->isLeaf) avl_forward(node, id, type);			\
																		\
																		\
			/* Copies id content into node's ID, heap allocated */		\
			if(type == 'c')												\
				node->ID = strcpy(calloc(strlen(id)+1, 1), id);			\
			else {														\
				node->ID = calloc(1, sizeof(typeof(id)));				\
				memcpy(node->ID, id, sizeof(typeof(id)));				\
			}															\
																		\
																		\
			/* 	Node's data now points to DATA, this 					\
				function's argument, and it's not a leaf anymore */		\
			node->data = DATA;                                    		\
			node->isLeaf = 0;                                     		\
																		\
																		\
		} while(0)





/**	@Functionality
 *		Compares 'id' to node's ID: if it's bigger,
 *		sets current node auxiliary pointer to point
 *		to its right child, if it's lesser, points to
 *		its left child, if it's equal, set DATA to point
 *		to node's data.
 *
 *		It is a macro function because otherwise,
 *		'id' would need to be a void* to support a
 *		generic type when calling the function,
 *		and so typeof() function would not work to
 *		dinamically cast node's ID to id's type.
 *
 *		This is a helper function of avl_search()
 *		and avl_remove() macro functions.
 *
 *	@Arguments
 *		struct AVLtree_sub* node:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root's node, an avl tree;
 *
 *		void* DATA:					a void pointer that will point to node's
 *									data;
 *
 *		? id:						the identifier to compare to that of the node;
 *
 *		char type:					to identify what id type it refers to.
 *									'c' to string id, 'a' otherwise.
 *
 *	@Return
 *		None, since it's a macro function, but alters what
 *		node argument points to, or DATA, when called
 *		
 */
#define avl_compare(node, DATA, id, type)								\
		do {															\
																		\
			/* Returns if node doesn't have ID */						\
			if(!node->ID){                                              \
				node = NULL;                                            \
				break;                                                  \
			}                                                           \
																		\
																		\
			/* Uses strcmp to compare, if id is a string */				\
			if(type == 'c'){                                            \
				char eval = strcmp(id, (char*)node->ID);				\
																		\
				if(eval > 0) node = node->Rchild;                       \
				else if(eval < 0) node = node->Lchild;                  \
				else DATA = node->data;                                 \
																		\
				break;                                                  \
			}                                                           \
																		\
																		\
			/* Casts ID to id's type and compares directly */			\
			if(id > *(typeof(id)*)node->ID)                             \
				node = node->Rchild;                                    \
			else if(id < *(typeof(id)*)node->ID)                        \
				node = node->Lchild;                                    \
			else if(id == *(typeof(id)*)node->ID)                       \
				DATA = node->data;                                      \
																		\
																		\
		} while(0)





/**	@Functionality
 *		Searches 'id' into one of the super avl tree's
 *		roots, an avl tree, and retrives its data if
 *		it finds it.
 *
 *		It is a macro function because otherwise,
 *		'id' would need to be a void* to support a
 *		generic type when calling the function,
 *		and so avl_getRootType() would not work to
 *		distinguish which root it should return.
 *
 *	@Arguments
 *		struct AVLtree_sub* root:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root, an avl tree;
 *
 *		? id:						the identifier to search for into the
 *									avl tree;
 *
 *		? DATA:						a pointer to the data type expected to be
 *									retrieved.
 *
 *	@Return
 *		None, since it's a macro function, but alters what
 *		DATA argument points to, when called
 *
 */
#define avl_search(root, id, DATA)										\
		do {                                                            \
																		\
			/* Gets super avl tree's root depending on id */			\
			char type;													\
			DATA = NULL;                                                \
			struct AVLtree_sub *node;                                 	\
			if(!(node = avl_getRootType(root, id))) break;            	\
																		\
																		\
			/* Calls avl_compare until DATA is found, or node is NULL */\
			type = (root->string_root == node) ? 'c' : 'a';				\
			while(!DATA && node)										\
				avl_compare(node, DATA, id, type);						\
																		\
																		\
		} while(0)





/**	@Functionality
 *		Gets all IDs and datas currently into an avl tree,
 *		normally starting from the root, into ID and data
 *		arrays, heap allocated.
 *
 *		Since they're heap allocated, they must be freed
 *		by the caller, but only the arrays, not their
 *		content, because their content just point to those
 *		of the nodes, they're not copied and heap allocated.
 *
 *		It is a macro function because otherwise, 'type'
 *		would need to be a void* to support a generic type
 *		when calling the function, and so avl_getRootType()
 *		would not be able to distinguish which root it should return.
 *
 *	@Arguments
 *		struct AVLtree_sub* root:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root, an avl tree;
 *
 *		void*** ID:					a void triple pointer that will store
 *									all node's IDs;
 *
 *		void*** data:				a void triple pointer that will store
 *									all node's datas;
 *
 *		int* counter:				a pointer to an int, that will keep
 *									track of how many nodes there are
 *									in the avl tree;
 *
 *		? type:						a primitive type to identify which
 *									root of the avl tree to traverse.
 *
 *	@Return
 *		None, since it's a macro function, but populates
 *		both ID and data arrays with IDs and datas from
 *		the avl tree, reallocing them on heap, when necessary
 *
 */
#define avl_traverse(root, ID, data, counter, type)							\
		do {																\
																			\
			/* 	Zeroes counter and allocates memory							\
				for ID and data arrays */									\
			*counter = 0;													\
			*ID = calloc(1, sizeof(void*));									\
			*data = calloc(1, sizeof(void*));								\
																			\
																			\
			/* 	Traverses through avl tree, getting							\
				node's ID and datas */										\
			avl_tTraverse(ID, data, counter, avl_getRootType(root, type));	\
																			\
																			\
		} while(0)





/**	@Functionality
 *		Searches for 'id' into root and removes
 *		it, if found, freeing its ID and data,
 *		as well as the node itself.
 *
 *	@Arguments
 *		struct AVLtree_sub* root:	pointer to an AVLtree_sub structure, one
 *									of super avl tree's root, an avl tree;
 *
 *		? id:						the identifier to search for into the
 *									avl tree and remove;
 *
 *	@Return
 *		None
 *
 */
#define avl_remove(root, id)											\
		do {                                                            \
																		\
			/* Gets super avl tree's root depending on id */			\
			char type;													\
			void *DATA = NULL;                                          \
			struct AVLtree_sub *node;                                 	\
			if(!(node = avl_getRootType(root, id))) break;            	\
																		\
																		\
			/* Calls avl_compare until DATA is found, or node is NULL */\
			type = (root->string_root == node) ? 'c' : 'a';				\
			while(!DATA && node) avl_compare(node, DATA, id, type);		\
																		\
																		\
			/* If node is found, removes it */							\
			if(!node) break;                                          	\
			avl_removeNode(root, node);									\
																		\
																		\
		} while(0)



#endif