#ifndef LIST_H
#define LIST_H

/* Represents a node in a list */
typedef struct node
{
    void *item; // data contained in node
    struct node *next; // next item in the list
} NODE;

/* Represents a list */
typedef struct list
{
    NODE *head; // head of list
    NODE *tail; // tail of list
    NODE *cur;  // current node
    int size;   // size of list
} LIST;

/*
 * Creates an empty list.
 *
 * Input:
 * None
 *
 * Output:
 * A pointer to the created LIST. NULL pointer
 * if the operation fails.
 */ 
LIST *listCreate(void);

/*
 * Adds and item to the back of the list.
 *
 * Input:
 * Queue *: pointer to a LIST
 * void *: item to be added
 *
 * Output:
 * 0 on success, -1 on failure
 */
int append(LIST *, void *);

/*
 * Deletes the current node in the list. Makes the
 * current node the next node in the list.
 *
 * Input:
 * LIST *: pointer to a LIST
 *
 * Output:
 * 0 on success, -1 on failure
 */
int deletecur(LIST *);

/*
 * Returns the current item in the list.
 *
 * Input:
 * LIST *: pointer to a LIST
 *
 * Output:
 * A pointer to the current item, NULL pointer if there
 * is no current item.
 */
void *getcur(LIST *);

/*
 * Points current node to head of list
 *
 * Input:
 * LIST *: pointer to a LIST
 */
void gofirst(LIST *);

/*
 * Points current node to next item in list
 *
 * Input:
 * LIST *: pointer to a LIST
 */
void gonext(LIST *);

/*
 * Points current node to tail of list
 *
 * Input:
 * LIST *: pointer to a LIST
 */
void golast(LIST *);

/*
 * Returns the size of the list
 *
 * Input:
 * LIST *: pointer to a LIST
 */
int size(LIST *);

#endif // LIST_H
