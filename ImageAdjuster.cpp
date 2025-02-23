//
// Created by henrik on 23/02/2025.
//

#include "ImageAdjuster.h"
#include "raylib.h"

namespace camTranslator {
    void ImageAdjuster::adjust_image() {
        const int screenWidth = 800;
        const int screenHeight = 600;

        InitWindow(screenWidth, screenHeight, "Hello World");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            EndDrawing();
        }
    }
}