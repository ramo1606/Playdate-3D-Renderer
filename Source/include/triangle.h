#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "utils.h"
#include "logging.h"

/* Triangle structures */

/**
 * @brief Represents a face in a mesh.
 *
 * A face is defined by three vertex indices and three texture coordinate indices.
 */
typedef struct
{
    int a, b, c;        /* Vertex indices */
    //int uva, uvb, uvc;  /* Texture coordinate indices */
} Face;

/**
 * @brief Represents a triangle in 2D space.
 *
 * A triangle is defined by three points in 2D space.
 */
typedef struct
{
    Vector2D points[3];  /* The three points of the triangle */
} Triangle2D;

#endif /* TRIANGLE_H */