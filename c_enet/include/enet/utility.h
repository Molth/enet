/**
 @file  utility.h
 @brief ENet utility header
*/
#ifndef __ENET_UTILITY_H__
#define __ENET_UTILITY_H__

#ifdef HAS_OFFSETOF
#include <stddef.h>
#endif

/** Returns the larger of x and y. */
#define ENET_MAX(x, y) ((x) > (y) ? (x) : (y))

/** Returns the smaller of x and y. */
#define ENET_MIN(x, y) ((x) < (y) ? (x) : (y))

/** Returns the absolute difference between x and y. */
#define ENET_DIFFERENCE(x, y) ((x) < (y) ? (y) - (x) : (x) - (y))

#ifdef HAS_OFFSETOF
/** Returns the byte offset of a field within a structure type. */
#define ENET_OFFSETOF(str, field) (offsetof(str, field))
#else
/** Returns the byte offset of a field within a structure type. */
#define ENET_OFFSETOF(str, field) ((size_t)&((str *)0)->field)
#endif

#endif /* __ENET_UTILITY_H__ */
