#ifndef _DESKTOP_H
#define _DESKTOP_H

#define DESKTOP_BANK __bank(4)

struct wi_desktop {

    // Properties for the desktop object (including inherited properties)
    ll_node_t node;

    // Parent - Parent is ALWAYS a desktop
    struct wi_desktop __far *parent;    

    // Inherited from Panel object
    uint16_t wnd_id;
    wi_bounds_t bounds;    
    uint8_t color;
    uint8_t character;
    linkedlist_t __far* children;
    bool has_focus;
    wi_application_t __far *application;
    bool __far (*draw)(void __far* element);
    void __far (*insert)(void __far* element, void __far* childelement);
    void __far (*getbounds)(void __far *element, uint8_t *x, uint8_t *y, uint8_t *h, uint8_t *w);
    void __far (*doevents)(void __far *element, wi_event_t __far *message);
    bool __far (*mousemove)(void __far *element, uint16_t x, uint16_t y);
    bool __far (*mouseup)(void __far *element, uint16_t button);
    bool __far (*mousedown)(void __far *element, uint16_t button);
    bool __far (*gotfocus)(void __far *element);
    bool __far (*lostfocus)(void __far *element);
    bool __far (*click)(void __far *element);

    // Desktop Object
};

DESKTOP_BANK wi_desktop_t __far *desktop_create(uint8_t color, uint8_t character);
DESKTOP_BANK void desktop_init(wi_desktop_t __far *desktop, uint8_t color, uint8_t character);
DESKTOP_BANK void desktop_done(wi_desktop_t __far *desktop);
DESKTOP_BANK void desktop_destroy(wi_desktop_t __far *desktop);

#endif