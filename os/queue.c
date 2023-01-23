 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: queue.h
 *
 * Description: Source file for queue data structure
 *
 *******************************************************************************/
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

// Function to Create A New Node 
static Node* newNode(TCB* d, uint32 p) 
{
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL; 
    return temp; 
}
  
// Return the value at head 
TCB* peek(Node** head) 
{ 
    return (*head)->data; 
}
  
// Removes the element with the highest priority from the list 
void Dequeue(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
  
// Function to push according to priority 
void Enqueue(Node** head, TCB* d, uint32 p) 
{ 
    Node* start = (*head); 
	Node* n = start->next;
    // Create new Node 
    Node* temp = newNode(d, p); 
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority < p) {
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp; 
    } 
    else { 
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && n->priority > p) { 
            start = start->next; 
			n = start->next;
        } 
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 

// Function to check is list is empty
boolean isEmpty(Node** head) 
{
	if((*head) == NULL)
		return TRUE;
	else
		return FALSE;
} 