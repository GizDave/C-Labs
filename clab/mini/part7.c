#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <assert.h>
 
 #include "part7.h"
 
 /* In part7.c you will implement a sorted binary tree
  A binary tree is a tree in which each node has 
  at most two children (referred to as left and right).
  Each node also holds a user value (which is a C string in 
  this lab).  In a sorted binary tree, for each node n, n's left child's
  value is less than n and n's right child's value is greater than n.
  Binary trees can be complicated if one is to keep them balanced.
  They are much simpler if we don't care about balancing. You will 
  implement the simple, unbalanced binary in this lab.
  Specifically, after inserting "a", "d", "b", "e" (in the order specified),
  your binary tree will look like this simplified representation:
      "a"
         \
 	"d"
 	/ \
      "b"  "e"
 */
 
 static tnode_t *alloc_node();
 tnode_t * init_tree();
 
 // alloc_node allocates a new tree node 
 // and initializes its fields properly.
 // Note: before implementing this function, you must decide what 
 // fields the tree node should contain and complete the 
 // type definition of tnode_t in part7.h
 static tnode_t *
 alloc_node() 
 {
 	tnode_t *newNode=(tnode_t *)malloc(sizeof(tnode_t));
 	newNode->value=NULL;
 	newNode->leftNode=NULL;
 	newNode->rightNode=NULL;
 	return newNode;
 }
 
 // init_tree initializes the tree by allocating its root node and 
 // returning it
 tnode_t *
 init_tree()
 {
 	return alloc_node();
 }
 
 // insert stores a string (s) in the sorted binary tree,
 // specified by its root (tree).
 // It returns 0 if string (s) has been successfully inserted 
 // (i.e. no duplicates)
 // It returns 1 if the string has not been inserted (due to duplicates)
 // Hint: you can use strcmp(..) to compare two NULL-terminated C strings
 // type "man strcmp" to learn how to use it.
 int
 insert(tnode_t *tree, char *s)
 {
 	//assert(tree!=NULL);
 		
 	if(tree->value==NULL){
 		tree->value=s;	
 		return 0;
 	}
 
 	int compare=strcmp(tree->value,s);
 	
 	if(compare==0)
 		return 1;
 	else{
 		tnode_t *tempNode=alloc_node();
 		tempNode->value=s;
 		
 		if(compare>0)
 			if(tree->leftNode!=NULL)
-				insert(tree->leftNode,s);
+				return insert(tree->leftNode,s);
 			else
 				tree->leftNode=tempNode;
 		else
 			if(tree->rightNode!=NULL)
-				insert(tree->rightNode,s);
+				return insert(tree->rightNode,s);
 			else
 				tree->rightNode=tempNode;
 
 		return 0;	
 	}
 	return 1;
 }
 
 // pre-order performs a preorder traversal of the sorted binary tree
 // and store the sequence of strings visited along the way in 
 // the array argument (result) whose allocated size is result_max
 // It returns the number of strings stored in the result array.
 // (Our tester part7_harness.c will allocate an array whose size (result_max) is 
 // larger than the number of strings to be stored.)
 // In preorder traversal, you print the value in node n first, and then 
 // visit n's left child, and then visit n's right child.
 // For example, for the tree below, the preorder traversal result is "a" "d" "b" "e"
 //     "a"
 //         \
 //	   "d"
 //	   /   \
 //    "b"  "e"
 //
 // Note: you are free to write an auxilary function and use it here.
 // (unclear instructions)
 int
 preorder(tnode_t *tree, char **result, int result_max)
 {	
 	if(tree==NULL || tree->value==NULL)
 		return 0;
 	
 	int values=1;
 	
 	*result=(char *) malloc(sizeof(char));
 	*result=tree->value;
 	result++;
 
 	values+=preorder(tree->leftNode, result, result_max);
 	result+=values-1;
 
 	values+= preorder(tree->rightNode, result, result_max);
 	return values;
 }
 
 // inorder performs an inorder traversal of the sorted binary tree
 // and store the sequence of strings visited along the way in 
 // the array argument (result) whose allocated size is result_max
 // It returns the number of strings stored in the result array.
 // (Our tester part7_harness.c will allocate an array whose size (result_max) is 
 // larger than the number of strings to be stored.)
 // In inorder traversal, you visit node n's left child first, then print the value in node n, and then 
 // visit node n's right child.
 // For example, for the tree below, the inorder traversal result is "a" "b" "d" "e"
 //     "a"
 //       \
 //	"d"
 //	/ \
 //    "b"  "e"
 //
 // Note: you are free to write an auxilary function and use it here.
 // (What duplicates?)
 int
 inorder(tnode_t *tree, char **result, int max_result)
 {
 	if(tree==NULL || tree->value==NULL)
 		return 0;
 	
 	int values=inorder(tree->leftNode, result, max_result);
 	result+=values;
 
 	*result=(char *) malloc(sizeof(char));
 	*result=tree->value;
 	result++;
 	values++;
 
 	values+=inorder(tree->rightNode, result, max_result);
 	return values;
 }
 
 // del_tree de-allocates all nodes in the tree
 // you should not de-allocate the strings stored inside each node
 void
 del_tree(tnode_t *tree)
 {
 	if(tree!=NULL){
 		if(tree->leftNode!=NULL)
 			del_tree(tree->leftNode);
 		if(tree->rightNode!=NULL)
 			del_tree(tree->rightNode);
 
 		free(tree);
 	}
}
