#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "forwards.h"
#include "farmalloc.h"
#include "linkedlist.h"

LINKEDLIST_BANK void ll_init(linkedlist_t __far* list)
{
    if (list == NULL) return;
    list->head = NULL;
    list->tail = NULL;    
    list->count = 0;
}

LINKEDLIST_BANK void ll_insert_head(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list or node
    if (list == NULL || node == NULL) return;

    // Insert node at the head of the list
    node->next = list->head;
    node->prev = NULL;

    //  Update the previous head node's prev pointer    
    if (list->head != NULL)
        list->head->prev = node;

    // Update the list head and tail pointers   
    list->head = node;

    // If the list was empty, update the tail pointer as well   
    if (list->tail == NULL)
        list->tail = node;

    // Increment the node count
    list->count++;
}

LINKEDLIST_BANK void ll_insert_tail(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list or node
    if (list == NULL || node == NULL) return;

    // Insert node at the tail of the list
    node->next = NULL;
    node->prev = list->tail;

    //  Update the previous tail node's next pointer    
    if (list->tail != NULL)
        list->tail->next = node;

    // Update the list head and tail pointers   
    list->tail = node;

    // If the list was empty, update the head pointer as well   
    if (list->head == NULL)
        list->head = node;

    // Increment the node count
    list->count++;
}

LINKEDLIST_BANK void ll_remove(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list or node
    if (list == NULL || node == NULL) return;

    // Update the previous node's next pointer
    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        list->head = node->next; // Node is head

    // Update the next node's prev pointer
    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        list->tail = node->prev; // Node is tail

    // Decrement the node count
    if (list->count > 0)
        list->count--;
}

LINKEDLIST_BANK void ll_move_to_top(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list, the list is empty, or node is already at the top
    if (list == NULL || list->head == NULL || node == NULL || list->head == node) return;

    // Remove node from its current position
    ll_remove(list, node);

    // Insert node at the head
    ll_insert_head(list, node);
}

LINKEDLIST_BANK void ll_move_to_bottom(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list, the list is empty, or node is already at the bottom
    if (list == NULL || list->head == NULL || node == NULL || list->tail == node) return;

    // Remove node from its current position
    ll_remove(list, node);

    // Insert node at the tail
    ll_insert_tail(list, node);
}

LINKEDLIST_BANK void ll_move_up(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list, the list is empty, or node is at the head
    if (list == NULL || list->head == NULL || node == NULL || node->prev == NULL) return;

    ll_node_t __far* prev = node->prev;

    // Remove node from its current position
    ll_remove(list, node);

    // Insert node before prev (after prev->prev)
    node->prev = prev->prev;
    node->next = prev;

    if (prev->prev != NULL)
        prev->prev->next = node;
    else
        list->head = node;

    prev->prev = node;

    // Increment count since ll_remove decremented it
    list->count++;
}

LINKEDLIST_BANK void ll_move_down(linkedlist_t __far* list, ll_node_t __far* node)
{
    // Bail if there is no list, the list is empty, or node is at the tail
    if (list == NULL || list->head == NULL || node == NULL || node->next == NULL) return;

    ll_node_t __far* next = node->next;

    // Remove node from its current position
    ll_remove(list, node);

    // Insert node after next (before next->next)
    node->next = next->next;
    node->prev = next;

    if (next->next != NULL)
        next->next->prev = node;
    else
        list->tail = node;

    next->next = node;

    // Increment count since ll_remove decremented it
    list->count++;
}

LINKEDLIST_BANK ll_node_t __far* ll_get_head(linkedlist_t __far* list)
{
    if (list == NULL) return NULL;
    return list->head;
}

LINKEDLIST_BANK ll_node_t __far* ll_get_tail(linkedlist_t __far* list)
{
    if (list == NULL) return NULL;
    return list->tail;
}

LINKEDLIST_BANK ll_node_t __far* ll_next(linkedlist_t __far* list, ll_node_t __far* node)
{
    if (list == NULL || node == NULL) return NULL;
    return node->next;
}

LINKEDLIST_BANK ll_node_t __far* ll_prev(linkedlist_t __far* list, ll_node_t __far* node)
{
    if (list == NULL || node == NULL) return NULL;
    return node->prev;
}

LINKEDLIST_BANK bool ll_is_empty(linkedlist_t __far* list)
{
    if (list == NULL) return true;
    return (list->count == 0);
}

LINKEDLIST_BANK int ll_get_count(linkedlist_t __far* list)
{
    if (list == NULL) return 0;
    return list->count;
}

// Frees all nodes in the list and resets the list to an empty state.
LINKEDLIST_BANK void ll_clear(linkedlist_t __far* list)
{
    ll_node_t __far* current;

    // Bail if there is no list
    if (list == NULL) return;

    // Free all nodes in the list
    current = ll_get_head(list);
    while (current != NULL)
    {
        ll_node_t __far* next = current->next;
        ll_remove(list, current);
        farfree(current);
        current = next;
    }

    // Reset list pointers and count
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

// Iterates over the list from head to tail, calling the provided callback function
// 'iterate_node' for each node. The callback receives a pointer to each node in order.
LINKEDLIST_BANK void ll_iterate_forward(linkedlist_t __far* list, void __far (*iterate_node)(ll_node_t __far* node, void __far *data), void __far *data)
{
    ll_node_t __far* current;

    if (list == NULL || iterate_node == NULL) return;

    current = list->head;
    while (current != NULL)
    {
        iterate_node(current, data);
        current = current->next;
    }
}

// Iterates over the list from tail to head, calling the provided callback function
// 'iterate_node' for each node. The callback receives a pointer to each node in reverse order.
LINKEDLIST_BANK void ll_iterate_backward(linkedlist_t __far* list, void __far (*iterate_node)(ll_node_t __far* node, void __far *data), void __far *data)
{
    ll_node_t __far* current;

    if (list == NULL || iterate_node == NULL) return;

    current = list->tail;
    while (current != NULL)
    {
        iterate_node(current, data);
        current = current->prev;
    }
}   
