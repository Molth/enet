/**
 @file  time.h
 @brief ENet time constants and macros
*/
#ifndef __ENET_TIME_H__
#define __ENET_TIME_H__

/** The number of milliseconds after which a time value is considered wrapped around. */
#define ENET_TIME_OVERFLOW 86400000

/** Compares two time values, returning true if a is before b, accounting for wraparound. */
#define ENET_TIME_LESS(a, b) ((a) - (b) >= ENET_TIME_OVERFLOW)

/** Compares two time values, returning true if a is after b, accounting for wraparound. */
#define ENET_TIME_GREATER(a, b) ((b) - (a) >= ENET_TIME_OVERFLOW)

/** Compares two time values, returning true if a is before or equal to b, accounting for wraparound. */
#define ENET_TIME_LESS_EQUAL(a, b) (!ENET_TIME_GREATER(a, b))

/** Compares two time values, returning true if a is after or equal to b, accounting for wraparound. */
#define ENET_TIME_GREATER_EQUAL(a, b) (!ENET_TIME_LESS(a, b))

/** Computes the absolute difference between two time values, accounting for wraparound. */
#define ENET_TIME_DIFFERENCE(a, b) ((a) - (b) >= ENET_TIME_OVERFLOW ? (b) - (a) : (a) - (b))

#endif /* __ENET_TIME_H__ */
