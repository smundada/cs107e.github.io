First, before going straight into optimization, let's take a look at our set-up here, and check our assumptions.
 - Walk through `TIME_TRIAL` first, then `main`.
 - Is our implementation of `write_screen` correct?

 - Are we convinced that the code runs correctly?
 - Are we convinced that the benchmarking code evaluates `write_screen` aptly?

 - What do we want to optimize for? SPEED.

 - Any bets as to how much faster we'll get?

COOL! Let's see how you can optimize this.

 1. You can optimize without even changing the code! Let's set compiler flags now.

 	start with nothing (default is -O0)
 	try -O1
 	try -O2
 	try -O3
 	try -O10
 	try -O100
 	try -Ofast

Now, we look at code.

 2. Pull `draw_pixel` inline.

```
void write_screen_inline(unsigned char v) {
    for (volatile unsigned i = 0; i < fb_get_height(); i++) {
        for (volatile unsigned j = 0; j < fb_get_width(); j++) {
            pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
            pixel_t *p = &fb_s[i][j];
            p->r = v;
            p->g = v;
            p->b = v;
            p->alpha = 0xff;
        }
    }
}
```

 3. Combine / squash the loop.

```
 void write_screen_squashloop(unsigned char v) {
    for (volatile unsigned i = 0; i < fb_get_width() * fb_get_height(); i++) {
        pixel_t *p = &fb[i];
        p->r = v;
        p->g = v;
        p->b = v;
        p->alpha = 0xff;
    }
}
```

 4. Pre-compute `n`.

```
void write_screen_precomp(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    for (volatile unsigned i = 0; i < n; i++) {
        pixel_t *p = &fb[i];
        p->r = v;
        p->g = v;
        p->b = v;
        p->alpha = 0xff;
    }
}
```

 5. Don't leave `volatile` in.

```
void write_screen_notvolatile(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    for (unsigned i = 0; i < n; i++) {
        pixel_t *p = &fb[i];
        p->r = v;
        p->g = v;
        p->b = v;
        p->alpha = 0xff;
    }
}
```

Also, don't forget to change: `typedef volatile struct pixel pixel_t;`
						to... `typedef struct pixel pixel_t;`

 6. Instead of writing three values separately, write them together.

```
void write_screen_loadrgb(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    for (unsigned i = 0; i < n; i++) {
        pixel_t *p = &fb[i];
        p->r = p->g = p->b = v;
        p->alpha = 0xff;
    }
}
```

 7. Try to pre-load the entire pixel... but it doesn't work so well.

```
void write_screen_loadpixel(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    for (unsigned i = 0; i < n; i++) {
        fb[i] = p;
    }
}
```

 8. Store as `unsigned` instead of `pixel_t`.

```
void write_screen_unsigned(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    for (unsigned i = 0; i < n; i++) {
        ((unsigned *)fb)[i] = *(unsigned *) &p;
    }
}
```

 9. We essentially re-created `memset`. Why don't we try using that now.

```
void write_screen_memset(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    memset((void *) fb, *(unsigned *) &p, n * sizeof(p));
}
```

 10. Why don't we try storing twice as pixels in one go?

```
void write_screen_ull(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u  = *(unsigned *) &p;
    unsigned long long uu = (u << 32) | u;

    for (unsigned i = 0; i < n/2; i++) {
        ((unsigned long long *)fb)[i] = uu;
    }
}
```

 11. Why not store more pixels per loop?

```
void write_screen_ull_unroll(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u  = *(unsigned *) &p;
    unsigned long long uu = (u << 32) | u;

    for (unsigned i = 0; i < n/2; i+= 8) {
        unsigned long long *fb_ll = (unsigned long long *) &fb[i];
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
```

 12. What if we tried using the `stm` instruction in ARM assembly?

Let's walk through the assembly code very quickly to make sure it does what we expect it to do.

```
void assign8(void *dst, unsigned u, unsigned n);

void write_screen_asm(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    assign8((void *) fb, *(unsigned *) &p, n / 8);
}
```

 13. Let's enable the system cache now.

```
    printf("\nCaching turned on!\n\n");
    system_enable_cache();
```


WOW, we went from ~2,075,171 ticks to ~2,561 -- that's >800x improvement!


What are the methods of computational optimization that we covered?

 * Compiler flags
 * Inlining
 * Loop combination
 * Hoisting loop invariants
 * Unnecessary type annotations (`volatile`)
 * Aggregate operations in loop by exploiting type size
 * Loop unrolling
 * Manually writing assembly
 * Hardware

 BTW this is a good resource to look at: http://icps.u-strasbg.fr/~bastoul/local_copies/lee.html