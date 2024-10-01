#ifndef LAGEBRA_H
#define LAGEBRA_H

#include <raylib.h>

typedef enum {
    NONE,
    I_HAT,
    J_HAT,
} HatVector;

void Init(void);
void Deinit(void);
void Update(void);
void Draw(void);

static Vector2 GetRelativeMousePosition(void);
static Vector2 GetWindowSize(void);
static Matrix  GetTranslationMatrix(void);

static void RenderGrid(void);
static void RenderLine(Vector2, Vector2, Color);
static void RenderRectangle(Vector2, Vector2, Color);
static void RenderPoint(Vector2, Color);
static void RenderVector(Vector2, Color);
static void RenderImage(Image, Color*);
static void RenderSelectedVectorDimensions(Vector2);

#endif // LAGEBRA_H
