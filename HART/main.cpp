#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Threading/Allocator.h"

#include "Threading/JobObject.h"
#include "Threading/ThreadPool.h"

#include "Math/Geometry.h"
#include "Shapes/Sphere.h"
#include "Shapes/Plane.h"
#include "RayTracing/Camera.h"
#include "RayTracing/Material.h"

#include "RayCastJob.h"

#undef min
#undef max

uint32_t RandomBetween(uint32_t From, uint32_t To, hart::Rng32& PRNG)
{
    return From + (PRNG() % ((To + 1) - From));
}


hart::CameraConfig Scene_BokehDepthOfField(hart::GeometryList* Scene, hart::Rng32& Prng)
{
    float Width  = 40.0f;
    float Length = 40.0f;

    hart::vec3 BottomLeft = hart::vec3(0.0f - (Width * 0.5f), 0.0f, 0.0f + (Length * 0.5f));

    Scene->PushGeometry(new hart::Parallelogram(BottomLeft, hart::vec3(1.0f, 0.0f, 0.0f), hart::vec3(0.0f, 0.0f, -1.0f), Width, Length, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.3f, 0.43f, 0.95f)))));

    for (size_t x = 0; x < 5; x++)
    {
        for (size_t y = 0; y < 5; y++)
        {
            for (size_t z = 0; z < 5; z++)
            {
                uint32_t RandomValue = RandomBetween(1, 5, Prng);

                hart::BaseMaterial* Material = nullptr;

                switch (RandomValue)
                {
                case 1:
                    Material = new hart::Metal(new hart::BaseTexture(hart::rgb(1.0f, 0.647f, 0.00f)), 0.2f);       // Gold
                    break;
                case 2:
                    Material = new hart::Metal(new hart::BaseTexture(hart::rgb(0.753f, 0.753f, 0.753f)), 0.2f);    // Silver
                    break;
                case 3:
                    Material = new hart::Metal(new hart::BaseTexture(hart::rgb(0.95f, 0.64f, 0.54f)), 0.2f);       // Copper
                    break;
                case 4:
                    Material = new hart::Dielectric(new hart::BaseTexture(hart::rgb(1.0f, 1.0f, 1.0f)), 1.52f);    // Glass
                    break;
                case 5:
                    Material = new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.1f, 0.1f, 0.1f)));   // Diffused
                    break;
                }

                Scene->PushGeometry(new hart::Sphere(hart::vec3((float)x, (float)y + 0.5f, -(float)z), 0.5f, Material));
            }
        }
    }

    // Back bokeh.
    Scene->PushGeometry(new hart::Sphere(hart::vec3( -5.0f, 5.0f, -55.0f), 1.2f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(1.0f,  0.11f, 0.23f)), 4.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3( -2.0f, 7.0f, -43.0f), 1.5f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(1.0f,  0.75f, 0.22f)), 6.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3(  5.0f, 5.0f, -40.0f), 2.0f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.23f, 0.44f, 1.0f)), 10.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3( 10.0f, 2.0f, -40.0f), 1.0f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.45f, 0.21f, 0.63f)), 7.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3(  7.0f, 7.0f, -50.0f), 2.0f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.73f, 0.33f, 1.0f)),  6.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3(-10.0f, 4.0f, -20.0f), 0.1f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.23f, 0.44f, 1.0f)), 50.0f)));

    // Front bokeh.
    Scene->PushGeometry(new hart::Sphere(hart::vec3(5.0f, 2.0f, 10.0f), 0.1f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.23f, 0.44f,  1.0f)), 20.0f)));
    Scene->PushGeometry(new hart::Sphere(hart::vec3(8.0f, 2.0f,  5.0f), 0.1f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(0.45f, 0.21f, 0.63f)), 20.0f)));

    hart::CameraConfig Config;

    Config.LookFrom = hart::vec3(-5.0f, 7.0f, 20.0f);;
    Config.LookTo   = hart::vec3(0.0f, 0.0f, 0.0f);
    Config.Aperture = 5.0f;
    Config.Distance = (Config.LookFrom - Config.LookTo).Length();
    Config.ViewAngle = 45.0f;

    return Config;
}


