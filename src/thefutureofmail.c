/*******************************************************************************************
*
*   TheFutureOfMail
*
*   This is a recreation of my rust game "The Future of Mail" written in C
*
********************************************************************************************/

#include "raylib.h"
#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

struct customCamera {
    float radius;
    float yaw;
    float pitch;
} customCamera;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera camera = { 0 };
Vector3 Position = { 0 };

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "The Future of Mail");

    camera.position = (Vector3){ 3.0f, 3.0f, 2.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    struct customCamera mainCam = {0};
    //mainCam.radius = 350.0f;
    mainCam.radius = 100.0f;
    mainCam.yaw = PI;
    mainCam.pitch = 1.0f;
    Model PlayerModel = LoadModel("resources/Models/bot_main.glb");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT)) mainCam.yaw += 0.02f;
        if (IsKeyDown(KEY_LEFT)) mainCam.yaw -= 0.02f;
        if (IsKeyDown(KEY_UP)) mainCam.pitch += 0.02f;
        if (IsKeyDown(KEY_DOWN)) mainCam.pitch -= 0.02f;
        mainCam.pitch = Clamp(mainCam.pitch, -PI/2 + 0.05f, PI/2 - 0.05f);
        camera.position.x = camera.target.x + mainCam.radius * cosf(mainCam.pitch) * sinf(mainCam.yaw);
        camera.position.y = camera.target.y + mainCam.radius * sinf(mainCam.pitch);
        camera.position.z = camera.target.z + mainCam.radius * cosf(mainCam.pitch) * cosf(mainCam.yaw);
        // Update
    //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_CUSTOM);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawModel(PlayerModel, Position, 1.0f, WHITE);
            DrawGrid(10, 1.0f);
        EndMode3D();

        DrawText("Make contact with people and deliver mail to earn points\nWarning: Watch out for buildings", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
