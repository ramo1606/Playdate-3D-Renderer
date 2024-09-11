#ifndef DISPLAY_H
#define DISPLAY_H

#include "pd_api.h"
#include "vector.h"
#include "triangle.h"

enum cullingMode
{
	kCullingNone,
	kCullingBackface
} cullingMode;

enum renderMode
{
	kRenderWireframe,
	kRenderWireframeVertex,
	kRenderSolid,
	kRenderSolidWireframe
} renderMode;

/**
 * @brief Initializes the display system.
 *
 * @return int 0 on success, non-zero on failure.
 */
int initDisplay(void);

/**
 * @brief Draws a single pixel on the screen.
 *
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param color Color of the pixel (kColorBlack or kColorWhite).
 */
void drawPixel(int x, int y, LCDSolidColor color);

/**
 * @brief Draws a line between two points.
 *
 * @param x1 X-coordinate of the start point.
 * @param y1 Y-coordinate of the start point.
 * @param x2 X-coordinate of the end point.
 * @param y2 Y-coordinate of the end point.
 * @param color Color of the line (kColorBlack or kColorWhite).
 */
void drawLine(int x0, int y0, int x1, int y1, LCDSolidColor color);

/**
 * @brief Draws a triangle.
 *
 * @param triangle The triangle to draw.
 * @param color Color of the triangle (kColorBlack or kColorWhite).
 */
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color);

/**
 * @brief Draws a filled triangle.
 *
 * @param triangle The triangle to draw.
 * @param color Color of the triangle (kColorBlack or kColorWhite).
 */
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color);

/**
 * @brief Draws a rectangle.
 *
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color of the rectangle (kColorBlack or kColorWhite).
 */
void drawRect(int x, int y, int width, int height, LCDSolidColor color);

/**
 * @brief Draws a grid on the screen.
 *
 * @param color Color of the grid lines (kColorBlack or kColorWhite).
 */
void drawGrid(LCDSolidColor color);

/**
 * @brief Clears the framebuffer.
 *
 * @param color Color to clear with (kColorBlack or kColorWhite).
 */
void clearFramebuffer(LCDSolidColor color);

/**
 * @brief Renders the current framebuffer to the screen.
 */
void renderBuffer(void);

#endif /* DISPLAY_H */