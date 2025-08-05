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

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(4 * WIDTH, 800, "Computer Algebra Solver");
  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    // Update
    int scaling = GetRenderWidth() / WIDTH;
    int image_width = scaling * WIDTH;
    int image_height = scaling * HEIGHT;

    // Draw
    BeginDrawing();

    ClearBackground(GRAY);
    for (int i = 0; i < SIZE; i++) {
      Color c = WHITE;
      if (buffer[i] != 0) {
        c = BLACK;
      }

      DrawRectangle((GetRenderWidth() - scaling * WIDTH) / 2.0 +
                        scaling * (i % WIDTH),
                    scaling * (i / WIDTH), scaling, scaling, c);
    }

    EndDrawing();
  }

  CloseWindow();
}
