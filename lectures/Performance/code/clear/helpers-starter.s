// the symbol can now be accessed by clear.c
.globl assign8

// void assign8(void *dst, unsigned u, unsigned n);
// r0 - dst - destination at which to write bytes
// r1 - u   - byte to write
// r2 - n   - number of 8-byte chunks to write
assign8:
