#include <raylib.h>

int main(int argc, char *argv[]) {
    InitWindow(800, 600, "Pong");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Hello, Pong!", 300, 200, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}