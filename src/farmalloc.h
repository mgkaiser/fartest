#ifndef _FARMALLOC_H
#define _FARMALLOC_H

#define FARMALLOC_BANK __bank(4)

typedef struct unpacked_fp {
    uint16_t offset;
    uint8_t bank;    
} unpacked_fp_t;

typedef struct pmalloc_item {
    struct pmalloc_item __far *prev;    // The previous block in the chain
    struct pmalloc_item __far *next;    // The next block in the chain
    uint32_t size;                      // This is the size of the block as reported to the user 
} pmalloc_item_t;

typedef struct pmalloc {
    pmalloc_item_t __far *available;    // The linked list of available blocks
    pmalloc_item_t __far *assigned;     // The linked list of allocated blocks
    uint32_t freemem;                   // The current free memory count
    uint32_t totalmem;                  // The total available free memory
    uint32_t totalnodes;                // The number of nodes in the allocated list
} pmalloc_t;

// Public methods   
FARMALLOC_BANK void farmalloc_init();                                                              // Initialize the farmalloc system  
FARMALLOC_BANK void farmalloc_addblock(void __far *ptr, uint32_t size);                            // Add a single block
FARMALLOC_BANK void farmalloc_addbanks(uint8_t startbank, uint8_t endbank, uint16_t sizeperbank);  // Add banks from start to end inclusive  
FARMALLOC_BANK void __far *farmalloc(uint32_t size);                                               // Allocate a block of memory
FARMALLOC_BANK void farfree(void __far *ptr);                                                      // Free a previously allocated block

#endif