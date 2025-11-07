#ifndef _PANEL_H
#define _PANEL_H

#define PANEL_BANK __bank(3)

struct wi_panel {

    // Properties for the panel object (including inherited properties)
    ll_node_t node;
    struct wi_panel __far *parent;    

    // Panel object
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
};

PANEL_BANK wi_panel_t __far *panel_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
PANEL_BANK void panel_init(wi_panel_t __far *panel, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
PANEL_BANK void panel_done(wi_panel_t __far *panel);
PANEL_BANK void panel_destroy(wi_panel_t __far *panel);

#endif