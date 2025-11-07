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

// Public method forwards declarations
WINDOW_BANK void window_draw(void __far *element);

// Constructor
WINDOW_BANK wi_window_t __far *window_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    wi_window_t __far *new_window = farmalloc(sizeof(wi_window_t));
    window_init(new_window, x, y, h, w, color, character);
    return new_window;
}

WINDOW_BANK void window_init(wi_window_t __far *window, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    // Do the parent init first
    panel_init((wi_panel_t __far *)window, x, y, h, w, color, character);

    // Then do the overrides
    window->draw_border = true;
    window->border_color = window->color;
    window->draw = window_draw;
}

// Destructor
WINDOW_BANK void window_done(wi_window_t __far *window)
{
    panel_done((wi_panel_t __far *)window);
}

WINDOW_BANK void window_destroy(wi_window_t __far *window)
{
    window_done(window);
    farfree(window);
}

// Public methods implementation
WINDOW_BANK bool window_draw(void __far *element)
{   
    wi_window_t __far *window = (wi_window_t __far *)element;
    uint8_t current_x;    
    uint8_t current_y;
    uint8_t current_h;
    uint8_t current_w;

    // Get the offsets of this object from all of its parents and the clipping range
    window->getbounds(window, &current_x, &current_y, &current_h, &current_w);
    
    // Do the parent draw first
    panel_draw(element);
    
    // Then draw a frame    
    for (uint8_t y1 = current_y; y1 < current_y + current_h; y1++ )
    {
        // Add the left and right edge
        SET_VERA_ADDRESS(current_x, y1, VERA_INC_1);
        VERA.data0 = 0x5d;
        VERA.data0 = window->border_color;

        SET_VERA_ADDRESS(current_x + current_w, y1, VERA_INC_1);
        VERA.data0 = 0x5d;
        VERA.data0 = window->border_color;
    }             
    uint8_t x_bounds = current_x + current_w;
    for (uint8_t x1 = current_x; x1 < x_bounds; x1++) 
    {
        
        // Add the left and right edge
        SET_VERA_ADDRESS(x1, current_y, VERA_INC_1);
        VERA.data0 = 0x40;
        VERA.data0 = window->border_color;

        SET_VERA_ADDRESS(x1, current_y + current_h, VERA_INC_1);
        VERA.data0 = 0x40;
        VERA.data0 = window->border_color;
    }             

    // Add the corners
    SET_VERA_ADDRESS(current_x, current_y, VERA_INC_1);
    VERA.data0 = BOX_UL;
    VERA.data0 = window->border_color;

    SET_VERA_ADDRESS(current_x, current_y + current_h, VERA_INC_1);
    VERA.data0 = BOX_LL;
    VERA.data0 = window->border_color;

    SET_VERA_ADDRESS(current_x + current_w, current_y, VERA_INC_1);
    VERA.data0 = BOX_UR;
    VERA.data0 = window->border_color;

    SET_VERA_ADDRESS(current_x + current_w, current_y + current_h, VERA_INC_1);
    VERA.data0 = BOX_LR;
    VERA.data0 = window->border_color;

    return false;

}