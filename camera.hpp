#pragma once

#include "math.hpp"

namespace kd
{
    class Camera
    {
    public:
        Camera() {}
        ~Camera() {}

        void update()
        {
            Matrix4x4f m =
                perspective(fov, 1.f, 0.1f, 100.f) *
                look_at(position, target, Vector3f(0.f, 1.f, 0.f));

            viewToClip = m;
            clipToView = viewToClip;
            clipToView.invert();
        }

        Vector3f position;
        Vector3f target;
        float fov;

        Matrix4x4f viewToClip;
        Matrix4x4f clipToView;
    };
}
