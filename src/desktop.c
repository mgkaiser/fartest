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
#include "desktop.h"

// Public method forwards declarations
DESKTOP_BANK bool desktop_draw(void __far *element);
DESKTOP_BANK void desktop_get_bounds(void __far *element, uint8_t *x, uint8_t *y, uint8_t *h, uint8_t *w);

// Constructor
DESKTOP_BANK wi_desktop_t __far *desktop_create(uint8_t color, uint8_t character)
{
    wi_desktop_t __far *new_desktop = farmalloc(sizeof(wi_desktop_t));
    desktop_init(new_desktop, color, character);    
    return new_desktop;
}

DESKTOP_BANK void desktop_init(wi_desktop_t __far *desktop, uint8_t color, uint8_t character)
{
    panel_init((wi_panel_t __far *)desktop, 0, 0, 59, 79, color, character);    
    desktop->wnd_id = WND_DESKTOP;
    desktop->draw = desktop_draw;
    desktop->getbounds = desktop_get_bounds;
}

// Destructor
DESKTOP_BANK void desktop_done(wi_desktop_t __far *desktop)
{
    panel_done((wi_panel_t __far *)desktop);    
}

DESKTOP_BANK void desktop_destroy(wi_desktop_t __far *desktop)
{
    desktop_destroy(desktop);
    farfree(desktop);
}


// Public methods implementation
DESKTOP_BANK void desktop_draw_iterator(ll_node_t __far* node, void __far *data) {
  wi_desktop_t __far *desktop = (wi_desktop_t __far *)node;    
  desktop->draw(desktop);  
}

DESKTOP_BANK bool desktop_draw(void __far *element)
{   
    wi_desktop_t __far *desktop = (wi_desktop_t __far *)element;        

    // Enable cache write
    VERA.control = VERA_DCSEL2;  
    VERA.display.dc2.fxctrl = 0b01000000;

    // Prime the cache
    VERA.control = VERA_DCSEL6;    
    VERA.display.dc6.fxcachel = desktop->character;
    VERA.display.dc6.fxcachem = desktop->color;
    VERA.display.dc6.fxcacheh = desktop->character;
    VERA.display.dc6.fxcacheu = desktop->color;
            
    // Make sure we're using Data0, DC0
    VERA.control = VERA_DCSEL0;

    // Address $1b000, increment 4
    SET_VERA_ADDRESS(0, 0, VERA_INC_4);

    // Write out 60 * 256 bytes, 4 bytes at a time
    for (uint16_t c = 0; c < ((60 * 256) /4); c++) VERA.data0 = 0;            

    // Disable cache write
    VERA.control = VERA_DCSEL2;  
    VERA.display.dc2.fxctrl = 0b00000000;

    // Draw all children
    ll_iterate_forward(desktop->children, desktop_draw_iterator, NULL);

    return false;
}

DESKTOP_BANK void desktop_get_bounds(void __far *element, uint8_t *x, uint8_t *y, uint8_t *h, uint8_t *w)
{
    *x = 0;
    *y = 0;
    *h = 59;
    *w = 79;
}

