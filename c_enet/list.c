/**
 @file list.c
 @brief ENet linked list functions
*/
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"

/**
    @defgroup list ENet linked list utility functions
    @ingroup private
    @{
*/

/** Initializes an empty list.
    @param list The list to clear.
*/
void enet_list_clear(ENetList *list)
{
    list->sentinel.next = &list->sentinel;
    list->sentinel.previous = &list->sentinel;
}

/** Inserts a new element after the given position.
    @param position The position after which to insert.
    @param data The element to insert.
    @returns an iterator to the inserted element.
*/
ENetListIterator
enet_list_insert(ENetListIterator position, void *data)
{
    ENetListIterator result = (ENetListIterator)data;

    result->previous = position->previous;
    result->next = position;

    result->previous->next = result;
    position->previous = result;

    return result;
}

/** Removes the element at the given position.
    @param position The position of the element to remove.
    @returns the removed element.
*/
void *
enet_list_remove(ENetListIterator position)
{
    position->previous->next = position->next;
    position->next->previous = position->previous;

    return position;
}

/** Moves an element to follow the given position.
    @param position The position after which the element is moved.
    @param dataFirst The first element to move.
    @param dataLast The last element to move.
    @returns an iterator to the moved element.
*/
ENetListIterator
enet_list_move(ENetListIterator position, void *dataFirst, void *dataLast)
{
    ENetListIterator first = (ENetListIterator)dataFirst,
                     last = (ENetListIterator)dataLast;

    first->previous->next = last->next;
    last->next->previous = first->previous;

    first->previous = position->previous;
    last->next = position;

    first->previous->next = first;
    position->previous = last;

    return first;
}

/** Returns the number of elements in the list.
    @param list The list to measure.
    @returns the number of elements.
*/
size_t
enet_list_size(ENetList *list)
{
    size_t size = 0;
    ENetListIterator position;

    for (position = enet_list_begin(list);
         position != enet_list_end(list);
         position = enet_list_next(position))
        ++size;

    return size;
}

/** @} */
