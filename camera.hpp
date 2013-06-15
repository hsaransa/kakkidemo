#pragma once

#include "math.hpp"

namespace kd
{
    class Camera
    {
    public:
        Camera() : targetCamera(true) {}
        ~Camera() {}

        void translateLocal(const Vector3f& v)
        {
            view = translate(v) * view;
        }

        void rotateLocal(const Vector3f& v, float rad)
        {
            view = rotate(v, rad) * view;
        }

        void update()
        {
            if (targetCamera)
                view = look_at(position, target, Vector3f(0.f, 1.f, 0.f));
            else
            {
                Vector4f p = invert(view) * Vector4f(0.f, 0.f, 0.f, 1.f);
                p /= p.w;
                position = p.xyz();
            }

            Matrix4x4f m = perspective(fov, 1.f, 0.1f, 100.f) * view;
            viewToClip = m;

            clipToView = viewToClip;
            clipToView.invert();
        }

        bool targetCamera;

        Vector3f position;
        Vector3f target;
        float fov;

        Matrix4x4f view;

        Matrix4x4f viewToClip;
        Matrix4x4f clipToView;
    };
}
