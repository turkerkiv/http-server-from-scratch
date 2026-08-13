#include <arraylist.h>
#include <stdlib.h>

arraylist_t *new_arraylist(int capacity)
{
    arraylist_t *arraylist = malloc(sizeof(arraylist_t));
    if (arraylist == NULL)
        return NULL;

    arraylist->count = 0;
    arraylist->capacity = capacity;
    void **dt = malloc(capacity * sizeof(void *));
    if (dt == NULL)
    {
        free(arraylist);
        return NULL;
    }
    arraylist->data = dt;
    return arraylist;
}

void push(arraylist_t *arraylist, void *item)
{
    if (arraylist->capacity == arraylist->count)
    {
        arraylist->capacity *= 2;
        void **reallocated = realloc(arraylist, arraylist->capacity);
        if (reallocated == NULL)
        {
            arraylist->capacity /= 2;
            return;
        }
        arraylist->data = reallocated;
    }

    arraylist->data[arraylist->count] = item;
    arraylist->count++;
}

void *get_at_index(arraylist_t *arraylist, int index)
{
    if (index >= arraylist->count || index < 0)
        return NULL;

    return arraylist->data[index];
}

void remove_at_index(arraylist_t *arraylist, int index)
{
    if (index >= arraylist->count || index < 0)
        return NULL;

    for (int i = index; i < arraylist->count - 1; i++)
    {
        arraylist->data[i] = arraylist->data[i + 1];
    }

    arraylist->data[arraylist->count - 1] = NULL;
    arraylist->count--;
}