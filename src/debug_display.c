#include "draw.h"

#include <raylib.h>

#define WIDTH 192
#define HEIGHT 64
#define SIZE WIDTH *HEIGHT

void debug_display() {
  // I'll design the drawing code able to be turned into a bitboard if memory
  // usage requires it. Right now though, using chars would use ~12kb out of
  // 128kb, so I won't optimize until I have to
  char buffer[SIZE] = {};

  clear_display(buffer, SIZE);

  for (int i = 0; i < HEIGHT; i++) {
    set_pixel(i, i, ON, buffer, WIDTH);
  }

  InitWindow(WIDTH, HEIGHT, "Computer Algebra Solver");
  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    // Update

    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
}
