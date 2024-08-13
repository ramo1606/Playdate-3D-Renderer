#include "global.h"
#include "display.h"
#include "logging.h"

/* Grid configuration constants */
#define GRID_OFFSET 5
#define GRID_SPACING 10

/* Static variables for display information */
static int displayRowBytes = 0;
static int displayWidth = 0;
static int displayHeight = 0;

static uint8_t* frameBuffer = NULL;

int initDisplay(void)
{
    uint8_t* bitMapMask = NULL;

    // Get display buffer bitmap information and framebuffer pointer
    pd->graphics->getBitmapData(
        pd->graphics->getDisplayBufferBitmap(),
        &displayWidth,
        &displayHeight,
        &displayRowBytes,
        &bitMapMask,
        &frameBuffer
    );
    return 0;
}

void drawPixel(int x, int y, LCDSolidColor color)
{
    // Bounds checking
    if (x < 0 || x >= displayWidth || y < 0 || y >= displayHeight)
    {
        LOG_WARNING("drawPixel: Coordinates out of bounds (%d, %d)", x, y);
        return;
    }

    uint8_t* block = frameBuffer + (y * displayRowBytes) + (x / 8);
    uint8_t data = 0x80 >> (x % 8);
    *block = color ? *block | data : *block & ~data;
}

void drawLine(int x0, int y0, int x1, int y1, LCDSolidColor color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    {  /* loop */
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void drawTriangle(Triangle2D triangle, LCDSolidColor color)
{
    drawLine((int)triangle.points[0].x, (int)triangle.points[0].y,
        (int)triangle.points[1].x, (int)triangle.points[1].y, color);
    drawLine((int)triangle.points[1].x, (int)triangle.points[1].y,
        (int)triangle.points[2].x, (int)triangle.points[2].y, color);
    drawLine((int)triangle.points[2].x, (int)triangle.points[2].y,
        (int)triangle.points[0].x, (int)triangle.points[0].y, color);
}

void drawRect(int x, int y, int width, int height, LCDSolidColor color)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            drawPixel(i, j, color);
        }
    }
}

void drawGrid(LCDSolidColor color)
{
    // Bounds checking
    int x = GRID_OFFSET, y = GRID_OFFSET;

    while (y < displayHeight)
    {
        while (x < displayWidth)
        {
            drawPixel(x, y, color);
            x += GRID_SPACING;
        }
        x = GRID_OFFSET;
        y += GRID_SPACING;
    }
}

void clearFramebuffer(LCDSolidColor color)
{
    // Get a pointer to the current framebuffer 
    frameBuffer = pd->graphics->getFrame();
    memset(frameBuffer, color ? 0xFF : 0x00, displayRowBytes * displayHeight);
}

void renderBuffer(void)
{
    pd->graphics->markUpdatedRows(0, displayHeight - 1);
}