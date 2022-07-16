#include "raylib.h"

#include "game/game.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// Compile WEB version:
// rm -R ./assets
// cp -R ../assets ./assets
// em++ ../src/main.cpp ../src/game/game.cpp ../src/systems.cpp -Os -Wall -std=c++20 -o index.html /Users/gerodrus/Downloads/raylib-4.0.0/src/libraylib.a -L/Users/gerodrus/Downloads/raylib-4.0.0/src/ -I/Users/gerodrus/Downloads/raylib-4.0.0/src/ -lraylib -DPLATFORM_WEB -s USE_GLFW=3 --preload-file assets
// python3 -m http.server

Game game {};

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame()
{
    // Update
    //----------------------------------------------------------------------------------
    game.update();
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BLACK);

    DrawText("Congrats! You created your first window!", 20, 20, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 1024;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    game.init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
