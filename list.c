/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"

LIST *listCreate()
{
    // allocate space for a list struct
    LIST *list = malloc(sizeof(LIST));

    if ( list == NULL )
    {
        fprintf(stderr, "listCreate(): malloc error\n");
        return NULL;
    }

    // set default values
    list->head = NULL;
    list->tail = NULL;
    list->cur = NULL;
    list->size = 0;

    return list;
}

int append(LIST *l, void *item)
{
    NODE *node;

    // set up new node
    node = malloc(sizeof(NODE));

    if ( node == NULL )
    {
        fprintf(stderr, "append(): malloc error\n");
        return -1;
    }
    else
    {
        node->next = NULL;
        node->item = item;
    }

    // list is empty
    if ( l->size == 0 )
    {
        l->head = node;
        l->tail = node;
        l->cur = node;
        l->size = 1;
    }
    // list is non-empty
    else
    {
        l->tail->next = node;
        l->tail = node;
        l->cur = l->tail;
        l->size++;
    }

    return 0;
}

int deletecur(LIST *l)
{
    // list is empty
    if ( l->size == 0 )
    {
        fprintf(stderr, "delete_cur(): cannot remove item from empty list\n");
        return -1;
    }

    // list contains 1 item
    if ( l->size == 1 )
    {
        l->head = NULL;
        l->tail = NULL;
        l->cur = NULL;
        l->size = 0;
    }
    // list contains multiple items
    else
    {
        // deleting head
        if (l->head == l->cur)
        {
            l->cur = l->head->next;
            l->head = l->cur;
        }
        // deleting tail
        else if (l->tail == l->cur)
        {
            // remove tail, leave cur where is it
            NODE *prev = NULL;
            NODE *this = l->head;
            while (this != l->tail)
            {
                prev = this;
                this = this->next;
            }

            l->tail = prev;
            l->cur = NULL;
        }
        // delete in middle
        else
        {
            NODE *prev = NULL;
            NODE *this = l->head;
            while (this != l->cur)
            {
                prev = this;
                this = this->next;
            }
            prev->next = l->cur->next;
            l->cur->next = NULL;
            l->cur = prev->next;
        }
        l->size--;
    }

    return 0;
}

void *getcur(LIST *l)
{
    if (l->cur != NULL)
        return l->cur->item;
    else
        return NULL;
}

void gofirst(LIST *l)
{
    l->cur = l->head;
}

void gonext(LIST *l)
{
    if (l->cur->next != NULL)
        l->cur = l->cur->next;
    else
        l->cur = NULL;
}

void golast(LIST *l)
{
    l->cur = l->tail;
}

int size(LIST *l)
{
    return l->size;
}
