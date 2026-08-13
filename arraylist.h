typedef struct ArrayList
{
    void **data;
    int count;
    int capacity;
} arraylist_t;

arraylist_t *new_arraylist(int capacity);
void push(arraylist_t *array, void *data);
void *get_at_index(arraylist_t *array, int index);
void remove_at_index(arraylist_t *array, int index);