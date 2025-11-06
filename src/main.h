#ifndef _MAIN_H
#define _MAIN_H

#define MAIN_BANK __bank(1)
#define DATA_BANK __bank(2)
#define APPLICATION_BANK __bank(3)

typedef struct wi_application wi_application_t;
struct wi_application {
    uint8_t x;        
    void __far (*run)(wi_application_t __far* element);    
};

MAIN_BANK void main_init(wi_application_t __far* element);
APPLICATION_BANK void application_run(struct wi_application __far *application);

#endif