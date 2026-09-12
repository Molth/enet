/**
 @file  list.h
 @brief ENet list management
*/
#ifndef __ENET_LIST_H__
#define __ENET_LIST_H__

#include <stdlib.h>

/**
  A doubly-linked list node embedded in list elements.
*/
typedef struct _ENetListNode
{
    /** Pointer to the next node in the list. */
    struct _ENetListNode *next;

    /** Pointer to the previous node in the list. */
    struct _ENetListNode *previous;
} ENetListNode;

/** An iterator over an ENetList. */
typedef ENetListNode *ENetListIterator;

/**
  A doubly-linked list managed by a sentinel node.
*/
typedef struct _ENetList
{
    /** The sentinel node anchoring the list. */
    ENetListNode sentinel;
} ENetList;

#ifdef __cplusplus
extern "C"
{
#endif

    /** Initializes an empty list. */
    extern void enet_list_clear(ENetList *);

    /** Inserts a new element after the given position.
        @param position The position after which to insert.
        @param data The element to insert.
        @returns an iterator to the inserted element.
    */
    extern ENetListIterator enet_list_insert(ENetListIterator, void *);

    /** Removes the element at the given position.
        @param position The position of the element to remove.
        @returns the removed element.
    */
    extern void *enet_list_remove(ENetListIterator);

    /** Moves an element to follow the given position.
        @param position The position after which the element is moved.
        @param dataFirst The first element to move.
        @param dataLast The last element to move.
        @returns an iterator to the moved element.
    */
    extern ENetListIterator enet_list_move(ENetListIterator, void *, void *);

    /** Returns the number of elements in the list.
        @param list The list to measure.
        @returns the number of elements.
    */
    extern size_t enet_list_size(ENetList *);

#ifdef __cplusplus
}
#endif

/** Returns an iterator to the first element of the list. */
#define enet_list_begin(list) ((list)->sentinel.next)
/** Returns an iterator to the end of the list. */
#define enet_list_end(list) (&(list)->sentinel)

/** Determines whether the list is empty. */
#define enet_list_empty(list) (enet_list_begin(list) == enet_list_end(list))

/** Advances an iterator to the next element. */
#define enet_list_next(iterator) ((iterator)->next)
/** Moves an iterator to the previous element. */
#define enet_list_previous(iterator) ((iterator)->previous)

/** Returns the first element of the list. */
#define enet_list_front(list) ((void *)(list)->sentinel.next)
/** Returns the last element of the list. */
#define enet_list_back(list) ((void *)(list)->sentinel.previous)

#endif /* __ENET_LIST_H__ */
