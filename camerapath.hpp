#pragma once

#include "math.hpp"
#include <vector>

namespace kd
{
    class CameraPath
    {
    public:
        void add(const Matrix4x4f& m)
        {
            frames.push_back(m);
        }

        Matrix4x4f get(float t)
        {
            int i = int(t * 10.f);
            if (i < 0)
                return frames[0];
            if (i >= frames.size())
                return frames[frames.size()-1];
            return frames[i];
        }

        void save(const char* fn)
        {
            FILE* fp = fopen(fn, "w");
            for (int i = 0; i < (int)frames.size(); i++)
            {
                for (int j = 0; j < 16; j++)
                    fprintf(fp, " %f", frames[i].data()[j]);
                fprintf(fp, "\n");
            }
            fclose(fp);
        }

        void load(const char* fn)
        {
            FILE* fp = fopen(fn, "r");

            for (;;)
            {
                Matrix4x4f m;
                int n = fscanf(fp, "%f %f %f %f  %f %f %f %f  %f %f %f %f  %f %f %f %f",
                    &m.data()[0], &m.data()[1], &m.data()[2], &m.data()[3],
                    &m.data()[4], &m.data()[5], &m.data()[6], &m.data()[7],
                    &m.data()[8], &m.data()[9], &m.data()[10], &m.data()[11],
                    &m.data()[12], &m.data()[13], &m.data()[14], &m.data()[15]);

                if (n != 16)
                    break;

                frames.push_back(m);
            }

            fclose(fp);
        }

        std::vector<Matrix4x4f> frames;
    };
}
