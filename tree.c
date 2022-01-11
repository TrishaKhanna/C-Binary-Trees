#include "tree.h"

/*********************
FUNCTIONS FOR NODES:
*********************/

// FUNCTION 1: Creates a Node and then stores its address.
/* 
- This function will memmalloc a new struct Node structure.
- It will memmalloc width bytes of data and save the address in data.
- It will then copy width bytes of data from the parameter src to the address data in the new Node structure.
- It will set the lower and higher addresses in the structure to MEMNULL.
- It should copy the new Node structure into memsys.
- It should copy the address of the new structure into the integer pointed to by node_ptr.
- If either memmalloc fails, it should print an error message to the standard error
  stream and exit.
 */
void attachNode( struct memsys *memsys, int *node_ptr, void *src, unsigned int width )
{
	struct Node new;

	int newData = memmalloc(memsys, width);

	int address = memmalloc(memsys, sizeof(struct Node));

	if(newData == MEMNULL)
	{
		fprintf(stderr, "This memmalloc (newData) has failed.\n");
		exit(0);
	}

	if(address == MEMNULL)
	{
		fprintf(stderr, "This memmalloc (address) has failed.\n");
		exit(0);
	}

	setval(memsys, src, width, newData);

	new.data = newData;
	new.lt =  MEMNULL;
	new.gte = MEMNULL;

	setval(memsys, &new, sizeof(struct Node), address);
	*node_ptr = address;

}


// FUNCTION 2: Attaches a child node to the parent node.
/*
- This function should retrieve the node stored at the address pointed to by node_ptr.
- If direction is less than (greater or equal to) zero, it should use attachNode to create a new node and update the lt (gte) variable in the original node.
- Finally it should update the original node in memsys. 
*/
void attachChild( struct memsys *memsys, int *node_ptr, void *src, unsigned int width, int direction )
{
	struct Node newNode;
	getval(memsys, &newNode, sizeof(struct Node), *node_ptr);

	if(direction < 0)
	{
		attachNode(memsys, &newNode.lt, src, width);
	}

	else
	{
		attachNode(memsys, &newNode.gte, src, width);
	}

	setval(memsys, &newNode, sizeof(struct Node), *node_ptr);
}


// FUNCTION 3: Compares the data stored in a variable to the data in a node.
/* 
- This function returns the value returned by the function pointed to by the compar function pointer, when applied to the
  data stored at target and in memsys at the address given by the data variable in the structure at the address
  that node_ptr points to (in that order).
 */
int comparNode( struct memsys *memsys, int *node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width )
{
	int val = 0;
	void * ptr;
	ptr = malloc(width);

	struct Node temp;
	getval (memsys, &temp, sizeof(struct Node), *node_ptr);

	getval (memsys, ptr, width, temp.data);

	val = compar(target, ptr);

	free(ptr);
	return val;

}


// FUNCTION 4: Determines the next node in the tree to visit.
/* 
- This function returns the address of the lt node, or the address of the gte node for the node whose address is pointed to by
  node_ptr, depending on whether direction is less than zero, or greater than or equal to zero.
*/
int next( struct memsys *memsys, int *node_ptr, int direction )
{
	struct Node temp;

	getval(memsys, &temp, sizeof(struct Node), *node_ptr);

	if(direction < 0)
	{
		return temp.lt;
	}

	else
	{
		return temp.gte;
	}
}


// FUNCTION 5: This function copies the data from a node in the tree into the dest void pointer.
/*
- If node_ptr is empty, this function prints an error message to the standard error stream and exits.
- Otherwise, this function will copy width bytes of data from the data address in the node whose address is pointed to by node_ptr,
  into dest.  
*/
void readNode( struct memsys *memsys, int *node_ptr, void *dest, unsigned int width )
{
	if(*node_ptr ==  MEMNULL)
	{
		fprintf(stderr, "readNode - This *node_ptr is empty.\n");
		exit(0);
	}

	else
	{
		struct Node temp;
		getval (memsys, &temp, sizeof(struct Node), *node_ptr);
		getval (memsys, dest, width, temp.data);
	}
}


