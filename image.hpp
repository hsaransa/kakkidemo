#pragma once

#include "defs.hpp"
#include <stdlib.h>
#include <string>

#include "SDL_image.h"

namespace kd
{
    class Image
    {
    public:
        Image() : w(0), h(0), data(0) {}
        ~Image() { destroy(); }

        Image(std::string filename)
        {
            SDL_Surface* surface = IMG_Load(filename.c_str());

            resize(surface->w, surface->h);

            for (int y = 0; y < surface->h; y++)
            {
                memcpy(static_cast<uint32*>(data) + y*w,
                        static_cast<uint8*>(surface->pixels) + y*surface->pitch,
                        w*4);
            }

            SDL_FreeSurface(surface);
        }

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

        float toF(const uint8& comp)
        {
            return float(comp) / float(255);
        }

        Vector4f get(int x, int y)
        {
            const uint32& pixel = data[y*w+x];

            const uint8 r = pixel & 0xff;
            const uint8 g = (pixel>> 8) & 0xff;
            const uint8 b = (pixel>>16) & 0xff;
            const uint8 a = (pixel>>24) & 0xff;

            return Vector4f(toF(r), toF(g), toF(b), toF(a));
        }

        int w, h;
        uint32* data;
    };
}
