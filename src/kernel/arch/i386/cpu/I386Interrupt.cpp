#include "I386Interrupt.h"
#include "../../../io/KPrintf.h"
#include "../../../io/Port.h"
#include "../../../dev/Keyboard.h"
#include "../../../io/Debug.h"
#include "I386IDT.h"

// universal interrupt wrapper function
extern "C" void interrupt_handler(CPU::Registers registers){
    // call the required interrupt
    CPU::interrupts[registers.int_no](registers);
    return;
}

extern "C" void interrupt_isr_0();
extern "C" void interrupt_isr_1();
extern "C" void interrupt_isr_2();
extern "C" void interrupt_isr_3();
extern "C" void interrupt_isr_4();
extern "C" void interrupt_isr_5();
extern "C" void interrupt_isr_6();
extern "C" void interrupt_isr_7();
extern "C" void interrupt_isr_err_8();
extern "C" void interrupt_isr_9();
extern "C" void interrupt_isr_err_10();
extern "C" void interrupt_isr_err_11();
extern "C" void interrupt_isr_err_12();
extern "C" void interrupt_isr_err_13();
extern "C" void interrupt_isr_err_14();
extern "C" void interrupt_isr_15();
extern "C" void interrupt_isr_16();
extern "C" void interrupt_isr_err_17();
extern "C" void interrupt_isr_18();
extern "C" void interrupt_isr_19();
extern "C" void interrupt_isr_20();
extern "C" void interrupt_isr_21();
extern "C" void interrupt_isr_22();
extern "C" void interrupt_isr_23();
extern "C" void interrupt_isr_24();
extern "C" void interrupt_isr_25();
extern "C" void interrupt_isr_26();
extern "C" void interrupt_isr_27();
extern "C" void interrupt_isr_28();
extern "C" void interrupt_isr_29();
extern "C" void interrupt_isr_err_30();
extern "C" void interrupt_isr_31();
extern "C" void interrupt_isr_32();
extern "C" void interrupt_isr_33();
extern "C" void interrupt_isr_34();
extern "C" void interrupt_isr_35();

namespace CPU{

    void (*interrupts[256])(Registers);
    InterruptService* interrupts_v2[256];

    void (*interrupts_isrs[])() = {
        interrupt_isr_0,
        interrupt_isr_1,
        interrupt_isr_2,
        interrupt_isr_3,
        interrupt_isr_4,
        interrupt_isr_5,
        interrupt_isr_6,
        interrupt_isr_7,
        interrupt_isr_err_8,
        interrupt_isr_9,
        interrupt_isr_err_10,
        interrupt_isr_err_11,
        interrupt_isr_err_12,
        interrupt_isr_err_13,
        interrupt_isr_err_14,
        interrupt_isr_15,
        interrupt_isr_16,
        interrupt_isr_err_17,
        interrupt_isr_18,
        interrupt_isr_19,
        interrupt_isr_20,
        interrupt_isr_21,
        interrupt_isr_22,
        interrupt_isr_23,
        interrupt_isr_24,
        interrupt_isr_25,
        interrupt_isr_26,
        interrupt_isr_27,
        interrupt_isr_28,
        interrupt_isr_29,
        interrupt_isr_err_30,
        interrupt_isr_31,
        interrupt_isr_32,
        interrupt_isr_33,
        interrupt_isr_34,
        interrupt_isr_35,
    };

    const char* exception_types[31] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Arithmetic Overflow",
        "Bound Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprosessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment Fault",
        "General Protection Fault",
        "Page Fault",
        "Reserved",
        "x87 Floating Point Exception",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating Point Exception",
        "Virtualisation Exception",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Triple Fault"
    };

    void handle_exception(const char* exception) {
        IO::kwarn(exception);
    }

    void setup_interrupts(){
        register_interrupt(0, trap_handler, 0x08, 0x8E);
        register_interrupt(1, trap_handler, 0x08, 0x8E);
        register_interrupt(2, trap_handler, 0x08, 0x8E);
        register_interrupt(3, trap_handler, 0x08, 0x8E);
        register_interrupt(4, trap_handler, 0x08, 0x8E);
        register_interrupt(5, trap_handler, 0x08, 0x8E);
        register_interrupt(6, trap_handler, 0x08, 0x8E);
        register_interrupt(7, trap_handler, 0x08, 0x8E);
        register_interrupt(8, trap_handler, 0x08, 0x8E);
        register_interrupt(9, trap_handler, 0x08, 0x8E);
        register_interrupt(10, trap_handler, 0x08, 0x8E);
        register_interrupt(11, trap_handler, 0x08, 0x8E);
        register_interrupt(12, trap_handler, 0x08, 0x8E);
        register_interrupt(13, trap_handler, 0x08, 0x8E);
        register_interrupt(14, trap_handler, 0x08, 0x8E);
        register_interrupt(15, trap_handler, 0x08, 0x8E);
        register_interrupt(16, trap_handler, 0x08, 0x8E);
        register_interrupt(17, trap_handler, 0x08, 0x8E);
        register_interrupt(18, trap_handler, 0x08, 0x8E);
        register_interrupt(19, trap_handler, 0x08, 0x8E);
    }

    void register_interrupt(u32 idx, void(interrupt)(Registers), u16 selector, u8 flags){
        interrupts[idx]=interrupt;
        CPU::add_entry(idx, (u32)interrupts_isrs[idx], selector, flags);
    }

    void register_interrupt(InterruptService* interrupt_service){
        interrupts_v2[interrupt_service->interrupt_idx()] = interrupt_service;
        IO::dbg() << "register_interrupt = " << interrupt_service->interrupt_idx() << "\n";
    }
    
    void end_of_interrupt(u32 idx){
        if(idx>=8){
            IO::outb(0xA0, 0x20);
        }
        IO::outb(0x20, 0x20);
    }

    void set_irq_mask(u32 idx){
        u16 port;
        u8 value;
    
        if(idx < 8) {
            port = 0x21;
        } else {
            port = 0xA1;
            idx -= 8;
        }
        value = IO::inb(port) | (1 << idx);
        IO::outb(port, value);  
    }

    void clear_irq_mask(u32 idx){
        u16 port;
        u8 value;
    
        if(idx < 8) {
            port = 0x21;
        } else {
            port = 0xA1;
            idx -= 8;
        }
        value = IO::inb(port) & ~(1 << idx);
        IO::outb(port, value);  
    }

    void trap_handler(Registers registers){
        IO::kprint_str(exception_types[registers.int_no]);
        IO::kprintf("\n");
        CPU::end_of_interrupt(registers.int_no);
        return;
    }
}