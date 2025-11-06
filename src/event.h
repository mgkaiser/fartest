#ifndef _EVENT_H
#define _EVENT_H

#define EVENT_BANK __bank(1)

enum {
    WND_BROADCAST   = 0xffff,
    WND_APPLICATION = 0xfffe,
    WND_DESKTOP     = 0x0000
};

enum {
    WM_PAINT        = 0x1000,
    WM_MOUSEMOVE    = 0x2000,
    WM_MOUSECLICK   = 0x2001,
    WM_MOUSEDOWN    = 0x2002,
    WM_MOUSEUP      = 0x2003,
    WM_KEYDOWN      = 0x3000,
    WM_KEY_UP       = 0x3001,   
    WM_GOTFOCUS     = 0x4000,
    WM_LOSTFOCUS    = 0x4001,
    WM_QUIT         = 0xffff        
};

struct wi_event {
    //uint8_t node[6];
    ll_node_t node;
    uint16_t msg_id;
    uint16_t wnd_id;
    uint16_t param1;
    uint16_t param2;
    uint16_t param3;
    void __far *data;
    bool processed;
};

EVENT_BANK wi_event_t __far *event_create(uint16_t msg_id, uint16_t wnd_id, uint16_t param1, uint16_t param2, uint16_t param3, void __far *data);
EVENT_BANK void event_init(wi_event_t __far *event, uint16_t msg_id, uint16_t wnd_id, uint16_t param1, uint16_t param2, uint16_t param3, void __far *data);
EVENT_BANK void event_done(wi_event_t __far *event);
EVENT_BANK void event_destroy(wi_event_t __far *event);


#endif