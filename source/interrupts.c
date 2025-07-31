#include "cpu.h"
#include "display.h"
#include "keys.h"
#include "main.h"
#include "memory.h"
#include "platform.h"
#include "registers.h"
#include <stdio.h>

#include "interrupts.h"

struct interrupt interrupt;

void interruptStep(void) {
  if (interrupt.master && interrupt.enable && interrupt.flags) {
    unsigned char fire = interrupt.enable & interrupt.flags;

    // printf("%b \n", fire);

    if (fire & INTERRUPTS_VBLANK) {
      interrupt.flags &= ~INTERRUPTS_VBLANK;
      vblank();
    }

    if (fire & INTERRUPTS_LCDSTAT) {
      interrupt.flags &= ~INTERRUPTS_LCDSTAT;
      lcdStat();
    }

    if (fire & INTERRUPTS_TIMER) {
      interrupt.flags &= ~INTERRUPTS_TIMER;
      timer();
    }

    if (fire & INTERRUPTS_SERIAL) {
      interrupt.flags &= ~INTERRUPTS_SERIAL;
      serial();
    }

    if (fire & INTERRUPTS_JOYPAD) {
      interrupt.flags &= ~INTERRUPTS_JOYPAD;
      joypad();
    }
  }
}

void vblank(void) {
  drawFramebuffer();

  interrupt.master = 0;
  writeShortToStack(registers.pc);
  registers.pc = 0x40;

  ticks += 12;
}

void lcdStat(void) {
  interrupt.master = 0;
  writeShortToStack(registers.pc);
  registers.pc = 0x48;

  ticks += 12;
}

void timer(void) {
  interrupt.master = 0;
  writeShortToStack(registers.pc);
  registers.pc = 0x50;

  ticks += 12;
}

void serial(void) {
  interrupt.master = 0;
  writeShortToStack(registers.pc);
  registers.pc = 0x58;

  ticks += 12;
}

void joypad(void) {
  interrupt.master = 0;
  writeShortToStack(registers.pc);
  registers.pc = 0x60;

  ticks += 12;
}

void returnFromInterrupt(void) {
  printf("RETI\n");
  interrupt.master = 1;
  registers.pc = readShortFromStack();
}
