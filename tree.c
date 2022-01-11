#include "tree.h"

/*********************
FUNCTIONS FOR NODES:
*********************/

// FUNCTION 1: Creates a Node and then stores its address.
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
void freeTree( struct memsys *memsys, struct Tree *tree )
{
	freeNodes(memsys, &tree->root);
	free(tree);
}


// FUNCTION 3: Adds an item to the tree at the appropriate spot.
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


/***************
SEARCH FUNCTION:
***************/


// Searches for a node within the tree
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
