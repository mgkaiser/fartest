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
#include "application.h"
#include "desktop.h"

uint16_t next_wnd_id = 0x0001;

// Public method forwards declarations
PANEL_BANK void panel_draw(void __far *element);
PANEL_BANK void panel_insert(void __far *element, void __far *childelement);
PANEL_BANK void panel_get_bounds(void __far *element, uint8_t *x, uint8_t *y, uint8_t *h, uint8_t *w);
PANEL_BANK void panel_doevents(void __far *element, wi_event_t __far *message);
PANEL_BANK bool panel_mousemove(void __far *element, uint16_t x, uint16_t y);
PANEL_BANK bool panel_mouseup(void __far *element, uint16_t button);
PANEL_BANK bool panel_mousedown(void __far *element, uint16_t button);
PANEL_BANK bool panel_gotfocus(void __far *element);
PANEL_BANK bool panel_lostfocus(void __far *element);
PANEL_BANK bool panel_click(void __far *element);

// Private method forwards declarations
PANEL_BANK bool panel_inbounds(wi_panel_t __far *panel, uint8_t x, uint8_t y);

// Constructor
PANEL_BANK wi_panel_t __far *panel_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    wi_panel_t __far *new_panel = farmalloc(sizeof(wi_panel_t));
    panel_init(new_panel, x, y, h, w, color, character);    
    return new_panel;
}

PANEL_BANK void panel_init(wi_panel_t __far *panel, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character)
{
    panel->application = NULL;
    panel->parent = NULL;    

    // Properties
    panel->wnd_id = next_wnd_id++;
    panel->bounds.x = x;
    panel->bounds.y = y;
    panel->bounds.h = h;
    panel->bounds.w = w;
    panel->color = color;
    panel->character = character;    
    panel->has_focus = false;
    
    // Methods
    panel->draw = panel_draw;
    panel->insert = panel_insert;
    panel->getbounds = panel_get_bounds;
    panel->doevents = panel_doevents;
    panel->mousemove = panel_mousemove;
    panel->mouseup = panel_mouseup;
    panel->mousedown = panel_mousedown;
    panel->gotfocus = panel_gotfocus;
    panel->lostfocus = panel_lostfocus;
    panel->click = panel_click;

    // Initialize the children list
    panel->children=farmalloc(sizeof(linkedlist_t));
    ll_init(panel->children);    
}

// Destructor
PANEL_BANK void panel_done(wi_panel_t __far *panel)
{
    ll_clear(panel->children);
    farfree(panel->children);
}

PANEL_BANK void panel_destroy(wi_panel_t __far *panel)
{
    panel_done(panel);
    farfree(panel);
}

PANEL_BANK bool panel_draw(void __far *element)
{   
    wi_panel_t __far *panel = (wi_panel_t __far *)element;    
    uint8_t current_x;    
    uint8_t current_y;
    uint8_t current_h;
    uint8_t current_w;

    // Get the offsets of this object from all of its parents and the clipping range
    panel->getbounds(panel, &current_x, &current_y, &current_h, &current_w);
        
    // Make sure we're using Data0, DC0
    VERA.control = VERA_DCSEL0;

    // Walk the y axis    
    for (uint8_t y1 = current_y; y1 < current_y + current_h; y1++ )
    {
        
        // Set the address        
        SET_VERA_ADDRESS(current_x, y1, VERA_INC_1);

        // Walk the x axis
        for (uint8_t x1 = current_x; x1 < current_x + current_w; x1++)       
        {
            VERA.data0 = panel->character;
            VERA.data0 = panel->color;
        }
    }    

    // Draw all children        
    wi_panel_t __far *node = (wi_panel_t __far *)ll_get_head(((wi_panel_t __far *)element)->children);
    while (node != NULL) {
        node->draw(node);
        node = (wi_panel_t __far *)ll_next(((wi_panel_t __far *)element)->children, node);
    }     

    return false;
}

PANEL_BANK void panel_insert(void __far *element, void __far *childelement)
{       
    ll_insert_tail(((wi_panel_t __far *)element)->children, childelement);
    ((wi_panel_t __far *)childelement)->parent = element;
    //((wi_panel_t __far *)childelement)->application = ((wi_panel_t __far *)element)->application; 
}

PANEL_BANK void panel_get_bounds(void __far *element, uint8_t *x, uint8_t *y, uint8_t *h, uint8_t *w)
{
    uint8_t parent_x;    
    uint8_t parent_y;
    uint8_t parent_h;
    uint8_t parent_w;

    wi_panel_t __far *panel = (wi_panel_t __far *)element;    

    if (panel->parent == NULL)    
    {        
        *x = panel->bounds.x;
        *y = panel->bounds.y;
        *h = panel->bounds.h;
        *w = panel->bounds.w;
    }
    else
    {          
        panel->parent->getbounds(panel->parent, &parent_x, &parent_y, &parent_h, &parent_w);

        *x = panel->bounds.x + parent_x;
        *y = panel->bounds.y + parent_y;
        *h = parent_h < panel->bounds.h ? parent_h : panel->bounds.h;
        *w = parent_w < panel->bounds.w ? parent_w : panel->bounds.w;        
    }
}

