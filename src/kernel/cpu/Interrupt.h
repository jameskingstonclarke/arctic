#pragma once


namespace CPU{

    extern const char* exception_types[31];
    extern void handle_exception(const char* exception);
}

extern void exc_0();
extern void exc_1();
extern void exc_2();
extern void exc_3();
extern void exc_4();
extern void exc_5();
extern void exc_6();
extern void exc_7();
extern void exc_8();
extern void exc_9();
extern void exc_10();
extern void exc_11();
extern void exc_12();
extern void exc_13();
extern void exc_14();
extern void exc_15();
extern void exc_16();
extern void exc_17();
extern void exc_18();
extern void exc_19();
extern void exc_20();
extern void exc_30();
extern void pit_handler();
extern void keyboard_handler();