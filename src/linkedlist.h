#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#define LINKEDLIST_BANK __bank(2)

typedef struct ll_node ll_node_t
struct ll_node  {	
	struct ll_node __far* next;     // Pointer to next node in the list     
	struct ll_node __far* prev;     // Pointer to previous node in the list
    // Data follows here
};

typedef struct linkedlist linkedlist_t;
struct linkedlist {
    ll_node_t __far* head;          // Pointer to first node in the list
    ll_node_t __far* tail;          // Pointer to last node in the list    
    uint32_t count;                 // Number of nodes in the list
};

// Notes:
// - All functions assume the list and nodes are in far memory
// - The list must be initialized with ll_init() before use
// - The node size must be specified when initializing the list
// - The node size must be at least sizeof(ll_node_t)
// - The node size is recomened to be a multiple of 4 bytes for alignment
// - The user is responsible for allocating and freeing the nodes
// - The user is responsible for ensuring the nodes are the correct size
// - The user is responsible for ensuring the nodes are not already in a list
// - The user is responsible for ensuring the nodes are not freed while in a list
// - The user is responsible for ensuring the list is not modified while iterating over it
// - The user is responsible for ensuring the list is not freed while in use
// - The user is responsible for ensuring thread safety if used in a multi-threaded environment
// - The user is responsible for ensuring the list is not modified while another operation is in progress

// Function prototypes
LINKEDLIST_BANK void ll_init(linkedlist_t __far* list);                                                             //  Initialize the list
LINKEDLIST_BANK void ll_insert_head(linkedlist_t __far* list, ll_node_t __far* node);                               //  Insert a node at the head/tail of the list
LINKEDLIST_BANK void ll_insert_tail(linkedlist_t __far* list, ll_node_t __far* node);                               //  Insert a node at the head/tail of the list
LINKEDLIST_BANK void ll_remove(linkedlist_t __far* list, ll_node_t __far* node);                                    //  Remove a node from the list
LINKEDLIST_BANK void ll_move_to_top(linkedlist_t __far* list, ll_node_t __far* node);                               //  Move node to top of list
LINKEDLIST_BANK void ll_move_to_bottom(linkedlist_t __far* list, ll_node_t __far* node);                            //  Move node to bottom of list
LINKEDLIST_BANK void ll_move_up(linkedlist_t __far* list, ll_node_t __far* node);                                   //  Move node up
LINKEDLIST_BANK void ll_move_down(linkedlist_t __far* list, ll_node_t __far* node);                                 //  Move node down
LINKEDLIST_BANK ll_node_t __far* ll_get_head(linkedlist_t __far* list);                                             //  Get the head node
LINKEDLIST_BANK ll_node_t __far* ll_get_tail(linkedlist_t __far* list);                                             //  Get the tail node
LINKEDLIST_BANK ll_node_t __far* ll_next(linkedlist_t __far* list, ll_node_t __far* node);                          //  Get the next node after the node specified
LINKEDLIST_BANK ll_node_t __far* ll_prev(linkedlist_t __far* list, ll_node_t __far* node);                          //  Get the previous node before the node specified
LINKEDLIST_BANK bool ll_is_empty(linkedlist_t __far* list);                                                         //  Check if the list is empty
LINKEDLIST_BANK int ll_get_size(linkedlist_t __far* list);                                                          //  Get the number of nodes in the list
LINKEDLIST_BANK void ll_clear(linkedlist_t __far* list);                                                            //  Free all nodes in the list
LINKEDLIST_BANK void ll_iterate_forward(linkedlist_t __far* list, void __far (*iterate_node)(ll_node_t __far* node, void __far *data), void __far *data);     //  Iterate over the list forwards
LINKEDLIST_BANK void ll_iterate_backward(linkedlist_t __far* list, void __far (*iterate_node)(ll_node_t __far* node, void __far *data), void __far *data);    //  Iterate over the list backwards

#endif