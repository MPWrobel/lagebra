#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdio.h>

#include "lagebra.h"

static int block_size = 16;

static Vector2 i_hat = {1, 0};
static Vector2 j_hat = {0, 1};
static HatVector selected_vector = NONE;

static Image  image;
static Color *pixels;

void
Init(void)
{
    puts("initializing...");

    rlDisableBackfaceCulling();

    image  = LoadImage("assets/bananas.png");
    pixels = LoadImageColors(image);
}

void
Deinit(void)
{
    puts("deinitializing...");

    UnloadImage(image);
    UnloadImageColors(pixels);
}

void
Update(void)
{
    int key_pressed = GetKeyPressed();

    if (key_pressed == KEY_SPACE) selected_vector = NONE;
    else if (key_pressed == KEY_I) selected_vector = I_HAT;
    else if (key_pressed == KEY_J) selected_vector = J_HAT;

    if (selected_vector == I_HAT) i_hat = GetRelativeMousePosition();
    else if (selected_vector == J_HAT) j_hat = GetRelativeMousePosition();
}

void
Draw(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    RenderImage(image, pixels);
    RenderGrid();
    RenderVector(i_hat, RED);
    RenderVector(j_hat, BLUE);
    RenderSelectedVectorDimensions((Vector2){16, 16});

    EndDrawing();
}

static Vector2
GetRelativeMousePosition(void)
{
    Vector2 vector = GetMousePosition();
    vector = Vector2Subtract(vector, Vector2Scale(GetWindowSize(), 0.5f));
    vector = Vector2Scale(vector, 1.0f / block_size);
    vector.y *= -1;
    return vector;
}

static Vector2
GetWindowSize(void)
{
    int width  = GetRenderWidth() / GetWindowScaleDPI().x;
    int height = GetRenderHeight() / GetWindowScaleDPI().y;
    return (Vector2){width, height};
}

static Matrix 
GetTranslationMatrix(void)
{
    return (Matrix){
        .m0 =  i_hat.x, .m4 =  j_hat.x,
        .m1 = -i_hat.y, .m5 = -j_hat.y
    };
}

static void
RenderRectangle(Vector2 position, Vector2 size, Color color)
{
    Vector2 translation = Vector2Scale(GetWindowSize(), 0.5f);
    Matrix  matrix      = GetTranslationMatrix();

    Vector2   topLeft     = {position.x, position.y};
    Vector2   topRight    = {position.x + size.x, position.y};
    Vector2   bottomLeft  = {position.x, position.y + size.y};
    Vector2   bottomRight = {position.x + size.x, position.y + size.y};

    topLeft     = Vector2Transform(topLeft, matrix);
    topRight    = Vector2Transform(topRight, matrix);
    bottomLeft  = Vector2Transform(bottomLeft, matrix);
    bottomRight = Vector2Transform(bottomRight, matrix);

    topLeft     = Vector2Add(topLeft, translation);
    topRight    = Vector2Add(topRight, translation);
    bottomLeft  = Vector2Add(bottomLeft, translation);
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

static void
RenderLine(Vector2 start, Vector2 end, Color color)
{
    Vector2 translation = Vector2Scale(GetWindowSize(), 0.5f);
    Matrix  matrix      = GetTranslationMatrix();

    start = Vector2Transform(start, matrix);
    end   = Vector2Transform(end, matrix);
    start = Vector2Add(start, translation);
    end   = Vector2Add(end, translation);

    DrawLineEx(start, end, 1.0f, WHITE);
}

static void
RenderGrid(void)
{
    Vector2 window_size = GetWindowSize();
    Vector2 segments = (Vector2){
        window_size.x / block_size,
        window_size.y / block_size
    };

    for (int i = 1; i < segments.x; ++i) {
        int x = window_size.x / segments.x * i - window_size.x / 2;
        RenderLine((Vector2){x, -window_size.y / 2},
                   (Vector2){x, window_size.y / 2},
                   WHITE);
    }

    for (int i = 1; i < segments.y; ++i) {
        int y = window_size.y / segments.y * i - window_size.y / 2;
        RenderLine((Vector2){-window_size.x / 2, y},
                   (Vector2){window_size.x / 2, y},
                   WHITE);
    }

    RenderPoint(Vector2Scale(window_size, 0.5f), YELLOW);
}

static void
RenderPoint(Vector2 position, Color color)
{
    Vector2 size = (Vector2){8, 8};
    position = Vector2Subtract(position, Vector2Scale(size, 0.5f));
    DrawRectangleV(position, size, color);
}

static void
RenderVector(Vector2 vector, Color color)
{
    vector = (Vector2){vector.x * block_size, -vector.y * block_size};

    Vector2 size  = GetWindowSize();
    Vector2 start = (Vector2){size.x / 2, size.y / 2};
    Vector2 end   = Vector2Add(start, (Vector2){vector.x, vector.y});

    DrawLineEx(start, end, 2.0f, color);
    RenderPoint(end, color);
}

static void
RenderImage(Image image, Color* pixels)
{
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            RenderRectangle((Vector2){(int)(x - image.width / 2) * block_size,
                                      (int)(y - image.height / 2) * block_size},
                            (Vector2){block_size, block_size},
                            pixels[(image.height - 1 - y) * image.width + x]);
        }
    }
}

static void
RenderSelectedVectorDimensions(Vector2 position)
{
    int font_size = 32;
    Vector2 mouse_position = GetRelativeMousePosition();

    DrawText(TextFormat("%.02f, %.02f", mouse_position.x, mouse_position.y),
             position.x, position.y, font_size, YELLOW);

    position.y += font_size;

    if (selected_vector == NONE) {
        DrawText("NONE", position.x, position.y, 32, YELLOW);
    } else if (selected_vector == I_HAT) {
        DrawText("I_HAT", position.x, position.y, 32, YELLOW);
    } else if (selected_vector == J_HAT) {
        DrawText("J_HAT", position.x, position.y, 32, YELLOW);
    } else {
        DrawText("UNKNOWN", 16, 64, 32, YELLOW);
    }
}
