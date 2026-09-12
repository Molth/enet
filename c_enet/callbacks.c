/**
 @file callbacks.c
 @brief ENet callback functions
*/
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"

static ENetCallbacks callbacks = {malloc, free, abort};

/** Initializes ENet globally and supplies user-overridden callbacks.
    @param version the constant ENET_VERSION should be supplied so ENet knows which version of ENetCallbacks struct to use
    @param inits user-overridden callbacks where any NULL callbacks will use ENet's defaults
    @returns 0 on success, < 0 on failure
*/
int enet_initialize_with_callbacks(ENetVersion version, const ENetCallbacks *inits)
{
  if (version < ENET_VERSION_CREATE(1, 3, 0))
    return -1;

  if (inits->malloc != NULL || inits->free != NULL)
  {
    if (inits->malloc == NULL || inits->free == NULL)
      return -1;

    callbacks.malloc = inits->malloc;
    callbacks.free = inits->free;
  }

  if (inits->no_memory != NULL)
    callbacks.no_memory = inits->no_memory;

  return enet_initialize();
}

/** Gives the linked version of the ENet library.
    @returns the version number
*/
ENetVersion
enet_linked_version(void)
{
  return ENET_VERSION;
}

/** Allocates memory using the configured allocation callbacks.
    @param size The number of bytes to allocate.
    @returns a pointer to the allocated memory, or NULL on failure.
*/
void *
enet_malloc(size_t size)
{
  void *memory = callbacks.malloc(size);

  if (memory == NULL)
    callbacks.no_memory();

  return memory;
}

/** Frees memory using the configured allocation callbacks.
    @param memory The memory to free.
*/
void enet_free(void *memory)
{
  callbacks.free(memory);
}
