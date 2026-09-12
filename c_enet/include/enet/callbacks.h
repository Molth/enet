/**
 @file  callbacks.h
 @brief ENet callbacks
*/
#ifndef __ENET_CALLBACKS_H__
#define __ENET_CALLBACKS_H__

#include <stdlib.h>

/**
  User-overridable memory allocation callbacks used by ENet.
  Initialize with enet_initialize_with_callbacks() to override the defaults.
*/
typedef struct _ENetCallbacks
{
    /** Function to allocate memory. */
    void *(ENET_CALLBACK *malloc)(size_t size);

    /** Function to free memory. */
    void(ENET_CALLBACK *free)(void *memory);

    /** Function called when an out-of-memory condition occurs. */
    void(ENET_CALLBACK *no_memory)(void);
} ENetCallbacks;

#ifdef __cplusplus
extern "C"
{
#endif

    /** @defgroup callbacks ENet internal callbacks
        @{
        @ingroup private
    */

    /** Allocates memory using the configured allocation callbacks.
        @param size The number of bytes to allocate.
        @returns a pointer to the allocated memory, or NULL on failure.
    */
    extern void *enet_malloc(size_t);

    /** Frees memory using the configured allocation callbacks.
        @param memory The memory to free.
    */
    extern void enet_free(void *);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* __ENET_CALLBACKS_H__ */
