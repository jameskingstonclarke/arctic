#pragma once

#include "Driver.h"
#include "../Types.h"

namespace Driver{
    class VGAGraphics : public Driver{
    public:
        VGAGraphics();
        ~VGAGraphics();
        void init() override;
        void close() override;
        void scroll(const u32 l);
        void putstr(const s8 * s);
        void putc(const s8 c);
        void colour(const u8 c);
        static constexpr u32 vga_green = 0x2;
        static constexpr u32 vga_cyan  = 0x3;
        static constexpr u32 vga_red   = 0x4;
    private:
        u32 m_idx = 0;
        u32 m_colour = 0;
        static constexpr u32 rows = 25;
        static constexpr u32 columns = 80;
        static constexpr u32 pixel_depth = 2;
        static u8* video_memory;
    };
}