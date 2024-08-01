#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <float.h>
#include "logging.h"

/* Float comparison epsilon */
#define FLOAT_EPSILON FLT_EPSILON

/**
 * @brief Checks if two floats are approximately equal.
 * @param a The first float.
 * @param b The second float.
 * @return 1 if approximately equal, 0 otherwise.
 */
static inline int floatEquals(float a, float b)
{
    return fabsf(a - b) <= FLOAT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(a), fabsf(b)));
}

/**
 * @brief Checks if a float is approximately zero.
 * @param a The float to check.
 * @return 1 if approximately zero, 0 otherwise.
 */
static inline int floatIsZero(float a)
{
    return fabsf(a) <= FLOAT_EPSILON;
}

/**
 * @brief Checks if a float is approximately greater than another.
 * @param a The first float.
 * @param b The second float.
 * @return 1 if a > b (approximately), 0 otherwise.
 */
static inline int floatGreaterThan(float a, float b)
{
    return (a - b) > FLOAT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(a), fabsf(b)));
}

/**
 * @brief Checks if a float is approximately less than another.
 * @param a The first float.
 * @param b The second float.
 * @return 1 if a < b (approximately), 0 otherwise.
 */
static inline int floatLessThan(float a, float b)
{
    return (b - a) > FLOAT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(a), fabsf(b)));
}

/**
 * @brief Clamps a float value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 * @return The clamped value.
 */
static inline float floatClamp(float value, float min, float max)
{
    return fmaxf(min, fminf(value, max));
}

/**
 * @brief Linearly interpolates between two float values.
 * @param a The start value.
 * @param b The end value.
 * @param t The interpolation factor (0.0 to 1.0).
 * @return The interpolated value.
 */
static inline float floatLerp(float a, float b, float t)
{
    return a + (b - a) * floatClamp(t, 0.0f, 1.0f);
}

#endif /* UTILS_H */