hart::CameraConfig Scene_Corridor(hart::GeometryList* Scene)
{
    float FWidth    = 10.0f;
    float FLength   = 36.0f;
    hart::vec3 FloorBL = hart::vec3(0.0f - (FWidth * 0.5f), 0.0f, -1.0f);

    // Floor.
    Scene->PushGeometry(new hart::Parallelogram(FloorBL, hart::vec3::Right, hart::vec3::Back, FWidth, FLength, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));
    
    // Right Wall.
    Scene->PushGeometry(new hart::Parallelogram(hart::vec3(5.0f, 0.0f, -37.0f), hart::vec3::Front, hart::vec3::Up, FLength, 10.0f, new hart::LambertianDiffused(new hart::GradientTexture((size_t)FLength, 10))));

    // Hole cover bottom.
    Scene->PushGeometry(new hart::Parallelogram(hart::vec3(-15.0f, 3.0f, -1.0f), hart::vec3::Right, hart::vec3::Back, 10.0f, 40.0f, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));

    // Hole cover top.
    Scene->PushGeometry(new hart::Parallelogram(hart::vec3(2.0f, 10.0f, -37.0f), hart::vec3::Front, hart::vec3::Up, FLength, 10.0f, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));

    // Sunlight.
    //Scene->PushGeometry(new hart::Sphere(hart::vec3(-30.0f, 30.0f, -21.0f), 15.0f, new hart::SunLight(new hart::BaseTexture(hart::rgb(1.0f)), hart::vec3(1.0f, -1.0f, 0.0f), 10.0f)));

    float LWHeight  = 0.0f;
    float TWWidth   = 0.0f;

    size_t LIndex = 0;
    //const hart::rgb LightCol[4] = { hart::rgb(1.0f, 0.22f, 0.13f), hart::rgb(0.22f, 1.0f, 0.13f), hart::rgb(0.22f, 0.13f, 1.0f), hart::rgb(1.0f) };

    for (size_t i = 0; i < 9; i++)
    {
        float x = -5.0f;

        LWHeight = 10.0f;
        TWWidth  = 10.0f;

        if ((i & 1))
        {
            LWHeight = 3.0f;
            TWWidth  = 3.0f;
            x = 2.0f;

            //Scene->PushGeometry(new hart::Sphere(hart::vec3(-20.0f, 20.0f, -1.0f - float((6 * (LIndex + 1)))), 4.0f, new hart::DiffusedLight(new hart::BaseTexture(hart::rgb(1.0f)), 10.0f)));
            float z = -1.0f - (float)(8 * (++LIndex));
            Scene->PushGeometry(new hart::Parallelogram(hart::vec3(-15.0f,  3.0f, z), hart::vec3::Right, hart::vec3::Up, 10.0f, 20.0f, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));
            Scene->PushGeometry(new hart::Parallelogram(hart::vec3( -5.0f, 10.0f, z), hart::vec3::Right, hart::vec3::Up, 7.0f, FLength, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));
        }

        // Left wall.
        Scene->PushGeometry(new hart::Parallelogram(hart::vec3(-5.0f, 0.0f, -1.0f - (float)(4 * i)), hart::vec3::Back, hart::vec3::Up, 4.0f, LWHeight, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));
        // Top wall.
        Scene->PushGeometry(new hart::Parallelogram(hart::vec3(x, 10.0f, -1.0f - (float)(4 * i)), hart::vec3::Back, hart::vec3::Right, 4.0f, TWWidth, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));
    }

    // Back wall.
    Scene->PushGeometry(new hart::Parallelogram(hart::vec3(-5.0f, 0.0f, -37.0f), hart::vec3::Right, hart::vec3::Up, FWidth, 10.0f, new hart::LambertianDiffused(new hart::BaseTexture(hart::rgb(0.89f, 0.89f, 0.89f)))));

    hart::CameraConfig Config;

    Config.LookFrom = hart::vec3(0.0f, 5.0f, 8.0f);
    Config.LookTo   = hart::vec3(0.0f, 5.0f, 0.0f);
    Config.Aperture = 0.0f;
    Config.Distance = (Config.LookFrom - Config.LookTo).Length();
    Config.ViewAngle = 45.0f;

    return Config;
}


hart::CameraConfig Scene_ReflectivePlanels(hart::GeometryList* Scene)
{


    hart::CameraConfig Config;

    return Config;
}


int main()
{
    int Width   = 640;
    int Height  = 480;
    int Samples = 526;
    //int Samples = 16384;

    constexpr int NUM_CHANNEL = 4;

    int TotalPixels = Width * Height * NUM_CHANNEL;

    size_t Index = 0;
    Gfl::uint8* Framebuffer = (Gfl::uint8*)malloc(TotalPixels);

    hart::Rng32         Prng(std::time(0));
    hart::GeometryList  Scene;
    
    //hart::CameraConfig Config = Scene_BokehDepthOfField(&Scene, Prng);
    hart::CameraConfig Config = Scene_Corridor(&Scene);

    hart::HartCamera Camera(Config.LookFrom, Config.LookTo, Config.ViewAngle, Width, Height, Config.Aperture, Config.Distance);

    hart::ThreadPool ThreadPool;

    ThreadPool.Initialise();

    for (int j = 0; j < Height - 1; j++)
    {
        for (int i = 0; i < Width; i++)
        {
            hart::RayCastArgs* Args = new hart::RayCastArgs();
            
            Args->CameraPtr = &Camera;
            Args->PixelPtr = Framebuffer;
            Args->ScenePtr = &Scene;
            Args->Width = Width;
            Args->Height = Height;
            Args->Samples = Samples;
            Args->V = j;
            Args->H = i;
            Args->Index = Index;
            
            ThreadPool.PushJobIntoThread(hart::RayCast, Args);

            Index += 4;
        }
    }

    ThreadPool.Shutdown();
    Scene.Free();

    stbi_write_png("Test.png", Width, Height, NUM_CHANNEL, Framebuffer, Width * NUM_CHANNEL);

    free(Framebuffer);

    return 0;
}