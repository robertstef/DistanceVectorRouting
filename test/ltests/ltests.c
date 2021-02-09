#include <stdio.h>
#include <stdlib.h>
#include <list.h>

int main()
{
    LIST *list;
    int *vals, *num;
    int rv;

    // initialize values to put in list
    vals = malloc(sizeof(int) * 20);

    for(int i = 1; i < 21; i++)
        vals[i - 1] = i;

    // Tests listCreate()
    printf("Testing listCreate()\n");
    list = listCreate();

    if ( list->head != NULL || list->tail != NULL || 
            list->cur != NULL || list->size != 0 )
        printf("listCreate() error - default values incorrect\n");

    // Test append()
    printf("Testing append()\n");
    
    rv = append(list, &vals[0]);
    if ( rv < 0 )
        printf("append(): return value is -1\n");
    if ( *(int *)(list->head->item) != 1 || *(int *)(list->tail->item) != 1 
           || *(int *)(list->cur->item) != 1 || list->size != 1 )
        printf("append() error: adding to empty list\n");

    rv = append(list, &vals[1]);
    if ( rv < 0 )
        printf("append(): return value is -1\n");
    if ( *(int *)(list->head->item) != 1 || *(int *)(list->tail->item) != 2 
            || *(int *)(list->cur->item) != 2 || list->size != 2 )
        printf("append() error: adding to non-empty list\n");
    
    rv = append(list, &vals[2]);
    if ( rv < 0 )
        printf("append(): return value is -1\n");
    if ( *(int *)(list->head->item) != 1 || *(int *)(list->tail->item) != 3
            || *(int *)(list->cur->item) != 3 || list->size != 3 )
        printf("append() error: adding to non-empty list\n");

    rv = append(list, &vals[3]);
    if ( rv < 0 )
        printf("append(): return value is -1\n");
    if ( *(int *)(list->head->item) != 1 || *(int *)(list->tail->item) != 4
            || *(int *)(list->cur->item) != 4 || list->size != 4 )
        printf("append() error: adding to non-empty list\n");
    
    rv = append(list, &vals[4]);
    if ( rv < 0 )
        printf("append(): return value is -1\n");
    if ( *(int *)(list->head->item) != 1 || *(int *)(list->tail->item) != 5
            || *(int *)(list->cur->item) != 5 || list->size != 5 )
        printf("append() error: adding to non-empty list\n");


    // Test getcur
    printf("Testing getcur()\n");
    num = (int *)getcur(list);
    if (*(num) != 5)
        printf("getcur() error: return values should have been 5\n");


    // Test gofirst
    printf("Testing gofirst()\n");
    gofirst(list);
    num = (int *)getcur(list);
    if (*(num) != 1)
        printf("getcur() error: return values should have been 1\n");


    // Test golast
    printf("Testing golast()\n");
    golast(list);
    num = (int *)getcur(list);
    if (*(num) != 5)
        printf("getcur() error: return values should have been 5\n");


    // Test gonext
    printf("Testing gonext()\n");
    gofirst(list);
    for(int i = 1; i < 4; i++)
    {
        num = (int *)getcur(list);
        if (*(num) != i)
            printf("gonext() error: current value should have been %d\n", i);
        gonext(list);
    }


    // Test deletecur()
    printf("Testing deletecur()\n");

    // delete from head
    gofirst(list);
    rv = deletecur(list);
    if (rv == -1)
        printf("deletecur() error: return value should have been 0\n");
    num = (int *)getcur(list);
    if ( *(num) != 2 )
        printf("deletecur() error: current values should be 2\n");

    // delete from tail
    golast(list);
    rv = deletecur(list);
    if (rv == -1)
        printf("deletecur() error: return value should have been 0\n");
    num = (int *)getcur(list);
    if ( num != NULL )
        printf("deletecur() error: current values should be NULL\n");
    
    // delete from middle
    if ((rv = append(list, &vals[4])) == -1)
        printf("append() error when adding 5\n");
    if ((rv = append(list, &vals[5])) == -1)
        printf("append() error when adding 6\n");

    gofirst(list);
    gonext(list);
    gonext(list);

    rv = deletecur(list);
    if (rv == -1)
        printf("deletecur() error: return value should have been 0\n");
    num = (int *)getcur(list);
    if ( *num != 5 )
        printf("deletecur() error: current values should be 5\n");
     
    rv = deletecur(list);
    if (rv == -1)
        printf("deletecur() error: return value should have been 0\n");
    num = (int *)getcur(list);
    if ( *num != 6 )
        printf("deletecur() error: current values should be 6\n");
    /*
    // Tests delist()/peek()
    printf("Testing delist()/peek()\n");

    num = (int *)peek(list);
    if ( *(num) != 1 )
        printf("peek() error: value return should have been 1\n");
    
    rv = delist(list);
    if ( rv == -1 )
        printf("delist() error: return value is -1\n");
    if ( *(int *)(list->head->item) != 2 || *(int *)(list->tail->item) != 4
            || list->size != 3 )
        printf("delist() error: removing 1 from non-empty list\n");

    rv = delist(list);
    if ( rv == -1 )
        printf("delist() error: return value is -1\n");
    if ( *(int *)(list->head->item) != 3 || *(int *)(list->tail->item) != 4
            || list->size != 2 )
        printf("delist() error: removing 2 from non-empty list\n");

    num = (int *)peek(list);
    if ( *(num) != 3 )
        printf("peek() error: value return should have been 3\n");

    rv = delist(list);
    if ( rv == -1 )
        printf("delist() error: return value is -1\n");
    if ( *(int *)(list->head->item) != 4 || *(int *)(list->tail->item) != 4
            || list->size != 1 )
        printf("delist() error: removing 4 from non-empty list\n");

    rv = delist(list);
    if ( rv == -1 )
        printf("delist() error: return value is -1\n");
    if ( (list->head) != NULL || (list->tail) != NULL || 
            list->size != 0 )
        printf("delist() error: removing from LAST TIME non-empty list\n");

    num = (int *)peek(list);
    if ( num != NULL )
        printf("peek() error: return value should be NULL\n");

    rv = delist(list);
    if ( rv == 0 )
        printf("delist() error: dequeuing from empty list, rv" 
                " should be 1\n");
    */
    // clean up
    free(list);
    free(vals);
    list = NULL;
    vals = NULL;

    printf("***TEST SCRIPT COMPLETE***\n");
    return 0;
}
