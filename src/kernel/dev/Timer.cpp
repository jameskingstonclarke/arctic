#include "Timer.h"
#include "../cpu/Interrupt.h"
#include "../KPrintf.h"
#include "../cpu/IDT.h"
#include "../io/Port.h"

namespace Dev::Timer{

    u32 tick = 0;

    extern "C" void timer_isr(){
        tick++;
        IO::kprintf("tick!\n");
        CPU::end_of_interrupt(0x20);
    }

    void init_timer(u32 frequency){
        CPU::add_entry(0x20, (u32)&exec_20_isr, 0x08, 0x8E);

        // The value we send to the PIT is the value to divide it's input clock
        // (1193180 Hz) by, to get our required frequency. Important to note is
        // that the divisor must be small enough to fit into 16-bits.
        u32 divisor = 1193180 / frequency;
        IO::outb(0x43, 0x36);

        // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
        u8 l = (u8)(divisor & 0xFF);
        u8 h = (u8)( (divisor>>8) & 0xFF );

        // Send the frequency divisor.
        IO::outb(0x40, l);
        IO::outb(0x40, h);
    }
}