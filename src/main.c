#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "farmalloc.h"
#include "linkedlist.h"
#include "event.h"
#include "application.h"
#include "main.h"

MAIN_BANK wi_application_t __far* pApp = NULL;

// This is in unbanked memory
int main (void)
{ 
    // Initialize farmalloc with 8 banks of 8KB each
    farmalloc_init();
    farmalloc_addbanks(16, 32, 0x2000); 

    // Call banked main init
    main_init();
    
    // Exit program
    return 0;
}  

// This is in bank 1  
MAIN_BANK void main_init()
{
    // Create the application
    pApp = application_create(0x61, 0x66);  

    // Run the application
    pApp->run(pApp);

    // Destroy the application
    application_done(pApp);
}