/* (Remove a leaf node from the tree.)
- If node_ptr is empty, it should print an error message to the standard error stream and exit.
- It should update the integer pointed to by node_ptr to be MEMNULL, and memfree the node structure and its data that used to
  be in the tree.
- If the node has child nodes, they can be ignored and will be “lost” in memsys.
- In other words, this function should never be called on a node with children.
 */
// FUNCTION 6: Removes a leaf node from the tree.
void detachNode( struct memsys *memsys, int *node_ptr )
{
	if(*node_ptr ==  MEMNULL)
	{
		fprintf(stderr, "This detachNode - (*node_ptr) is empty.\n");
		exit(0);
	}

	else
	{
		struct Node newNode;

		getval(memsys,&newNode, sizeof(struct Node), *node_ptr);

        memfree(memsys, *node_ptr);
		memfree(memsys, newNode.data);
		*node_ptr = MEMNULL;
	}
}


// FUNCTION 7: Frees all the nodes including and below node_ptr.
/*
- This function identifies the two children of the node pointed to by node_ptr using the next function.
- It calls freeNodes recursively on each child and finally implements detachNode on the original node.
- If any nodes are missing (as indicated by MEMNULL), then they will not be freed or detached. 
*/
void freeNodes( struct memsys *memsys, int *node_ptr )
{
	if(*node_ptr ==  MEMNULL)
	{
		return;
	}

	int directionL;
	int directionR;
	int temp;
	temp = *node_ptr;
	directionL = next(memsys, &temp, -1 );
	freeNodes(memsys, &directionL);

	directionR = next(memsys, &temp, 1 );
	freeNodes(memsys, &directionR);

	detachNode (memsys, node_ptr);

}


/*********************
FUNCTIONS FOR TREES:
*********************/


// FUNCTION 1: This function creates a new tree.
/* 
- It uses a regular malloc to allocate memory for the struct Tree.
- If the malloc command fails, it prints an error to stderr and exits.
- The list structure is initialized with the given width and a root equal to MEMNULL.
- This function then returns a pointer to the structure. 
*/
struct Tree *newTree( struct memsys *memsys, unsigned int width )
{
	struct Tree * new;
	new = malloc(sizeof(struct Tree));

	if(new == NULL)
	{
		printf("newTree -- This malloc has failed.\n");
		exit(0);
	}

	else
	{
		new->width = width;
		new->root = MEMNULL;
		return new;
	}
}


// FUNCTION 2: This function frees the tree.
/* 
- It does so by calling freeNodes on the root of the tree and then freeing the tree structure itself.
*/
void freeTree( struct memsys *memsys, struct Tree *tree )
{
	freeNodes(memsys, &tree->root);
	free(tree);
}


// FUNCTION 3: Adds an item to the tree at the appropriate spot.
/*
- If the tree is empty this function calls attachNode to add a root node to the tree.
- Otherwise, this function uses a loop that moves through the nodes of the tree, using the comparNode function
  and the next function.
- When it reaches an empty pointer it goes back one node and uses the attachChild function to add a new node. 
*/
void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *src )
{
  if(tree->root == MEMNULL)
	{
		attachNode(memsys, &(tree->root), src, sizeof(struct Node));
	}

	else
	{
		int temp1 = tree->root;
		int temp2 = 0;
		int returnVal = 0;
		while(temp1 != MEMNULL)
		{

			temp2 = temp1;
			returnVal = comparNode (memsys, &temp1, compar, src, tree->width);
			temp1 = next (memsys, &temp1, returnVal);

		}
		attachChild(memsys, &temp2, src, tree->width, returnVal);
	}
}


/*******************************************
ADDITIONAL FUNCTIONALITY -- SEARCH FUNCTION:
*******************************************/


// ADDITIONAL FUNCTIONALITY: Searches for a node within the tree
/* 
- It does so by performing a binary search in the tree. 
- Once the target node is found, its data will be copied to target (replacing the original search term). 
- This function uses comparNode, readNode and next. 
- If a node is found, it returns 1, otherwise it returns 0.  
*/
int searchItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *target )
{
    int root = tree->root;
	int direction;

	while(root != MEMNULL)
	{
		direction = comparNode(memsys, &root, compar, target, tree->width);

		if(direction == 0)
		{
			readNode(memsys, &root, target, tree->width);
			return 1;
		}

		root = next(memsys, &root, direction);
	}

	return 0;
}
