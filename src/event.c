#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "farmalloc.h"
#include "linkedlist.h"
#include "event.h"

EVENT_BANK wi_event_t __far *event_create(uint16_t msg_id, uint16_t wnd_id, uint16_t param1, uint16_t param2, uint16_t param3, void __far *data)
{
    wi_event_t __far *new_event = farmalloc(sizeof(wi_event_t));
    event_init(new_event, msg_id, wnd_id, param1, param2, param3, data);    
    return new_event;
}

EVENT_BANK void event_init(wi_event_t __far *event, uint16_t msg_id, uint16_t wnd_id, uint16_t param1, uint16_t param2, uint16_t param3, void __far *data)
{
    event->msg_id = msg_id;
    event->wnd_id = wnd_id;
    event->param1 = param1;
    event->param2 = param2;
    event->param3 = param3;
    event->data = data;
    event->processed = false;
}

EVENT_BANK void event_done(wi_event_t __far *event)
{
    if (event->data != NULL) farfree(event->data);
}

EVENT_BANK void event_destroy(wi_event_t __far *event)
{
    event_done(event);
    farfree(event);
}