## Binary Trees
**Created a series of functions in C to learn how to better work with binary trees**

##### Function Descriptions:
###### FUNCTIONS FOR NODES:
a) void attachNode( struct memsys *memsys, int *node_ptr, void *src, unsigned int width );
- This function will memmalloc a new struct Node structure.
- It will memmalloc width bytes of data and save the address in data.
- It will then copy width bytes of data from the parameter src to the address data in the new Node structure.
- It will set the lower and higher addresses in the structure to MEMNULL.
- It should copy the new Node structure into memsys.
- It should copy the address of the new structure into the integer pointed to by node_ptr.
- If either memmalloc fails, it should print an error message to the standard error
  stream and exit.
  
  
b) void attachChild( struct memsys *memsys, int *node_ptr, void *src, unsigned int width, int direction );
- This function should retrieve the node stored at the address pointed to by node_ptr.
- If direction is less than (greater or equal to) zero, it should use attachNode to create a new node and update the lt (gte) variable in the original node.
- Finally it should update the original node in memsys. 


c) int comparNode( struct memsys *memsys, int *node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width );
- This function returns the value returned by the function pointed to by the compar function pointer, when applied to the
  data stored at target and in memsys at the address given by the data variable in the structure at the address
  that node_ptr points to (in that order).
  
  
d) int next( struct memsys *memsys, int *node_ptr, int direction );
- This function returns the address of the lt node, or the address of the gte node for the node whose address is pointed to by
  node_ptr, depending on whether direction is less than zero, or greater than or equal to zero.
  
  
e) void readNode( struct memsys *memsys, int *node_ptr, void *dest, unsigned int width );
- If node_ptr is empty, this function prints an error message to the standard error stream and exits.
- Otherwise, this function will copy width bytes of data from the data address in the node whose address is pointed to by node_ptr,
  into dest.
  
  
f) void detachNode( struct memsys *memsys, int *node_ptr );
- If node_ptr is empty, it should print an error message to the standard error stream and exit.
- It should update the integer pointed to by node_ptr to be MEMNULL, and memfree the node structure and its data that used to
  be in the tree.
- If the node has child nodes, they can be ignored and will be “lost” in memsys.
- In other words, this function should never be called on a node with children.


g) void freeNodes( struct memsys *memsys, int *node_ptr );
- This function identifies the two children of the node pointed to by node_ptr using the next function.
- It calls freeNodes recursively on each child and finally implements detachNode on the original node.
- If any nodes are missing (as indicated by MEMNULL), then they will not be freed or detached. 

###### FUNCTIONS FOR TREES:

a) struct Tree *newTree( struct memsys *memsys, unsigned int width );
- It uses a regular malloc to allocate memory for the struct Tree.
- If the malloc command fails, it prints an error to stderr and exits.
- The list structure is initialized with the given width and a root equal to MEMNULL.
- This function then returns a pointer to the structure. 


b) void freeTree( struct memsys *memsys, struct Tree *tree );
- It does so by calling freeNodes on the root of the tree and then freeing the tree structure itself.


c) void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *src );
- If the tree is empty this function calls attachNode to add a root node to the tree.
- Otherwise, this function uses a loop that moves through the nodes of the tree, using the comparNode function
  and the next function.
- When it reaches an empty pointer it goes back one node and uses the attachChild function to add a new node. 

###### SEARCH FUNCTION:

a) int searchItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *target );
- It does so by performing a binary search in the tree. 
- Once the target node is found, its data will be copied to target (replacing the original search term). 
- This function uses comparNode, readNode and next. 
- If a node is found, it returns 1, otherwise it returns 0.  
