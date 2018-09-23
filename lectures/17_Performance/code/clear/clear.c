/* File clear.c
 * -----------
 * Simple main program with benchmarking code to compare
 * performance of different `write_screen` implementations.
 */

#include "assert.h"
#include "fb.h"
#include "printf.h"
#include "strings.h"
#include "system.h"
#include "timer.h"

// -------------------------------------------------------------------
// Graphics Invariants
// -------------------------------------------------------------------

#define _WIDTH  640
#define _HEIGHT 512
#define _DEPTH  4

// -------------------------------------------------------------------
// Time Trial Helper
// -------------------------------------------------------------------

#define ITERATIONS 3

#define TIME_TRIAL(command) do {                                  \
        printf("running " #command "\n");                         \
        int total = 0;                                            \
        for (int i = 0; i < ITERATIONS; i++) {                    \
            unsigned start = timer_get_ticks();                   \
            command(0x00);                                        \
            command(0xFF);                                        \
            unsigned ticks = timer_get_ticks() - start;           \
            printf("finished in %d ticks\n", ticks);              \
            total += ticks;                                       \
        }                                                         \
        printf("--------------------");                           \
        printf("----> average ticks: %d\n", total / ITERATIONS);  \
        timer_delay(2);                                           \
    } while (0)

// -------------------------------------------------------------------
// Wicked fast screen clearing functions to test
// -------------------------------------------------------------------

struct pixel { unsigned char b, g, r, alpha; };
typedef volatile struct pixel pixel_t;

static pixel_t *fb;

static void draw_pixel(unsigned char v, int x, int y) {
    pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
    pixel_t *p = &fb_s[y][x];
    p->r = v;
    p->g = v;
    p->b = v;
    p->alpha = 0xff;
}

void write_screen(unsigned char v) {
    for (volatile unsigned i = 0; i < fb_get_height(); i++) {
        for (volatile unsigned j = 0; j < fb_get_width(); j++) {
            draw_pixel(v, j, i);
        }
    }
}

void write_screen_hoist_height_width(unsigned char v) {
    int h = fb_get_height();
    int w = fb_get_width();
    for (volatile unsigned i = 0; i < h; i++) {
        for (volatile unsigned j = 0; j < w; j++) {
            draw_pixel(v, j, i);
        }
    }
}

static void draw_pixel_bitpattern(unsigned char v, int x, int y) {
    pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
    pixel_t *p = &fb_s[y][x];
    *(unsigned *)p =  (0xff << 24) | (v << 16) | (v << 8) | v;
}

void write_screen_bitpattern(unsigned char v) {
    int h = fb_get_height();
    int w = fb_get_width();
    for (volatile unsigned i = 0; i < h; i++) {
        for (volatile unsigned j = 0; j < w; j++) {
            draw_pixel_bitpattern(v, j, i);
        }
    }
}

// inline void draw_pixel_inline(unsigned char v, int x, int y) {
//     pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
//     pixel_t *p = &fb_s[y][x];
//     *(unsigned *)p =  (0xff << 24) | (v << 16) | (v << 8) | v;
// }

// void write_screen_inline(unsigned char v) {
//     int h = fb_get_height();
//     int w = fb_get_width();
//     for (volatile unsigned i = 0; i < h; i++) {
//         for (volatile unsigned j = 0; j < w; j++) {
//             draw_pixel_inline(v, j, i);
//         }
//     }
// }

void write_screen_inline(unsigned char v) {
    int h = fb_get_height();
    int w = fb_get_width();
    pixel_t (*fb_s)[w] = (pixel_t (*)[w])fb;
    for (volatile unsigned y = 0; y < h; y++) {
        for (volatile unsigned x = 0; x < w; x++) {
            pixel_t *p = &fb_s[y][x];
            *(unsigned *)p =  (0xff << 24) | (v << 16) | (v << 8) | v;
        }
    }
}

// void write_screen_one_for_all(unsigned char v) {
//     int h = fb_get_height();
//     int w = fb_get_width();
//     int p;
//     for (volatile unsigned i = 0; i < h*w; i++) {
//         p =  (0xff << 24) | (v << 16) | (v << 8) | v;
//         fb[i] = *(pixel_t *) &p;
//     }
// }

// void write_screen_one_for_hoisted(unsigned char v) {
//     int h = fb_get_height();
//     int w = fb_get_width();
//     int p;
//     p =  (0xff << 24) | (v << 16) | (v << 8) | v;
//     for (volatile unsigned i = 0; i < h*w; i++) {
//         fb[i] = *(pixel_t *) &p;
//     }
// }

// store all of p as an unsigned integer
void write_screen_unsigned(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    for (unsigned i = 0; i < n; i++) {
        ((unsigned *)fb)[i] = *(unsigned *) &p;
    }
}

void write_screen_ull(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u = *(unsigned *)&p;
    unsigned long long uu = (u << 32) | u;

    for (unsigned i = 0; i < n / 2; i++) {
        ((unsigned long long *)fb)[i] = uu;
    }
}

void write_screen_unroll(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u = *(unsigned *)&p;
    unsigned long long uu = (u << 32) | u;

    for (unsigned i = 0; i < n / 2; i+=8) {
        unsigned long long *fb_ll = (unsigned long long *)&fb[i];
        fb_ll[0] = uu;
        fb_ll[1] = uu;
        fb_ll[2] = uu;
        fb_ll[3] = uu;
        fb_ll[4] = uu;
        fb_ll[5] = uu;
        fb_ll[6] = uu;
        fb_ll[7] = uu;
    }
}

void assign8(void *dst, unsigned u, unsigned n);

void write_screen_asm(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    assign8((void *) fb, *(unsigned *) &p, n / 8);
}

// -------------------------------------------------------------------
// Main Benchmark -- don't forget to add time trials here!
// -------------------------------------------------------------------

int main() {
    timer_init();
    fb_init(_WIDTH, _HEIGHT, _DEPTH, 0);
    fb = (struct pixel *) fb_get_draw_buffer();
    timer_delay(3);   // wait for uart & screen to catch up

    printf("Ready to start time trials!\n");

    TIME_TRIAL(write_screen);
    TIME_TRIAL(write_screen_hoist_height_width);
    TIME_TRIAL(write_screen_bitpattern);
    TIME_TRIAL(write_screen_inline);
    // TIME_TRIAL(write_screen_one_for_all);
    // TIME_TRIAL(write_screen_one_for_hoisted);
    TIME_TRIAL(write_screen_unsigned);
    TIME_TRIAL(write_screen_ull);
    TIME_TRIAL(write_screen_unroll);
    TIME_TRIAL(write_screen_asm);


    printf("\nCaching turned on!\n\n");
    system_enable_cache();
    TIME_TRIAL(write_screen);
    TIME_TRIAL(write_screen_hoist_height_width);
    TIME_TRIAL(write_screen_bitpattern);
    TIME_TRIAL(write_screen_inline);
    // TIME_TRIAL(write_screen_one_for_all);
    // TIME_TRIAL(write_screen_one_for_hoisted);
    TIME_TRIAL(write_screen_unsigned);
    TIME_TRIAL(write_screen_ull);
    TIME_TRIAL(write_screen_unroll);
    TIME_TRIAL(write_screen_asm);

    printf("%c", 4); // Send EOT to signal done.
    return 0;
}
