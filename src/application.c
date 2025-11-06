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
#include "application.h"

// Public method forwards declarations
APPLICATION_BANK void application_run(struct wi_application __far *application);
APPLICATION_BANK void application_insert(wi_application_t __far *application, void __far *childelement);
APPLICATION_BANK void application_postmessage(wi_application_t __far *application, wi_event_t __far *message);
APPLICATION_BANK void application_bringfront(wi_application_t __far *application, void __far *childelement);
APPLICATION_BANK void application_invalidate(struct wi_application __far *application);

// Private method forwards declarations
APPLICATION_BANK void application_pollmouse(struct wi_application __far *application);
APPLICATION_BANK void application_pollkb(struct wi_application __far *application);
APPLICATION_BANK void panel_setapplication_iterator(ll_node_t __far* node, void __far *data);

// Private mouse routines
APPLICATION_BANK __regsused("a/x/y") void mouse_config() =      \
                                            "\tsec\n            \
                                             \tjsr\t0xff5f\n    \
                                             \tlda\t#1\n        \
                                             \tjsr\t0xff68      ";

APPLICATION_BANK __regsused("a/x/y") void mouse_scan()   =      \
                                            "\tjsr\t0xff71\n";

APPLICATION_BANK __regsused("a/x")   uint8_t mouse_get() =      \
                                            "\tldx\t#$18\n      \
                                             \tjsr\t0xff6b\n    "; 

// Constructor
APPLICATION_BANK wi_application_t __far *application_create(uint8_t color, uint8_t character)
{    
    wi_application_t __far *new_application = farmalloc(sizeof(wi_application_t));        
    application_init(new_application, color, character);        
    return new_application;
}

APPLICATION_BANK void application_init(wi_application_t __far *application, uint8_t color, uint8_t character)
{
    application->desktop = desktop_create(color, character);      
    application->desktop->application = application;

    //  Setup method pointers    
    application->run = (void __far *)application_run;   
    application->insert = (void __far *)application_insert; 
    application->postmessage = (void __far *)application_postmessage;
    application->bringfront = (void __far *)application_bringfront;
    application->invalidate = (void __far *)application_invalidate;
    application->last_mouse_button = 0;

    // Initialize the list
    application->eventqueue = farmalloc(sizeof(linkedlist_t));
    ll_init(application->eventqueue);

    // Turn on the mouse cursor
    mouse_config();

    // Push the first draw event to Desktop          
    application->postmessage(application, event_create(WM_PAINT, WND_DESKTOP, 0, 0, 0, NULL));
}

// Destructor
APPLICATION_BANK void application_done(wi_application_t __far *application)
{    
    //farfree(application->desktop);
    ll_clear(application->eventqueue);
    farfree(application->eventqueue);
}

APPLICATION_BANK void application_destroy(wi_application_t __far *application)
{
    // Call the destructor    
    application_done(application);

    // Free the memory    
    farfree(application);
}

APPLICATION_BANK void application_run(struct wi_application __far *application)
{
    // Main application loop
    while (1)
    {

        application_pollmouse(application);
        application_pollkb(application);                
        
        // Pop a message from the queue
        wi_event_t __far *message = (wi_event_t __far *)ll_get_tail(application->eventqueue);                

        // If we got a message, process it
        if (message != NULL)
        {            
            //debug_printf("message: %04x %04x %04x\r", message->msg_id, message->param1, message->param2);

            // Process Application events (are there any?)
            if (message->wnd_id == WND_APPLICATION)
            {
                switch (message->msg_id)
                {
                    case WM_QUIT:
                        return;                        
                    default:
                        break;
                }
            }

            // Pass it to the event handler
            application->desktop->doevents(application->desktop, message);

            // When done destroy it
            ll_remove(application->eventqueue, message);
            event_destroy(message);
        }

    }
}

APPLICATION_BANK void panel_setapplication_iterator(ll_node_t __far* node, void __far *data) {
    wi_panel_t __far *panel = (wi_panel_t __far *)node;    
    wi_application_t __far *application =  (wi_application_t __far *)data;        
    panel->application = application;
    ll_iterate_forward(panel->children, panel_setapplication_iterator, (void __far *)application);  
}

APPLICATION_BANK void application_insert(wi_application_t __far *application, void __far *childelement)
{
    application->desktop->insert(application->desktop, childelement);
    ((wi_panel_t __far *)childelement)->application = application;    
    ll_iterate_forward(((wi_panel_t __far *)childelement)->children, panel_setapplication_iterator, (void __far *)application); 
}

APPLICATION_BANK void application_postmessage(wi_application_t __far *application, wi_event_t __far *message)
{
    ll_insert_head(application->eventqueue, (ll_node_t __far*)message);
}

APPLICATION_BANK void application_bringfront(wi_application_t __far *application, void __far *childelement)
{
    ll_remove(application->desktop->children, (ll_node_t __far *)childelement);
    ll_insert_tail(application->desktop->children, (ll_node_t __far *)childelement);    
    application->postmessage(application, event_create(WM_PAINT, WND_DESKTOP, 0, 0, 0, NULL));
}

APPLICATION_BANK void application_invalidate(struct wi_application __far *application)
{
    application->postmessage(application, event_create(WM_PAINT, WND_DESKTOP, 0, 0, 0, NULL));
}

APPLICATION_BANK void application_pollmouse(struct wi_application __far *application)
{
    // Look for mouse move/click
    uint8_t mouse_button = mouse_get();
    uint16_t mouse_x = MOUSE_DATA.x / 8;
    uint16_t mouse_y = MOUSE_DATA.y / 8;

    // Divide x and y by 8, compare to last position and only emit an event when they change
    if (mouse_x != application->last_mouse_x || mouse_y != application->last_mouse_y)
    {
        // Insert a mouse move event
        application->postmessage(application, event_create(WM_MOUSEMOVE, WND_BROADCAST, mouse_x, mouse_y, 0, NULL));

        // Remember the mouse position
        application->last_mouse_x = mouse_x;
        application->last_mouse_y = mouse_y;
    }

    // Compare click to last and only emit an event on change.
    if ((application->last_mouse_button & 0x01) != (mouse_button & 0x01))
    {            
        application->postmessage(application, event_create(((mouse_button & 0x01) == 0) ? WM_MOUSEUP : WM_MOUSEDOWN, WND_BROADCAST, mouse_x, mouse_y, 1, NULL));            
    }
    if ((application->last_mouse_button & 0x02) != (mouse_button & 0x02))
    {
        application->postmessage(application, event_create(((mouse_button & 0x02) == 0) ? WM_MOUSEUP : WM_MOUSEDOWN, WND_BROADCAST, mouse_x, mouse_y, 2, NULL));
    }
    application->last_mouse_button = mouse_button;
}

APPLICATION_BANK void application_pollkb(struct wi_application __far *application)
{

}
