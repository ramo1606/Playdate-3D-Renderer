#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include "logging.h" 
#include "utils.h"

/* Type Definitions */

typedef struct
{
    float x;
    float y;
} Vector2D;

typedef struct
{
    float x;
    float y;
    float z;
} Vector3D;

/* Vector 3D functions */

/**
 * @brief Adds two 2D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The resulting Vector2D.
 */
static inline Vector2D vector2DAdd(Vector2D v1, Vector2D v2)
{
    return Vector2D { v1.x + v2.x, v1.y + v2.y };
}

/**
 * @brief Subtracts two 2D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The resulting Vector2D.
 */
static inline Vector2D vector2DSub(Vector2D v1, Vector2D v2)
{
    return Vector2D { v1.x - v2.x, v1.y - v2.y };
}

/**
 * @brief Multiplies a 2D vector by a scalar.
 * @param v The vector.
 * @param scalar The scalar value.
 * @return The resulting Vector2D.
 */
static inline Vector2D vector2DMul(Vector2D v, float scalar)
{
    return Vector2D{ v.x * scalar, v.y * scalar };
}

/**
 * @brief Divides a 2D vector by a scalar.
 * @param v The vector.
 * @param scalar The scalar value.
 * @return The resulting Vector2D.
 */
static inline Vector2D vector2DDiv(Vector2D v, float scalar)
{
    if (floatIsZero(scalar))
    {
        LOG_ERROR("Division by zero in vector2DDiv");
        return v;
    }
    return Vector2D{ v.x / scalar, v.y / scalar };
}

/**
 * @brief Calculates the dot product of two 2D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The dot product.
 */
static inline float vector2DDot(Vector2D v1, Vector2D v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

/**
 * @brief Calculates the magnitude of a 2D vector.
 * @param v The vector.
 * @return The magnitude.
 */
static inline float vector2DLength(Vector2D v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

/**
 * @brief Normalizes a 2D vector.
 * @param v The vector to normalize.
 * @return The normalized Vector2D.
 */
static inline Vector2D vector2DNormalize(Vector2D v)
{
    float length = vector2DLength(v);
    if (floatIsZero(length))
    {
        LOG_WARNING("Attempt to normalize zero vector");
        return v;
    }
    return vector2DMul(v, 1.0f / length);
}

/* Vector 3D functions */

/**
 * @brief Adds two 3D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The resulting Vector3D.
 */
static inline Vector3D vector3DAdd(Vector3D v1, Vector3D v2)
{
    return Vector3D { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/**
 * @brief Subtracts two 3D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The resulting Vector3D.
 */
static inline Vector3D vector3DSub(Vector3D v1, Vector3D v2)
{
    return Vector3D { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/**
 * @brief Multiplies a 3D vector by a scalar.
 * @param v The vector.
 * @param scalar The scalar value.
 * @return The resulting Vector3D.
 */
static inline Vector3D vector3DMul(Vector3D v, float scalar)
{
    return Vector3D { v.x * scalar, v.y * scalar, v.z * scalar };
}

/**
 * @brief Divides a 3D vector by a scalar.
 * @param v The vector.
 * @param scalar The scalar value.
 * @return The resulting Vector3D.
 */
static inline Vector3D vector3DDiv(Vector3D v, float scalar)
{
    if (floatIsZero(scalar))
    {
        LOG_ERROR("Division by zero in vector3DDiv");
        return v;
    }
    return Vector3D{ v.x / scalar, v.y / scalar, v.z / scalar };
}

/**
 * @brief Calculates the dot product of two 3D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The dot product.
 */
static inline float vector3DDot(Vector3D v1, Vector3D v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/**
 * @brief Calculates the cross product of two 3D vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The resulting Vector3D.
 */
static inline Vector3D vector3DCross(Vector3D v1, Vector3D v2)
{
    return Vector3D
    {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

/**
 * @brief Calculates the magnitude of a 3D vector.
 * @param v The vector.
 * @return The magnitude.
 */
static inline float vector3DLength(Vector3D v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * @brief Normalizes a 3D vector.
 * @param v The vector to normalize.
 * @return The normalized Vector3D.
 */
static inline Vector3D vector3DNormalize(Vector3D v)
{
    float length = vector3DLength(v);
    if (floatIsZero(length))
    {
        LOG_WARNING("Attempt to normalize zero vector");
        return v;
    }
    return vector3DMul(v, 1.0f / length);
}

/* 3D Vector rotation operations */

/**
 * @brief Rotates a 3D vector around the X-axis.
 * @param v The vector to rotate.
 * @param angle The angle of rotation in radians.
 * @return The rotated Vector3D.
 */
static inline Vector3D vector3DRotateX(Vector3D v, float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return Vector3D
    {
        v.x,
        v.y * cosA - v.z * sinA,
        v.y * sinA + v.z * cosA
    };
}

/**
 * @brief Rotates a 3D vector around the Y-axis.
 * @param v The vector to rotate.
 * @param angle The angle of rotation in radians.
 * @return The rotated Vector3D.
 */
static inline Vector3D vector3DRotateY(Vector3D v, float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return Vector3D
    {
        v.x * cosA + v.z * sinA,
        v.y,
        -v.x * sinA + v.z * cosA
    };
}

/**
 * @brief Rotates a 3D vector around the Z-axis.
 * @param v The vector to rotate.
 * @param angle The angle of rotation in radians.
 * @return The rotated Vector3D.
 */
static inline Vector3D vector3DRotateZ(Vector3D v, float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return Vector3D
    {
        v.x * cosA - v.y * sinA,
        v.x * sinA + v.y * cosA,
        v.z
    };
}
#endif /* VECTOR_H */