#pragma once

namespace kd
{
    static void blurh(Image& dst, const Image& src)
    {
        for (int y = 0; y < src.h; y++)
        {
            for (int x = 1; x < src.w; x++)
            {
                uint32 c1 = src.data[y*src.w+x-1];
                uint32 c2 = src.data[y*src.w+x];

                uint32 c = ((c1 >> 1) & 0x7F7F7F7F) + ((c2 >> 1) & 0x7F7F7F7F);

                dst.data[y*dst.w+x] = c;
            }
        }
    }

    static void blurv(Image& dst, const Image& src)
    {
        for (int y = 1; y < src.h; y++)
        {
            for (int x = 0; x < src.w; x++)
            {
                uint32 c1 = src.data[y*src.w+x];
                uint32 c2 = src.data[(y-1)*src.w+x];

                uint32 c = ((c1 >> 1) & 0x7F7F7F7F) + ((c2 >> 1) & 0x7F7F7F7F);

                dst.data[y*dst.w+x] = c;
            }
        }
    }
}
