#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_thread.h"
#include "math.hpp"
#include "image.hpp"
#include "camera.hpp"

using namespace kd;

static int num_threads   = 3;
static int screen_width  = 700;
static int screen_height = 700;
static SDL_sem* sem;
static SDL_mutex* mutex;
static Image screen;
static float demoTime;

//
// Job.
//

enum JobType
{
    RAY_TRACE
};

struct Job
{
    int type;
    int x, y, w, h;
    Image* img;
    const Camera* cam;
};

static Job jobs[128];
static int numJobs;
static int jobsLeft;

static void putJob(Job& j)
{
    SDL_mutexP(mutex);
    jobs[numJobs++] = j;
    jobsLeft++;
    SDL_mutexV(mutex);
    SDL_SemPost(sem);
}

static bool allJobsDone()
{
    SDL_mutexP(mutex);
    bool done = jobsLeft == 0;
    SDL_mutexV(mutex);
    return done;
}

//
// Other.
//

static void putImageFullScreen(const Image& img)
{
    glPixelZoom(screen_width / float(img.w), screen_height / float(img.h));
    glDrawPixels(img.w, img.h, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
}

static float intersectPlane(const Vector3f& o, const Vector3f& d)
{
    return o.y / -d.y;
}

void raytraceSub(Image& dst, const Camera& cam, int sx, int sy, int sw, int sh)
{
    for (int y = sy; y < sy+sh; y++)
        for (int x = sx; x < sx+sw; x++)
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

                int xx = int(p.x * 5.f);
                int yy = int(p.z * 5.f);

                Vector4f c;

                c.x = ((xx ^ yy) & 256) * (1.f / 255.f);
                c.y = ((xx ^ yy) & 128) * (1.f / 127.f);
                c.z = ((xx ^ yy) & 64) * (1.f / 63.f);
                c.w = 1.f;

                dst.put(x, y, c);
            }
            else
                dst.put(x, y, Vector4f(dir, 1.f));
        }
}

void raytrace(Image& dst, const Camera& cam)
{
    for (int i = 0; i < 16; i++)
    {
        int y0 = dst.h * i / 16;
        int y1 = dst.h * (i+1) / 16;

        Job j;
        j.type = RAY_TRACE;
        j.x = 0;
        j.y = y0;
        j.w = dst.w;
        j.h = y1 - y0;
        j.img = &dst;
        j.cam = &cam;
        putJob(j);
    }

    while (!allJobsDone())
        SDL_Delay(1);
}

static int thread_func(void* id)
{
    for (;;)
    {
        SDL_SemWait(sem);
        SDL_mutexP(mutex);
        Job j = jobs[--numJobs];
        SDL_mutexV(mutex);

        if (j.type == 0)
            raytraceSub(*j.img, *j.cam, j.x, j.y, j.w, j.h);
        else
            assert(0);

        SDL_mutexP(mutex);
        jobsLeft--;
        SDL_mutexV(mutex);
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

    screen.resize(256, 256);

    sem = SDL_CreateSemaphore(0);
    mutex = SDL_CreateMutex();

    SDL_Thread* th[num_threads];
    for (int i = 0; i < num_threads; i++)
        th[i] = SDL_CreateThread(thread_func, (void*)i);

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

