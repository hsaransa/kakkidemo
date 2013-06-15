#include "SDL.h"
#include "SDL_opengl.h"
#include "math.hpp"
#include "image.hpp"
#include "camera.hpp"

using namespace kd;

static int screen_width = 700;
static int screen_height = 700;

static float demoTime;

static Image screen;

static void putImageFullScreen(const Image& img)
{
    glPixelZoom(screen_width / float(img.w), screen_height / float(img.h));
    glDrawPixels(img.w, img.h, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
}

static float intersectPlane(const Vector3f& o, const Vector3f& d)
{
    return o.y / -d.y;
}

void raytrace(Image& dst, const Camera& cam)
{
    for (int y = 0; y < screen.h; y++)
        for (int x = 0; x < screen.w; x++)
        {
            float fy = (y + .5f) / float(screen.h) * 2.f - 1.f;
            float fx = (x + .5f) / float(screen.w) * 2.f - 1.f;

            Vector4f s = Vector4f(fx, fy, -1.f, 1.f);
            Vector4f e = Vector4f(fx, fy,  1.f, 1.f);

            Vector4f s2 = cam.clipToView * s;
            Vector4f e2 = cam.clipToView * e;

            s2 /= s2.w;
            e2 /= e2.w;

            Vector3f o = s2.xyz();

            Vector3f dir = e2.xyz() - s2.xyz();
            dir.normalize();

            float t = intersectPlane(o, dir);

            if (t > 0.f)
            {
                Vector3f p = o + dir * t;

                int xx = int(p.x / 0.2f);
                int yy = int(p.z / 0.2f);

                Vector4f c;

                c.x = ((xx ^ yy) & 256) / 255.f;
                c.y = ((xx ^ yy) & 128) / 127.f;
                c.z = ((xx ^ yy) & 64) / 63.f;
                c.w = 1.f;

                dst.put(x, y, c);
            }
            else
                dst.put(x, y, Vector4f(dir, 1.f));
        }
}

static void render()
{
    glClearColor(0.2f, 0.4f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

#if 0
    for (int y = 0; y < screen.h; y++)
        for (int x = 0; x < screen.w; x++)
            screen.data[y*screen.w+x] = x*256+y*3;
#endif

    Camera camera;
    camera.position.x = cosf(demoTime*3.14159265f*2.f * 0.1f) * 10.f;
    camera.position.y = sinf(demoTime*3.14159265f*2.f * 0.2f) * 3.f + 10.f;
    camera.position.z = sinf(demoTime*3.14159265f*2.f * 0.1f) * 10.f;
    camera.fov = 3.14159265f*2.f * 90.f / 360.f;

    camera.update();

    raytrace(screen, camera);

    putImageFullScreen(screen);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetVideoMode(screen_width, screen_height, 0, SDL_OPENGL);

    screen.resize(128, 128);

    for (;;)
    {
        SDL_Event ev;

        SDL_PollEvent(&ev);

        if (ev.type == SDL_QUIT)
            break;

        if (ev.type == SDL_KEYDOWN)
        {
            if (ev.key.keysym.sym == SDLK_ESCAPE)
                break;
        }

        demoTime = SDL_GetTicks() / 1000.f;

        render();
        SDL_GL_SwapBuffers();
    }

}

