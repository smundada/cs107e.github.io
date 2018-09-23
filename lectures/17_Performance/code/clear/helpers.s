// the symbol can now be accessed by clear.c
.globl assign8

// void assign8(void *dst, unsigned u, unsigned n);
// r0 - dst - destination at which to write bytes
// r1 - u   - byte to write
// r2 - n   - number of 8-byte chunks to write
assign8:
  // r3 -- caller-saved
  // r4, r5, r6, r7, r8, r10, r11 -- callee-saved
  push {r4, r5, r6, r7, r8, r10, r11}

  // load u into 8 registers
  mov r3, r1
  mov r4, r1
  mov r5, r1
  mov r6, r1
  mov r7, r1
  mov r8, r1
  mov r10, r1
  mov r11, r1

// for(int i = 0; i < n; i++) { ... }
loop:
  // store value of registers r3..r11 to address r0+offset
  // '!' is the write-back suffix. Final address that is written to is written
  // back into r0
  stm r0!, {r3, r4, r5, r6, r7, r8, r10, r11}
  subs r2, #1
  bne loop

  // restore registers saved prior
  pop {r4, r5, r6, r7, r8, r10, r11}
  bx lr
