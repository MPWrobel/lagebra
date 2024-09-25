#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#define NONE  0
#define I_HAT 1
#define J_HAT 2

static Vector2 GetRelativeMousePosition();
static Vector2 GetWindowSize();
static void RenderGrid();
static void RenderPoint(Vector2, Color);
static void RenderVector(Vector2, Color);

static int block_size = 64;
static int selected_vector = NONE;

static Vector2 i_hat = {1, 0};
static Vector2 j_hat = {0, 1};

void
Init()
{
	puts("initializing...");
}

void
Deinit()
{
	puts("deinitializing...");
}

void
Update()
{
  switch (GetKeyPressed()) {
  case KEY_I: 
    selected_vector = I_HAT;
    break;
  case KEY_J:
    selected_vector = J_HAT;
    break;
  case KEY_SPACE:
    selected_vector = NONE;
    break;
  }

  switch (selected_vector) {
  case I_HAT: 
    i_hat = GetRelativeMousePosition();
    break;
  case J_HAT:
    j_hat = GetRelativeMousePosition();
    break;
  }
}

void
Draw()
{
  BeginDrawing();

  ClearBackground(BLACK);

  RenderGrid();

  RenderVector(i_hat, RED);
  RenderVector(j_hat, BLUE);

  Vector2 mouse_position = GetRelativeMousePosition();
  DrawText(TextFormat("%.02f, %.02f", mouse_position.x, mouse_position.y), 16, 16, 32, YELLOW);

  switch (selected_vector) {
  case NONE: 
    DrawText("NONE", 16, 64, 32, YELLOW);
    break;
  case I_HAT: 
    DrawText("I_HAT", 16, 64, 32, YELLOW);
    break;
  case J_HAT:
    DrawText("J_HAT", 16, 64, 32, YELLOW);
    break;
  default:
    DrawText("UNKNOWN", 16, 64, 32, YELLOW);
    break;
  }

  EndDrawing();
}

static Vector2 GetRelativeMousePosition()
{
  Vector2 vector = GetMousePosition();

  vector = Vector2Subtract(vector, Vector2Scale(GetWindowSize(), 0.5f));
  vector = Vector2Scale(vector, 1.0f / block_size);
  vector.y *= -1;

  return vector;
}

static Vector2 GetWindowSize()
{
  int width = GetRenderWidth() / GetWindowScaleDPI().x;
  int height = GetRenderHeight() / GetWindowScaleDPI().y;

  return (Vector2){width, height};
}

static void RenderGrid()
{
  Vector2 size = GetWindowSize();
  Vector2 segments = (Vector2){size.x / block_size, size.y / block_size};

  for (int i = 1; i < segments.x; ++i) {
    int x = size.x / segments.x * i;
    Vector2 start = {x, 0}, end = {x, size.y};
    DrawLineEx(start, end, 1.0f, WHITE);
  }

  for (int i = 1; i < segments.y; ++i) {
    int y = size.y / segments.y * i;
    Vector2 start = {0, y}, end = {size.x, y};
    DrawLineEx(start, end, 1.0f, WHITE);
  }

  RenderPoint(Vector2Scale(size, 0.5f), YELLOW);
}

static void RenderPoint(Vector2 position, Color color)
{
  Vector2 size = (Vector2){8, 8};
  position = Vector2Subtract(position,  Vector2Scale(size, 0.5f));

  DrawRectangleV(position, size, color);
}

static void RenderVector(Vector2 vector, Color color)
{
  vector = (Vector2){vector.x * block_size, -vector.y * block_size};

  Vector2 size = GetWindowSize();
  Vector2 start = (Vector2){size.x / 2, size.y / 2};
  Vector2 end = Vector2Add(start, (Vector2){vector.x, vector.y});

  DrawLineEx(start, end, 2.0f, color);
  RenderPoint(end, color);
}
