#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "math.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "raytracer.hpp"
#include "plotpixels.hpp"
#include "camerapath.hpp"
#include "music.hpp"
#include "wobbler.hpp"
#include "blur.hpp"

using namespace kd;

static int num_threads   = 2;
static int screen_width  = 600;
static int screen_height = 600;
static SDL_sem* sem;
static SDL_mutex* mutex;
static Image screen;
static Image screen2;
static float demoTime;
static struct PlotPixels pixels;
static RayTracer rt;
static Camera camera;

static float demoLength = 2 * 60.f + 15.f;
static Music music("assets/musa.ogg", 130.0);

static Image testImg("assets/title.png");

//
// Job.
//

enum JobType
{
    RAY_TRACE,
    PLOT_PIXEL,
};

struct Job
{
    int type;
    int x, y, w, h;
    Image* img;
    const Camera* cam;
    RayTracer* rt;
    PlotPixels* plotPixels;
};

static Job jobs[128];
static volatile int numJobs;
static volatile int jobsLeft;

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

void raytrace(Image& dst, const Camera& cam)
{
#if 1
    const int slices = 8;
    for (int i = 0; i < slices; i++)
    {
        int y0 = dst.h * i / slices;
        int y1 = dst.h * (i+1) / slices;

        Job j;
        j.type = RAY_TRACE;
        j.x = 0;
        j.y = y0;
        j.w = dst.w;
        j.h = y1 - y0;
        j.img = &dst;
        j.cam = &cam;
        j.rt = &rt;
        putJob(j);
    }

    while (!allJobsDone())
        SDL_Delay(1);
#endif

    //raytraceSub(rt, 0, 0, dst.w, dst.h);

    plotPixels(dst, cam, pixels);
}

static int thread_func(void* id)
{
    for (;;)
    {
        SDL_SemWait(sem);
        SDL_mutexP(mutex);
        Job j = jobs[--numJobs];
        SDL_mutexV(mutex);

        if (j.type == RAY_TRACE)
            raytraceSub(*j.rt, j.x, j.y, j.w, j.h);
        else if (j.type == PLOT_PIXEL)
            plotPixels(*j.img, *j.cam, *j.plotPixels);
        else
            assert(0);

        SDL_mutexP(mutex);
        jobsLeft--;
        SDL_mutexV(mutex);
    }
}

static void render()
{
#if 0
    glClearColor(0.2f, 0.4f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
#endif

#if 0
    for (int y = 0; y < screen.h; y++)
        for (int x = 0; x < screen.w; x++)
            screen.data[y*screen.w+x] = x*256+y*3;
#endif

    camera.position.x = cosf(demoTime*3.14159265f*2.f * 0.1f) * 10.f;
    camera.position.y = sinf(demoTime*3.14159265f*2.f * 0.2f) * 3.f + 10.f;
    camera.position.z = sinf(demoTime*3.14159265f*2.f * 0.1f) * 10.f;
    camera.fov = 3.14159265f*2.f * 90.f / 360.f;
    camera.update();

    rt.camera = camera;

    if (!rt.image)
    {
        rt.image = &screen;
        rt.update();
    }

    raytrace(screen, camera);

    float strength = demoTime / demoLength * 10.f;
    wobbler(screen2, screen, demoTime * 30.f, demoTime* 23.4f, strength);

    blurh(screen, screen2);

    if (demoTime > 10.f)
    {
        for (int i = 0; i< 4; i++)
        {
            blurh(screen2, screen);
            blurh(screen, screen2);
            blurv(screen2, screen);
            blurv(screen, screen2);
        }
    }

    putImageFullScreen(screen);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_SetVideoMode(screen_width, screen_height, 0, SDL_OPENGL);

    IMG_Init(IMG_INIT_PNG);

    music.init();

    //pixels.create(256);
    //generateSphere(pixels, Vector3f(0.f, 0.f, 0.f), 4.f, 128);

    pixels.create(testImg.w * testImg.h);
    plotImage(pixels, Vector3f(-2.f, -4.f, 0.f), testImg, 4.f, 4.f);

    screen.resize(256, 256);
    screen2.resize(256, 256);

    camera.targetCamera = false;
    camera.translateLocal(Vector3f(0.f, -1.f, 0.f));

    sem = SDL_CreateSemaphore(0);
    mutex = SDL_CreateMutex();

    music.play();

    SDL_Thread* th[num_threads];
    for (int i = 0; i < num_threads; i++)
        th[i] = SDL_CreateThread(thread_func, (void*)i);

    int ticks = SDL_GetTicks();
    int mouseX, mouseY;

    int lastRecordTime = 0;
    bool recordMode = false;
    bool playMode = false;
    CameraPath recordPath;

    for (;;)
    {
        int deltaTicks = SDL_GetTicks() - ticks;
        ticks += deltaTicks;
        float dt = deltaTicks / 1000.f;

        Uint8 *keys = SDL_GetKeyState(NULL);
        if (keys[SDLK_ESCAPE])
            break;

        static float scale = 10.5f;

        if (SDL_GetKeyState(0)[SDLK_KP_PLUS])
        {
            scale *= expf(dt);
            printf("speed %f\n", scale);
        }
        if (SDL_GetKeyState(0)[SDLK_KP_MINUS])
        {
            scale /= expf(dt);
            printf("speed %f\n", scale);
        }
        if (SDL_GetKeyState(0)[SDLK_w])
            camera.translateLocal(Vector3f(0.f, 0.f, scale) * dt);
        if (SDL_GetKeyState(0)[SDLK_s])
            camera.translateLocal(Vector3f(0.f, 0.f, -scale) * dt);
        if (SDL_GetKeyState(0)[SDLK_a])
            camera.translateLocal(Vector3f(scale, 0.f, 0.f) * dt);
        if (SDL_GetKeyState(0)[SDLK_d])
            camera.translateLocal(Vector3f(-scale, 0.f, 0.f) * dt);

        if (playMode)
        {
            camera.view = recordPath.get(SDL_GetTicks() / 1000.f);
        }

        if (recordMode)
        {
            if (SDL_GetTicks() - lastRecordTime >= 10)
            {
                lastRecordTime = SDL_GetTicks();
                recordPath.add(camera.view);
                recordPath.save("camera.txt");
            }
        }

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                break;

            if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_r)
                {
                    lastRecordTime = SDL_GetTicks();
                    recordMode = true;
                }

                if (ev.key.keysym.sym == SDLK_p)
                {
                    recordPath.load("camera.txt");
                    printf("%d frames\n", recordPath.frames.size());
                    demoTime = 0.f;
                    playMode = true;
                }

                if (ev.key.keysym.sym == SDLK_ESCAPE)
                    break;
            }

            if (ev.type == SDL_MOUSEMOTION)
            {
                if (ev.motion.state & SDL_BUTTON(1))
                {
                    float dx = (ev.motion.x - mouseX) / float(screen_width) * 3.14159265f * 3.3f;
                    float dy = (ev.motion.y - mouseY) / float(screen_height) * 3.14159265f * 3.3f;

                    camera.rotateLocal(Vector3f(0.f, 1.f, 0.f), dx);
                    camera.rotateLocal(Vector3f(1.f, 0.f, 0.f), dy);
                }

                mouseX = ev.motion.x;
                mouseY = ev.motion.y;
            }
        }

        demoTime += dt;

        render();
        SDL_GL_SwapBuffers();
    }

    IMG_Quit();
}

