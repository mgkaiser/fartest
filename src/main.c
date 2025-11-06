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

    printf("Starting application...\n"); getchar();
    main_init();
    printf("Application ended.\n"); getchar();
    return 0;
}  

// This is in bank 1  
MAIN_BANK void main_init()
{
    printf("Initializing application...\n"); getchar();
    pApp = application_create(0x61, 0x66);  

    printf("Running application...\n"); getchar(); 
    pApp->run(pApp);

    printf("Destroying application...\n"); getchar();  
    application_done(pApp);
}
