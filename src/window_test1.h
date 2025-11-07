#ifndef _WINDOW_TEST1_H
#define _WINDOW_TEST1_H

#define WINDOW_TEST1_BANK __bank(1)

typedef struct wi_window_test1 wi_window_test1_t;
struct wi_window_test1 {
    
    // Inherited
    ll_node_t node;    
    wi_panel_t __far *parent;

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

    // Window object
    bool draw_border;
    uint8_t border_color;

    // Test1 properties
    //wi_label_t __far *label1;
    //wi_label_t __far *label2;
    void __far *label1;
    void __far *label2;

    // Test1 methods 
    bool __far (*parent_gotfocus)(void __far *element);
    bool __far (*parent_lostfocus)(void __far *element);
    bool __far (*parent_mouseup)(void __far *element, uint16_t button);
    bool __far (*parent_mousedown)(void __far *element, uint16_t button);
    
};

WINDOW_TEST1_BANK wi_window_test1_t __far *window_test1_create(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
WINDOW_TEST1_BANK void window_test1_init(wi_window_test1_t __far *window, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t color, uint8_t character);
WINDOW_TEST1_BANK void window_test1_done(wi_window_test1_t __far *window);
WINDOW_TEST1_BANK void window_test1_destroy(wi_window_test1_t __far *window);

#endif