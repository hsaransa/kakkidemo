#pragma once

#include "math.hpp"

namespace kd
{
    struct RayTracer
    {
        RayTracer() : image(0), camera(0), invRayLen(0) {}

        void update()
        {
            int w = image->w;
            int h = image->h;

            invW = new float [w*h];
            invRayLen = new float [w*h];

            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++)
                {
                    float fy = (y + .5f) / float(image->h) * 2.f - 1.f;
                    float fx = (x + .5f) / float(image->w) * 2.f - 1.f;

                    Vector4f e = Vector4f(fx, fy,  1.f, 1.f);
                    Vector4f e2 = camera->clipToView * e;

                    invW[y*w+x] = 1.f / e2.w;

                    e2 /= e2.w;

                    Vector3f o = camera->position;
                    Vector3f d = e2.xyz() - o;

                    float len = d.length();

                    invRayLen[y*w+x] = 1.f / len;
                }
        }

        Image* image;
        const Camera* camera;
        float* invW;
        float* invRayLen;
    };

    static void getRayForPixel(const RayTracer& rt, int x, int y, Vector3f& o, Vector3f& d)
    {
        float fy = (y + .5f) / float(rt.image->h) * 2.f - 1.f;
        float fx = (x + .5f) / float(rt.image->w) * 2.f - 1.f;

        Vector4f e = Vector4f(fx, fy,  1.f, 1.f);
        Vector4f e2 = rt.camera->clipToView * e;

        int p = y * rt.image->w + x;

        e2.x *= rt.invW[p];
        e2.y *= rt.invW[p];
        e2.z *= rt.invW[p];

        o = rt.camera->position;
        d = e2.xyz() - o;

        d.x *= rt.invRayLen[p];
        d.y *= rt.invRayLen[p];
        d.z *= rt.invRayLen[p];
    }

    static bool intersectCylinder(const Vector3f& o, const Vector3f& d, float& t1, float& t2)
    {
        float l = d.x * d.x + d.z * d.z;
        float a = d.z * o.x - d.x * o.z;
        float det = -(a * a) + l * 64.f;

        if (det < 0.f)
            return false;

        det = sqrtf(det);

        t1 = (-d.x * o.x - d.z * o.z - det) / l;
        t2 = (-d.x * o.x - d.z * o.z + det) / l;
        return true;
    }

    static float intersectPlane(const Vector3f& o, const Vector3f& d)
    {
        return o.y / -d.y;
    }

    static void raytraceSub(RayTracer& rt, int sx, int sy, int sw, int sh)
    {
        for (int y = sy; y < sy+sh; y++)
            for (int x = sx; x < sx+sw; x++)
            {
                Vector3f o, d;
                getRayForPixel(rt, x, y, o, d);

                float tp = intersectPlane(o, d);

                float tc1, tc2;

                if (intersectCylinder(o, d, tc1, tc2))
                {
                    if (tp > 0.f && tp < tc1)
                        goto plop;

                    float t;
                    if (tp > tc1 && tp < tc2)
                        t = tc2;

                    Vector3f p = o + d * t;

                    if (p.y > 0.f)
                        goto plop;

                    int xx = int(p.x * 5.f);
                    int yy = int(p.y * 5.f);

                    Vector4f c;

                    c.x = ((xx ^ yy) & 255) * (1.f / 255.f);
                    c.y = ((xx ^ yy) & 127) * (1.f / 127.f);
                    c.z = ((xx ^ yy) & 63) * (1.f / 63.f);
                    c.w = 1.f;

                    rt.image->put(x, y, c);

                    continue;
                }

plop:;
                if (tp > 0.f)
                {
                    Vector3f p = o + d * tp;

                    int xx = int(p.x * 5.f);
                    int yy = int(p.z * 5.f);

                    Vector4f c;

                    c.x = ((xx ^ yy) & 255) * (1.f / 255.f);
                    c.y = ((xx ^ yy) & 127) * (1.f / 127.f);
                    c.z = ((xx ^ yy) & 63) * (1.f / 63.f);
                    c.w = 1.f;

                    rt.image->put(x, y, c);
                }
                else
                {
                    rt.image->put(x, y, Vector4f(0.1f, 0.2f, 0.8f, 1.f));
                    rt.image->put(x, y, Vector4f((d+Vector3f(1.f, 1.f, 1.f))*.5f, 1.f));
                }
            }
    }
}
