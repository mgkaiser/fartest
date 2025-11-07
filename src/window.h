#ifndef _WINDOW_H
#define _WINDOW_H

#define WINDOW_BANK __bank(4)

struct wi_window {
    
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

    // Window object
    bool draw_border;
    uint8_t border_color;
    bool __far (*parent_draw)(void __far* element);

};

WINDOW_BANK wi_window_t __far *window_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
WINDOW_BANK void window_init(wi_window_t __far *window, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
WINDOW_BANK void window_done(wi_window_t __far *window);
WINDOW_BANK void window_destroy(wi_window_t __far *window);

#endif