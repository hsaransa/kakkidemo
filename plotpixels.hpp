#pragma once

#include "image.hpp"
#include "math.hpp"

namespace kd
{
    struct PlotPixels
    {
        PlotPixels() : numPixels(0), maxPixels(0), pixelPos(0), pixelColor(0) {}
        ~PlotPixels() { delete[] pixelPos; delete[] pixelColor; }

        void create(int m)
        {
            numPixels = 0;
            maxPixels = m;
            pixelPos = new Vector3f [m];
            pixelColor = new Vector4f [m];
        }

        int numPixels;
        int maxPixels;
        Vector3f* pixelPos;
        Vector4f* pixelColor;
    };

    static void plotPixels(Image& dst, const Camera& cam, const PlotPixels& pp)
    {
        for (int i = 0; i < pp.numPixels; i++)
        {
            Vector4f p = cam.viewToClip * Vector4f(pp.pixelPos[i], 1.f);

            p /= p.w;

            p.x = (p.x + 1.f) * dst.w * .5f;
            p.y = (p.y + 1.f) * dst.h * .5f;

            int x = int(p.x);
            int y = int(p.y);

            if (x < 1 || y < 1 || x >= dst.w || y >= dst.h)
                continue;

            dst.put(x-1, y-1, pp.pixelColor[i]);
            dst.put(x, y-1, pp.pixelColor[i]);
            dst.put(x-1, y, pp.pixelColor[i]);
            dst.put(x, y, pp.pixelColor[i]);
        }
    }

    static void generateSphere(PlotPixels& pp, const Vector3f& p, float r, int n)
    {
        for (int i = 0; i < n; i++)
        {
            float u = (i+.5f) / n;
            float v = haltonf<2>(i+1);

            pp.pixelPos[pp.numPixels] = p + point_on_sphere(u, v) * r;
            pp.pixelColor[pp.numPixels] = Vector4f(1.f, 1.f, 1.f, 1.f);
            pp.numPixels++;
        }
    }

    static void plotImage(PlotPixels& pp, const Vector3f& p, Image& img, float sx, float sy)
    {
        for (int y = 0; y < img.h; y++)
            for (int x = 0; x < img.w; x++)
            {
                const float fx = sx * float(x) / float(img.w);
                const float fy = sy * float(y) / float(img.h);

                pp.pixelPos[pp.numPixels] = p + Vector3f(fx, fy, 0.f);
                pp.pixelColor[pp.numPixels] = img.get(x, y);
                pp.numPixels++;
            }
    }
}
