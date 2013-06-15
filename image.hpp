#pragma once

#include "defs.hpp"
#include <stdlib.h>

namespace kd
{
    class Image
    {
    public:
        Image() : w(0), h(0), data(0) {}
        ~Image() { destroy(); }

        void destroy()
        {
            if (data)
                free(data);
            w = h = 0;
            data = 0;
        }

        void resize(int nw, int nh)
        {
            destroy();

            w = nw;
            h = nh;
            data = (uint32*)malloc(w*h*4);
        }

        void put(int x, int y, Vector4f c)
        {
            kd_assert(x >= 0 && x < w);
            kd_assert(y >= 0 && y < h);

            int r = std::max(0, std::min(255, int(c.x * 256.f)));
            int g = std::max(0, std::min(255, int(c.y * 256.f)));
            int b = std::max(0, std::min(255, int(c.z * 256.f)));
            int a = std::max(0, std::min(255, int(c.w * 256.f)));

            data[y*w+x] = r | (g<<8) | (b<<16) | (a<<24);
        }

        int w, h;
        uint32* data;
    };
}
