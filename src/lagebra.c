#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#define NONE  0
#define I_HAT 1
#define J_HAT 2

static Vector2 GetRelativeMousePosition();
static Vector2 GetWindowSize();
static void RenderGrid();
static void RenderLine(Vector2, Vector2, Color);
static void RenderRectangle(Vector2, Vector2, Color);
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
	rlDisableBackfaceCulling();
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

  RenderRectangle((Vector2){-2 * block_size, 0}, (Vector2){block_size, block_size}, BLUE);

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

static void RenderRectangle(Vector2 position, Vector2 size, Color color) {
	Vector2 translation = Vector2Scale(GetWindowSize(), 0.5f);
	Matrix matrix = {.m0 = i_hat.x, .m1 = -i_hat.y, .m4 = j_hat.x, .m5 = -j_hat.y};
	Rectangle rec = {position.x, position.y, size.x, size.y};
	Vector2 topLeft = { rec.x, rec.y };
	Vector2 topRight = { rec.x + rec.width, rec.y };
	Vector2 bottomLeft = { rec.x, rec.y + rec.height };
	Vector2 bottomRight = { rec.x + rec.width, rec.y + rec.height };

	topLeft = Vector2Transform(topLeft, matrix);
	topRight = Vector2Transform(topRight, matrix);
	bottomLeft = Vector2Transform(bottomLeft, matrix);
	bottomRight = Vector2Transform(bottomRight, matrix);

	topLeft = Vector2Add(topLeft, translation);
	topRight = Vector2Add(topRight, translation);
	bottomLeft = Vector2Add(bottomLeft, translation);
	bottomRight = Vector2Add(bottomRight, translation);

	rlBegin(RL_TRIANGLES);

	rlColor4ub(color.r, color.g, color.b, color.a);

	rlVertex2f(bottomRight.x, bottomRight.y);
	rlVertex2f(bottomLeft.x, bottomLeft.y);
	rlVertex2f(topRight.x, topRight.y);

	rlVertex2f(topRight.x, topRight.y);
	rlVertex2f(bottomLeft.x, bottomLeft.y);
	rlVertex2f(topLeft.x, topLeft.y);

	rlEnd();
}

static void RenderLine(Vector2 start, Vector2 end, Color color)
{
	Matrix matrix = {.m0 = i_hat.x, .m1 = -i_hat.y, .m4 = j_hat.x, .m5 = -j_hat.y};
	Vector2 translation = Vector2Scale(GetWindowSize(), 0.5f);
	start = Vector2Transform(start, matrix);
	end   = Vector2Transform(end, matrix);
  	start = Vector2Add(start, translation);
  	end   = Vector2Add(end, translation);
    DrawLineEx(start, end, 1.0f, WHITE);
}

static void RenderGrid()
{
  Vector2 window_size = GetWindowSize();
  Vector2 segments = (Vector2){
	  window_size.x / block_size,
	  window_size.y / block_size};

  for (int i = 1; i < segments.x; ++i) {
    int x = window_size.x / segments.x * i - window_size.x / 2;
    Vector2 start = {x, -window_size.y / 2 }, end = {x, window_size.y / 2};
	RenderLine(start, end, WHITE);
  }

  for (int i = 1; i < segments.y; ++i) {
    int y = window_size.y / segments.y * i - window_size.y / 2;
    Vector2 start = {-window_size.x / 2, y}, end = {window_size.x / 2, y};
	RenderLine(start, end, WHITE);
  }

  RenderPoint(Vector2Scale(window_size, 0.5f), YELLOW);
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
