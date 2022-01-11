#ifndef _TREE_H
#define _TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memsys.h"
#endif


struct Node
{
	int data;
	int lt;
	int gte;
};

struct Tree
{
	unsigned int width;
    int root;
};

/*********************
FUNCTIONS FOR NODES:
*********************/

void attachNode( struct memsys *memsys, int *node_ptr, void *src, unsigned int width );
void attachChild( struct memsys *memsys, int *node_ptr, void *src, unsigned int width, int direction );
int comparNode( struct memsys *memsys, int *node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width );
int next( struct memsys *memsys, int *node_ptr, int direction );
void readNode( struct memsys *memsys, int *node_ptr, void *dest, unsigned int width );
void detachNode( struct memsys *memsys, int *node_ptr );
void freeNodes( struct memsys *memsys, int *node_ptr );

/*********************
FUNCTIONS FOR TREES:
*********************/

struct Tree *newTree( struct memsys *memsys, unsigned int width );
void freeTree( struct memsys *memsys, struct Tree *tree );
void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *src );

/*****************
SEARCH FUNCTION:
*****************/

int searchItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *target );