PANEL_BANK bool panel_mousemove(void __far *element, uint16_t x, uint16_t y)
{
    // This is a dummy event handler.  DO NOT add code here or it will happen in EVERY object. Override this.
    if (!((wi_panel_t __far *)element)->has_focus)
    {
        wi_application_t __far *application = (wi_application_t __far *)(((wi_panel_t __far *)element)->application);

        // Trap the lack of application.
        if (application == NULL) return false;           

        // Everything but this guy loses focus
        application->postmessage(application, event_create(WM_LOSTFOCUS, WND_BROADCAST, ((wi_panel_t __far *)element)->wnd_id, 0, 0, NULL));            

        // This guy gets focus
        application->postmessage(application, event_create(WM_GOTFOCUS, WND_BROADCAST, ((wi_panel_t __far *)element)->wnd_id, 0, 0, NULL));                    
    }
    return true;
}

PANEL_BANK bool panel_mouseup(void __far *element, uint16_t button)
{
    //wi_application_t __far *application = (wi_application_t __far *)(((wi_panel_t __far *)element)->application);    
    //application->postmessage(application, event_create(WM_MOUSECLICK, ((wi_panel_t __far *)element)->wnd_id, 0, 0, 0, NULL));                    
    ((wi_panel_t __far *)element)->click(element);    
    return true;
}

PANEL_BANK bool panel_mousedown(void __far *element, uint16_t button)
{    
    return true;
}

PANEL_BANK bool panel_gotfocus(void __far *element)
{
    // Only one item has focus so set focus to true and STOP procesing
    ((wi_panel_t __far *)element)->has_focus = true;
    return true;
}

PANEL_BANK bool panel_lostfocus(void __far *element)
{
    // Set focus to false and continue processing
    ((wi_panel_t __far *)element)->has_focus = false;
    return false;
}

PANEL_BANK bool panel_click(void __far *element)
{
    // This is a dummy event handler.  DO NOT add code here or it will happen in EVERY object. Override this.
    return false;
}   

PANEL_BANK void panel_doevents(void __far *element, wi_event_t __far *message)
{        
    if (message != NULL)
    {                   
        if (message->wnd_id == WND_BROADCAST || message->wnd_id == ((wi_panel_t __far *)element)->wnd_id)
        {   
            if (!message->processed)
            {         
                // Trigger events on all childen first                
                wi_panel_t __far *node = (wi_panel_t __far *)ll_get_tail(((wi_panel_t __far *)element)->children);
                while (node != NULL) {                    
                    node->doevents(node, message);
                    node = (wi_panel_t __far *)ll_prev(((wi_panel_t __far *)element)->children, node);
                }                 

                if (!message->processed)
                {

                    switch(message->msg_id)
                    {
                        case WM_PAINT:                
                            message->processed = ((wi_panel_t __far *)element)->draw((wi_panel_t __far *)element);
                            break;            

                        case WM_MOUSECLICK:                                  
                            message->processed = ((wi_panel_t __far *)element)->click((wi_panel_t __far *)element);
                            break;            

                        case WM_MOUSEMOVE:                    
                            if (panel_inbounds((wi_panel_t __far *)element, message->param1, message->param2))
                            {
                                message->processed = ((wi_panel_t __far *)element)->mousemove((wi_panel_t __far *)element, message->param1, message->param2);
                                // If you do not already have focus then 
                                //      trigger WM_GOTFOCUS targeted to THIS window.
                                //      trigger WM_LOSTFOCUS targeted to WND_BROADCAST with THIS window as Param1
                            }
                            break;

                        case WM_GOTFOCUS:                            
                            if (message->param1 == ((wi_panel_t __far *)element)->wnd_id)
                            {
                                message->processed = ((wi_panel_t __far *)element)->gotfocus((wi_panel_t __far *)element);    
                            }
                            break;

                        case WM_LOSTFOCUS:

                            // Only do this if you're not the window that just got focus
                            if (message->param1 != ((wi_panel_t __far *)element)->wnd_id)
                            {
                                if (((wi_panel_t __far *)element)->has_focus)
                                {
                                    message->processed = ((wi_panel_t __far *)element)->lostfocus((wi_panel_t __far *)element);    
                                }                                
                            }
                            break;

                        case WM_MOUSEUP:                    
                            if (panel_inbounds((wi_panel_t __far *)element, message->param1, message->param2))
                            {
                                message->processed = ((wi_panel_t __far *)element)->mouseup((wi_panel_t __far *)element, message->param3);
                            }
                            break;

                        case WM_MOUSEDOWN:                    
                            if (panel_inbounds((wi_panel_t __far *)element, message->param1, message->param2))
                            {
                                message->processed = ((wi_panel_t __far *)element)->mousedown((wi_panel_t __far *)element, message->param3);
                            }
                            break;
                    }      

                }
            }      
        }
    }
}

// Private method implementations
PANEL_BANK bool panel_inbounds(wi_panel_t __far *panel, uint8_t x, uint8_t y)
{
    uint8_t current_x;    
    uint8_t current_y;
    uint8_t current_h;
    uint8_t current_w;
    
    // Get the offsets of this object from all of its parents and the clipping range
    panel->getbounds(panel, &current_x, &current_y, &current_h, &current_w);

    return (x >= current_x) && (x <= current_x + current_w) && (y >= current_y) && (y <= current_y + current_h);
}