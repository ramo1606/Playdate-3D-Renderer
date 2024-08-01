#ifndef MEMORY_H
#define MEMORY_H

#include "pd_api.h"
#include "logging.h" 

/* Extern declaration for the PlaydateAPI singleton */
extern PlaydateAPI* pd;

/* Function Declarations */

/**
 * @brief Changes the size of the memory block pointed to by ptr.
 * @param ptr Pointer to the memory block to resize.
 * @param size The new size of the memory block.
 * @return A pointer to the reallocated memory, or NULL if reallocation fails.
 */
static inline void* pdRealloc(void* ptr, size_t size)
{
    void* new_ptr = pd->system->realloc(ptr, size);
    if (new_ptr == NULL && size != 0)
    {
        LOG_ERROR("pdRealloc: Memory reallocation failed for size %zu", size);
    }
    return new_ptr;
}

/**
 * @brief Allocates memory of the specified size.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
static inline void* pdMalloc(size_t size)
{
    void* ptr = pdRealloc(NULL, size);
    if (ptr == NULL)
	{
		LOG_ERROR("pdMalloc: Memory allocation failed for size %zu", size);
	}
    return ptr;
}

/**
 * @brief Allocates memory for an array of count elements of size bytes each and initializes all bytes in the allocated storage to zero.
 * @param count The number of elements to allocate.
 * @param size The size of each element.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
static inline void* pdCalloc(size_t count, size_t size)
{
    void* ptr = pdMalloc(count * size);
    if (ptr)
    {
        memset(ptr, 0, count * size);
    }
    else
    {
        LOG_ERROR("pdCalloc: Memory allocation failed for size %zu * %zu", num, size);
    }
    return ptr;
}

/**
 * @brief Frees the memory space pointed to by ptr.
 * @param ptr Pointer to the memory to free.
 */
static inline void pdFree(void* ptr)
{
    pd->system->realloc(ptr, 0);
}

#endif /* MEMORY_H */