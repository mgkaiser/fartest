#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "farmalloc.h"

// Private methods 
FARMALLOC_BANK void pmalloc_merge(pmalloc_item_t __far *node);
FARMALLOC_BANK void pmalloc_item_insert(pmalloc_item_t __far *__near *root, void __far *ptr);
FARMALLOC_BANK void pmalloc_item_remove(pmalloc_item_t __far *__near *root, pmalloc_item_t __far *node);

pmalloc_t pm;

FARMALLOC_BANK void farmalloc_init()
{     
    pm.available = NULL;
	pm.assigned = NULL;
	pm.freemem = 0;
	pm.totalmem = 0;
	pm.totalnodes = 0;
}

FARMALLOC_BANK void farmalloc_addblock(void __far *ptr, uint32_t size)
{
    // Calculate the usable size of the block
    uint32_t usablesize = size - sizeof(pmalloc_item_t);

	// Get the usable size of the block
	((pmalloc_item_t __far *)ptr)->size = usablesize;
	((pmalloc_item_t __far *)ptr)->next = NULL;
	((pmalloc_item_t __far *)ptr)->prev = NULL;

	// Update freemem and totalmem
	pm.freemem += usablesize;
	pm.totalmem += usablesize;
	
	// Add it to the available heap, update totalnodes
	pmalloc_item_insert(&(pm.available), ptr);
	pm.totalnodes++;
}

FARMALLOC_BANK void farmalloc_addbanks(uint8_t startbank, uint8_t endbank, uint16_t sizeperbank)
{
	for(uint8_t bank=startbank; bank<=endbank; bank++) {
		farmalloc_addblock((void __far *)(((uint32_t)bank<<16) | 0xa000), sizeperbank);
	}	
}

FARMALLOC_BANK void __far *farmalloc(uint32_t size)
{    
	// Find a suitable block		
	pmalloc_item_t __far *current = pm.available;    	
	while(current != NULL && current->size < size) current = current->next;         

	// If there's nothing suitable, we're either out of memory or fragged.
	if(current == NULL) return NULL;    
	    
    // Remove it from pm.available
    pmalloc_item_remove(&(pm.available), current);
		    
    // Add to pm.assigned
	pmalloc_item_insert(&(pm.assigned), current);    

	// If it's not the exact size we need, split it
	if(current->size != size) {        
		
		// Add a free block that's the remainder size
        pmalloc_item_t __far *newfree = (pmalloc_item_t __far *)((char __far*)current + sizeof(pmalloc_item_t) + size);		

		newfree->size = current->size - sizeof(pmalloc_item_t) - size;
		newfree->next = current->next;
		newfree->prev = NULL;
		//current->next = NULL;
        		
		// Change pm.assigned size
		current->size = size;
		pmalloc_item_insert(&(pm.available), (void __far *)newfree);
        
		// We've lost a bit of overhead making the new node
		pm.freemem -= sizeof(pmalloc_item_t);
		pm.totalnodes++;

		// Merge around newfree
		pmalloc_merge(newfree);
	}		

	// Reduce the amount of free memory
	pm.freemem -= current->size;

	// Return the user memory
	return ((char __far *)current) + sizeof(pmalloc_item_t);
}

FARMALLOC_BANK void farfree(void __far *ptr)
{
	// Match stdlib free() NULL interface
	if(ptr == NULL) return;

	// Get the node of this memory	
    pmalloc_item_t __far *node = (pmalloc_item_t __far *)(((char __far*)ptr) - sizeof(pmalloc_item_t));

	// Remove it from pm->assigned
	pmalloc_item_remove(&(pm.assigned), node);

	// Free memory increases by node size
	pm.freemem += node->size;

	// Add to pm->available
	pmalloc_item_insert(&(pm.available), node);

	// Merge around current
	pmalloc_merge(node);
}

/////////////////////////////////////////////////////////////////////
// Private methods
/////////////////////////////////////////////////////////////////////

FARMALLOC_BANK void pmalloc_merge(pmalloc_item_t __far *node) {
    
	// Scan backward for contiguous blocks    
	while (node->prev != NULL && node == (pmalloc_item_t __far *node)(((char __far*)(node->prev)) + sizeof(pmalloc_item_t) + node->prev->size))
		node = node->prev;
    
	// Scan forward and merge free blocks
	while (node->next == (pmalloc_item_t __far *node)(((char __far*)node) + sizeof(pmalloc_item_t) + node->size)) {
		uint32_t nodesize = node->next->size + sizeof(pmalloc_item_t);
		pm.freemem += sizeof(pmalloc_item_t);
		pmalloc_item_remove(&(pm.available), node->next);
		pm.totalnodes--;
		node->size += nodesize;
	}
}

FARMALLOC_BANK void pmalloc_item_insert(pmalloc_item_t __far *__near *root, void __far *ptr)
{
    // No existing root
	if(*root == NULL) {        
		*root = (pmalloc_item_t __far *)ptr;
		(*root)->prev = NULL;
		(*root)->next = NULL;
		return;
	}
            
    // Where is the block in relation to the root?      
	if((pmalloc_item_t __far *)ptr < *root) {
        
		// New block before root		
		pmalloc_item_t __far *oldroot = *root;
		oldroot->prev = (pmalloc_item_t __far *)ptr;
		((pmalloc_item_t __far *)ptr)->next = oldroot;
		*root = (pmalloc_item_t __far *)ptr;

	} else {        

		// New block within or at end of list
		pmalloc_item_t __far *current = *root;
		pmalloc_item_t __far *node = (pmalloc_item_t __far *)ptr;
		
		// Skip until we find the right place to insert, or the end of the list        
		while(current->next != NULL && node > current->next) current = current->next;

		// We're inserting the block at...
		if(current->next == NULL) {
		
			// The end of list
			node->prev = current;
			current->next = node;

		} else {
			
			// Somewhere in the middle
			pmalloc_item_t __far *oldnext = current->next;

			current->next = node;
			node->prev = current;
			node->next = oldnext;
			oldnext->prev = node;
		}
	} 	
}

FARMALLOC_BANK void pmalloc_item_remove(pmalloc_item_t __far *__near *root, pmalloc_item_t __far *node) 
{   		        
	// Remove the node
	if(node->prev) node->prev->next = node->next;        
	if(node->next) node->next->prev = node->prev;    

	// Fixup root if the node was root
	if(node==*root) {        
		if (node->prev)
        {           			
			*root = node->prev;             
        }
		else
        {          
			*root = node->next;             
        }
	}

	// Clear the next and previous pointers    
	node->next = NULL;
	node->prev = NULL;    
}
