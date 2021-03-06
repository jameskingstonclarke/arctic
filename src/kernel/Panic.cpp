#include "Panic.h"
#include "Kernel.h"
#include "io/KPrintf.h"
#include "dev/Video.h"
#include "io/Debug.h"

namespace Kernel{
    u8 panic(const char* file, u32 line, const char* msg){
        IO::dbg() << "kernel panic: " << msg << "\n";
        // set video mode VGA
        Device::set_video_mode(Device::VideoMode::VGA);
        IO::kerr("KERNEL PANIC\n");
        IO::kerr(file);
        IO::kprint_str(":");
        IO::kprint_int(line);
        IO::kprint_c('\n');
        IO::kerr(msg);
        return 0;
    }
}