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

    printf("%c", 4); // Send EOT to signal done.
    return 0;
}
