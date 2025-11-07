#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "vera.h"
#include "forwards.h"
#include "farmalloc.h"
#include "linkedlist.h"
#include "event.h"
#include "bounds.h"
#include "panel.h"
#include "window.h"
#include "window_test1.h"

// Public method forwards declarations
WINDOW_TEST1_BANK bool window_test1_mouseup(void __far *element, uint16_t button);
WINDOW_TEST1_BANK bool window_test1_mousedown(void __far *element, uint16_t button);
WINDOW_TEST1_BANK bool window_test1_gotfocus(void __far *element);
WINDOW_TEST1_BANK bool window_test1_lostfocus(void __far *element);
WINDOW_TEST1_BANK bool window_test1_okclick(void __far *element);
WINDOW_TEST1_BANK bool window_test1_cancelclick(void __far *element);

// Constructor
WINDOW_TEST1_BANK wi_window_test1_t __far *window_test1_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    wi_window_test1_t __far *new_window = farmalloc(sizeof(wi_window_test1_t));
    window_test1_init(new_window, x, y, h, w, color, character);
    return new_window;
}

WINDOW_TEST1_BANK void window_test1_init(wi_window_test1_t __far *window, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    // Do the parent init first
    window_init((wi_window_t __far *)window, x, y, h, w, color, character);

    // Remember the parent functions and override
    window->parent_mouseup = window->mouseup;    
    window->mouseup=window_test1_mouseup;

    window->parent_mousedown = window->mousedown;       
    window->mousedown=window_test1_mousedown;    

    window->parent_gotfocus = window->gotfocus;
    window->gotfocus = window_test1_gotfocus;

    window->parent_lostfocus = window->lostfocus;
    window->lostfocus = window_test1_lostfocus;

    // Add some panels    
    //window->label1 = label_create(1, 1, 12, 0x32, "Hello World");
    //window->label2 = label_create(1, 2, 14, 0x32, "This is cool!");   
    //window->insert(window, window->label1);
    //window->insert(window, window->label2);

    //wi_button_t __far *okButton = button_create(3, 3, 14, 0x21, 0x12, "OK");
    //okButton->click = window_test1_okclick;
    
    //wi_button_t __far *cancelButton = button_create(3, 4, 14, 0x21, 0x12, "CANCEL");   
    //cancelButton->click = window_test1_cancelclick;  

    //window->insert(window, okButton);
    //window->insert(window, cancelButton);
        
}

// Destructor
WINDOW_TEST1_BANK void window_test1_done(wi_window_test1_t __far *window)
{
    window_done((wi_panel_t __far *)window);
}

WINDOW_TEST1_BANK void window_test1_destroy(wi_window_test1_t __far *window)
{
    window_test1_done(window);
    farfree(window);
}

// Public methods implementation
WINDOW_TEST1_BANK bool window_test1_mouseup(void __far *element, uint16_t button)
{   
    bool result = (((wi_window_test1_t __far *)element)->parent_mouseup(element, button)); 
    //((wi_window_test1_t  __far *)element)->application->bringfront(((wi_window_test1_t  __far *)element)->application, element);
    return result;    
}

WINDOW_TEST1_BANK bool window_test1_mousedown(void __far *element, uint16_t button)
{
    bool result = (((wi_window_test1_t __far *)element)->parent_mousedown(element, button));        
    return result;
}

WINDOW_TEST1_BANK bool window_test1_gotfocus(void __far *element)
{
    bool result = (((wi_window_test1_t __far *)element)->parent_gotfocus(element));    
    return result;
}

WINDOW_TEST1_BANK bool window_test1_lostfocus(void __far *element)
{
    bool result = (((wi_window_test1_t __far *)element)->parent_lostfocus(element));       
    return result;
}

WINDOW_TEST1_BANK bool window_test1_okclick(void __far *element)
{           
    //(((wi_window_test1_t __far *)element)->label1->color++;    
    //(((wi_window_test1_t __far *)element)->label1->draw(label1);
    return true;
}

WINDOW_TEST1_BANK bool window_test1_cancelclick(void __far *element)
{           
    //(((wi_window_test1_t __far *)element)->label2->color++;
    //(((wi_window_test1_t __far *)element)->label2->draw(label2);    
    return true;
}