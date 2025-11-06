#ifndef _VERA_H
#define _VERA_H

// Position VERA address
#define SET_VERA_ADDRESS(x, y, step) VERA.address_hi = step + 1; VERA.address = 0xb000 + ((y) * 0x100) + ((x) * 0x02);

// Box characters
#define BOX_UL 0x70
#define BOX_LL 0x6d
#define BOX_UR 0x6e
#define BOX_LR 0x7d
#define BOX_SIDE_H 0x40
#define BOX_SIDE_V 0x5d 

enum {
    VERA_DCSEL0                 = 0 << 1,
    VERA_DCSEL1                 = 1 << 1,
    VERA_DCSEL2                 = 2 << 1,
    VERA_DCSEL3                 = 3 << 1,
    VERA_DCSEL4                 = 4 << 1,
    VERA_DCSEL5                 = 5 << 1,
    VERA_DCSEL6                 = 6 << 1,
    VERA_DCSEL63                = 63 << 1
};

/* VERA's address increment/decrement numbers */
enum {
    VERA_DEC_0                  = ((0 << 1) | 1) << 3,
    VERA_DEC_1                  = ((1 << 1) | 1) << 3,
    VERA_DEC_2                  = ((2 << 1) | 1) << 3,
    VERA_DEC_4                  = ((3 << 1) | 1) << 3,
    VERA_DEC_8                  = ((4 << 1) | 1) << 3,
    VERA_DEC_16                 = ((5 << 1) | 1) << 3,
    VERA_DEC_32                 = ((6 << 1) | 1) << 3,
    VERA_DEC_64                 = ((7 << 1) | 1) << 3,
    VERA_DEC_128                = ((8 << 1) | 1) << 3,
    VERA_DEC_256                = ((9 << 1) | 1) << 3,
    VERA_DEC_512                = ((10 << 1) | 1) << 3,
    VERA_DEC_40                 = ((11 << 1) | 1) << 3,
    VERA_DEC_80                 = ((12 << 1) | 1) << 3,
    VERA_DEC_160                = ((13 << 1) | 1) << 3,
    VERA_DEC_320                = ((14 << 1) | 1) << 3,
    VERA_DEC_640                = ((15 << 1) | 1) << 3,
    VERA_INC_0                  = ((0 << 1) | 0) << 3,
    VERA_INC_1                  = ((1 << 1) | 0) << 3,
    VERA_INC_2                  = ((2 << 1) | 0) << 3,
    VERA_INC_4                  = ((3 << 1) | 0) << 3,
    VERA_INC_8                  = ((4 << 1) | 0) << 3,
    VERA_INC_16                 = ((5 << 1) | 0) << 3,
    VERA_INC_32                 = ((6 << 1) | 0) << 3,
    VERA_INC_64                 = ((7 << 1) | 0) << 3,
    VERA_INC_128                = ((8 << 1) | 0) << 3,
    VERA_INC_256                = ((9 << 1) | 0) << 3,
    VERA_INC_512                = ((10 << 1) | 0) << 3,
    VERA_INC_40                 = ((11 << 1) | 0) << 3,
    VERA_INC_80                 = ((12 << 1) | 0) << 3,
    VERA_INC_160                = ((13 << 1) | 0) << 3,
    VERA_INC_320                = ((14 << 1) | 0) << 3,
    VERA_INC_640                = ((15 << 1) | 0) << 3
};

/* VERA's interrupt flags */
#define VERA_IRQ_VSYNC          0b00000001
#define VERA_IRQ_RASTER         0b00000010
#define VERA_IRQ_SPR_COLL       0b00000100
#define VERA_IRQ_AUDIO_LOW      0b00001000

/* A structure with the Video Enhanced Retro Adapter's external registers */
struct __vera {
    unsigned short      address;        /* Address for data ports */
    unsigned char       address_hi;
    unsigned char       data0;          /* Data port 0 */
    unsigned char       data1;          /* Data port 1 */
    unsigned char       control;        /* Control register */
    unsigned char       irq_enable;     /* Interrupt enable bits */
    unsigned char       irq_flags;      /* Interrupt flags */
    unsigned char       irq_raster;     /* Line where IRQ will occur */
    union {
        struct {                        /* Visible when DCSEL flag = 0 */
            unsigned char video;        /* Flags to enable video layers */
            unsigned char hscale;       /* Horizontal scale factor */
            unsigned char vscale;       /* Vertical scale factor */
            unsigned char border;       /* Border color (NTSC mode) */
        } dc0;
        struct {                        /* Visible when DCSEL flag = 1 */
            unsigned char hstart;       /* Horizontal start position */
            unsigned char hstop;        /* Horizontal stop position */
            unsigned char vstart;       /* Vertical start position */
            unsigned char vstop;        /* Vertical stop position */
        } dc1;
        struct {                        /* Visible when DCSEL flag = 2 */
            unsigned char fxctrl;
            unsigned char fxtilebase;
            unsigned char fxmapbase;
            unsigned char fxmult;
        } dc2;
        struct {                        /* Visible when DCSEL flag = 3 */
            unsigned char fxxincrl;
            unsigned char fxxincrh;
            unsigned char fxyincrl;
            unsigned char fxyincrh;
        } dc3;
        struct {                        /* Visible when DCSEL flag = 4 */
            unsigned char fxxposl;
            unsigned char fxxposh;
            unsigned char fxyposl;
            unsigned char fxyposh;
        } dc4;
        struct {                        /* Visible when DCSEL flag = 5 */
            unsigned char fxxposs;
            unsigned char fxyposs;
            unsigned char fxpolyfilll;
            unsigned char fxpolyfillh;
        } dc5;
        struct {                        /* Visible when DCSEL flag = 6 */
            unsigned char fxcachel;
            unsigned char fxcachem;
            unsigned char fxcacheh;
            unsigned char fxcacheu;
        } dc6;
        struct {                        /* Visible when DCSEL flag = 63 */
            unsigned char dcver0;
            unsigned char dcver1;
            unsigned char dcver2;
            unsigned char dcver3;
        } dc63;
    } display;
    struct {
        unsigned char   config;         /* Layer map geometry */
        unsigned char   mapbase;        /* Map data address */
        unsigned char   tilebase;       /* Tile address and geometry */
        unsigned int    hscroll;        /* Smooth scroll horizontal offset */
        unsigned int    vscroll;        /* Smooth scroll vertical offset */
    } layer0;
    struct {
        unsigned char   config;
        unsigned char   mapbase;
        unsigned char   tilebase;
        unsigned int    hscroll;
        unsigned int    vscroll;
    } layer1;
    struct {
        unsigned char   control;        /* PCM format */
        unsigned char   rate;           /* Sample rate */
        unsigned char   data;           /* PCM output queue */
    } audio;                            /* Pulse-Code Modulation registers */
    struct {
        unsigned char   data;
        unsigned char   control;
    } spi;                              /* SD card interface */
};
#define VERA    (*(volatile struct __vera *)0x9F20)

#endif