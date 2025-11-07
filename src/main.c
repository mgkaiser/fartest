#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "vera.h"
#include "forwards.h"
#include "farmalloc.h"
#include "linkedlist.h"
#include "event.h"
#include "bounds.h"
#include "panel.h"
#include "desktop.h"
#include "window.h"
#include "application.h"
#include "window_test1.h"

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
    
    // Add test window 1    
    wi_window_test1_t __far *testWindow1 = window_test1_create(30, 12, 14, 20, 0x31, ' ');
    pApp->insert(pApp, testWindow1);    

    // Run the application
    pApp->run(pApp);

    // Destroy the application
    application_done(pApp);
}
