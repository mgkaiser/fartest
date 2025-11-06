#ifndef _APPLICATION_H
#define _APPLICATION_H

#define APPLICATION_BANK __bank(1)

//typedef struct wi_desktop wi_desktop_t;
typedef struct wi_application wi_application_t;

struct wi_application {
    
    //wi_desktop_t __far *desktop;
    void __far *desktop;
    linkedlist_t __far* eventqueue;    
    uint8_t last_mouse_button;
    uint16_t last_mouse_x;
    uint16_t last_mouse_y;
    void __far (*run)(wi_application_t __far* element);
    void __far (*insert)(wi_application_t __far *application, void __far *childelement);
    void __far (*postmessage)(wi_application_t __far *application, wi_event_t __far *message);    
    void __far (*bringfront)(wi_application_t __far *application, void __far *childelement);
    void __far (*invalidate)(wi_application_t __far* element);
};

typedef struct wi_mouse_data {
    uint16_t x;
    uint16_t y;
} wi_mouse_data_t;
#define MOUSE_DATA (*(volatile wi_mouse_data_t *)0x0018)

// Constructor / Destructor
APPLICATION_BANK wi_application_t __far *application_create(uint8_t color, uint8_t character);
APPLICATION_BANK void application_init(wi_application_t __far *application, uint8_t color, uint8_t character);
APPLICATION_BANK void application_done(wi_application_t __far *application);
APPLICATION_BANK void application_destroy(wi_application_t __far *application);

// Public methods
APPLICATION_BANK void application_run(struct wi_application __far *application);
APPLICATION_BANK void application_insert(wi_application_t __far *application, void __far *childelement);
APPLICATION_BANK void application_postmessage(wi_application_t __far *application, wi_event_t __far *message);
APPLICATION_BANK void application_bringfront(wi_application_t __far *application, void __far *childelement);
APPLICATION_BANK void application_invalidate(struct wi_application __far *application);

#endif 