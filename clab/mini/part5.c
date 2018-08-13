#include <stdio.h>
#include <assert.h>

#include "part5.h"

// Don't remove these two lines!
extern struct list_node *alloc_node(void);
extern void free_node(struct list_node *node);

// Insert a new list node with the given value right after the
// specified node.  The next pointer of the head node should point
// to the new node, and the next pointer of the new node should
// point to the old next pointer of the head node. 
// If the head node pointer refers to the node with the value 0,
// and list_insert(head, 1) is called, then the linked list
// structure after the list_insert call should be as follows:
// Use alloc_node to create a new node.  Don't forget to set its value!
void
list_insert(struct list_node *head, int value)
{
	assert(head != NULL); 

	struct list_node *nextNode=alloc_node();

	nextNode->value=value;
	nextNode->next=head->next;
	head->next=nextNode;
	//assert(0);
}

// Return a pointer to the last node in a linked list, starting
// from the given head node.  If the list only consists of the
// head node (i.e. the head node's next pointer is null), then
// simply return the head node pointer.
// If the head node pointer refers to the node with the value 0,
// list_end(head) should return a pointer to the node with the
// value of 2.
struct list_node *
list_end(struct list_node *head)
{
	assert(head != NULL);

	struct list_node *lastNode=head;

	while(lastNode->next!=NULL)
		lastNode=lastNode->next;

	//assert(0);
	return lastNode;
}

// Return the number of nodes in a linked list, starting from the
// given head pointer.  Since the head pointer is always non-null,
// the size of a list will be at least 1.
int
list_size(struct list_node *head)
{
	assert(head != NULL);

	int size=1;
	
	while(head->next!=NULL){
		head=head->next;
		size++;
	}

	//assert(0);
	return size;
}

// Return a pointer to the first node in the given linked list
// (starting at head) with the specified value, and store the pointer
// to its predecessor node at predp.  If no such node exists,
// return NULL and set the predecessor to NULL as well.
// If the head node contains the sought-after value, then the predecesor
// pointer should be set to NULL.
struct list_node *
list_find(struct list_node *head, int value, struct list_node **predp)
{
	assert(head != NULL);
	assert(predp != NULL); //is this even necessary?
	
	if(value==head->value){
		*predp=NULL;
		return head;
	}
	else {
		struct list_node *tempNode=head;
		while(tempNode->next!=NULL){
			if(value==tempNode->next->value){
				*predp=tempNode;
				return tempNode->next;
			}
			tempNode=tempNode->next;
		}
	}
	//assert(0);
	*predp=NULL;
	return NULL;
}

// Remove a node from the given linked list (starting at the given head)with the specified value.  Return 1 if a node was removed and 0 otherwise.  If the node removed is the head node, then set the pointer to the head pointer to point to the new list head (which should be head->next).  Use free_node on the removed node. Note that instead of a pointer to a node, the passed head pointer "headp" is actually a pointer to a pointer.  To get the pointer to the head node, you will need to dereference it.  A pointer to a pointer is passed so that the value of the head node pointer can be changed if the head node is removed.
//
// Hint: Use list_find to get the node to remove and its predecessor, then manipulate the next pointers of the nodes to restructure the list.
// Hint: Don't forget to call free_node!
// Hint: Don't forget to set the new pointer to the head node if the head node is removed.
int
list_remove(struct list_node **headp, int value)
{
	//assert(headp != NULL);
	assert(*headp != NULL);
	
	struct list_node *predp;
	struct list_node *removedNode=list_find(*headp, value, &predp);

	if(removedNode==NULL) 
		return 0;
	else{
		if(predp==NULL)
			*headp=(*headp)->next;
		else
			predp->next=removedNode->next;
		removedNode->next=NULL;
		free_node(removedNode);
		return 1;
	}
	
	//assert(0);
	return 0;
}
