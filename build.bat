clear

set SOURCES=./src
set BUILD=./build

set CCOMPILER=g++
set CCOMPILER=wsl $WSLENV/x86_64-elf-gcc
set LINKER=wsl $WSLENV/x86_64-elf-ld
set CFLAGS=-m32 -ffreestanding -nostdinc -nostdlib -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions

nasm -f elf32 %SOURCES%/kernel/arch/i386/cpu/I386GDTAsm.asm -o %BUILD%/I386GDTAsm.o
nasm -f elf32 %SOURCES%/kernel/arch/i386/cpu/I386IDTAsm.asm -o %BUILD%/I386IDTAsm.o
nasm -f elf32 %SOURCES%/kernel/arch/i386/cpu/I386CPUAsm.asm -o %BUILD%/I386CPUAsm.o
nasm -f elf32 %SOURCES%/kernel/arch/i386/memory/I386PagingAsm.asm -o %BUILD%/I386PagingAsm.o
nasm -f elf32 %SOURCES%/kernel/KernelAsm.asm -o %BUILD%/KernelAsm.o

%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/Kernel.cpp -o %BUILD%/Kernel.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/arch/i386/cpu/I386IDT.cpp -o %BUILD%/I386IDT.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/arch/i386/cpu/I386GDT.cpp -o %BUILD%/I386GDT.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/arch/i386/cpu/I386CPU.cpp -o %BUILD%/I386CPU.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/arch/i386/cpu/i386Interrupt.cpp -o %BUILD%/I386Interrupt.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/arch/i386/memory/I386Paging.cpp -o %BUILD%/I386Paging.o


%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/io/Debug.cpp -o %BUILD%/Debug.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/io/KPrintf.cpp -o %BUILD%/KPrintf.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/shell/Shell.cpp -o %BUILD%/Shell.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/io/Port.cpp -o %BUILD%/Port.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/io/Serial.cpp -o %BUILD%/Serial.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/dev/Keyboard.cpp -o %BUILD%/Keyboard.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/dev/Character.cpp -o %BUILD%/Character.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/dev/Device.cpp -o %BUILD%/Device.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/dev/Timer.cpp -o %BUILD%/Timer.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/driver/Driver.cpp -o %BUILD%/Driver.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/driver/PS2Keyboard.cpp -o %BUILD%/PS2Keyboard.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/driver/VGAGraphics.cpp -o %BUILD%/VGAGraphics.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/Panic.cpp -o %BUILD%/Panic.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/dev/Video.cpp -o %BUILD%/Video.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/memory/KMalloc.cpp -o %BUILD%/KMalloc.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/driver/VBEGraphics.cpp -o %BUILD%/VBEGraphics.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/utils/Math.cpp -o %BUILD%/Math.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/utils/String.cpp -o %BUILD%/String.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/utils/SStream.cpp -o %BUILD%/SStream.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/utils/Numeric.cpp -o %BUILD%/Numeric.o
%CCOMPILER% %CFLAGS% -c %SOURCES%/kernel/utils/Mem.cpp -o %BUILD%/Mem.o

%LINKER% -m elf_i386 -T %SOURCES%/link.ld -o %BUILD%/Arctic-0 ^
    %BUILD%/KernelAsm.o ^
    %BUILD%/Kernel.o ^
    %BUILD%/Debug.o ^
    %BUILD%/KPrintf.o ^
    %BUILD%/Shell.o ^
    %BUILD%/Port.o ^
    %BUILD%/I386Interrupt.o ^
    %BUILD%/I386IDT.o ^
    %BUILD%/I386GDT.o ^
    %BUILD%/I386CPU.o ^
    %BUILD%/I386Paging.o ^
    %BUILD%/Keyboard.o ^
    %BUILD%/Device.o ^
    %BUILD%/Character.o ^
    %BUILD%/Timer.o ^
    %BUILD%/Driver.o ^
    %BUILD%/KMalloc.o ^
    %BUILD%/PS2Keyboard.o ^
    %BUILD%/VGAGraphics.o ^
    %BUILD%/Panic.o ^
    %BUILD%/Video.o ^
    %BUILD%/I386GDTAsm.o ^
    %BUILD%/I386IDTAsm.o ^
    %BUILD%/I386CPUAsm.o ^
    %BUILD%/I386PagingAsm.o ^
    %BUILD%/VBEGraphics.o ^
    %BUILD%/Math.o ^
    %BUILD%/String.o ^
    %BUILD%/SStream.o ^
    %BUILD%/Numeric.o ^
    %BUILD%/Serial.o ^
    %BUILD%/Mem.o 

cp %SOURCES%/grub.cfg %BUILD%/boot/grub
objcopy -O elf32-i386 %BUILD%/Arctic-0 %BUILD%/boot/Arctic-0

if %1=="iso" (
    REM wsl grub-mkrescue %BUILD% -o %BUILD%/boot/iso/Arctic-0.iso
    qemu-system-i386 -cdrom %BUILD%/boot/Arctic-0.iso -serial stdio
)
if %1=="kernel" (
    qemu-system-i386 -kernel %BUILD%/boot/Arctic-0 -serial stdio
)

read