#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// This is in bank 2
DATA_BANK wi_application_t app;

// This is in unbanked memory
int main (void)
{ 
  // Creates a pointer to application in bank 2
  wi_application_t __far* pApp = &app;

  // Initialize application structure
  pApp->x = 42;
  pApp->run = application_run;

  // Call main_init in bank 1 and pass pointer to application in bank 2
  main_init(pApp);
}  

// This is in bank 1  
MAIN_BANK void main_init(wi_application_t __far* element)
{
  printf("main_init called\n");   

  // Call application_run in bank 3 via function pointer  
  element->run(element);
}

// This is in bank 3
APPLICATION_BANK void application_run(struct wi_application __far *application)
{
  // From bank 3 print the value of x from the application structure in bank 2
  printf("application_run called %u\n", application->x);  

  // SHOULD OUTPUT: application_run called 42       (The value of application->x)
  // ACTUAL OUTPUT: application_run called 40960    (The address of application->x)
}

