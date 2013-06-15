#pragma once

#include "math.hpp"

namespace kd
{
    struct RayTracePrecalc
    {
        const Camera* camera;

    };

    static float intersectPlane(const Vector3f& o, const Vector3f& d)
    {
        return o.y / -d.y;
    }

    static void getRayForPixel(const Image& img, const Camera& cam, int x, int y, Vector3f& o, Vector3f& d)
    {
        float fy = (y + .5f) / float(img.h) * 2.f - 1.f;
        float fx = (x + .5f) / float(img.w) * 2.f - 1.f;

        Vector4f s = Vector4f(fx, fy, -1.f, 1.f);
        Vector4f e = Vector4f(fx, fy,  1.f, 1.f);

        Vector4f s2 = cam.clipToView * s;
        Vector4f e2 = cam.clipToView * e;

        s2 /= s2.w;
        e2 /= e2.w;

        o = s2.xyz();
        d = e2.xyz() - o;
    }

    static void raytraceSub(Image& dst, const Camera& cam, int sx, int sy, int sw, int sh)
    {
        for (int y = sy; y < sy+sh; y++)
            for (int x = sx; x < sx+sw; x++)
            {
                Vector3f o, d;
                getRayForPixel(dst, cam, x, y, o, d);

                float t = intersectPlane(o, d);

                if (t > 0.f)
                {
                    Vector3f p = o + d * t;

                    int xx = int(p.x * 5.f);
                    int yy = int(p.z * 5.f);

                    Vector4f c;

                    c.x = ((xx ^ yy) & 255) * (1.f / 255.f);
                    c.y = ((xx ^ yy) & 127) * (1.f / 127.f);
                    c.z = ((xx ^ yy) & 63) * (1.f / 63.f);
                    c.w = 1.f;

                    dst.put(x, y, c);
                }
                else
                {
                    dst.put(x, y, Vector4f(0.1f, 0.2f, 0.8f, 1.f));
                }
            }
    }
}
