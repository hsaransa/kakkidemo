#pragma once

namespace kd
{
    static void wobbler(Image& dst, const Image& src, float a, float b)
    {
        int i = 0;
        for (int y = 0; y < dst.h; y++)
            for (int x = 0; x < dst.w; x++)
            {
                int xx = x + int(cosf((x + a) * 0.123f) * 6.f);
                int yy = y + int(sinf((y + b) * 0.143f) * 6.f);

                if (xx < 0 || yy < 0 || xx >= dst.w || yy >= dst.h)
                    dst.data[i] = 0;
                else
                    dst.data[i] = src.data[yy*src.w+xx];

                i++;
            }
    }
}
