typedef struct ArrayList
{
    void **data;
    int count;
    int capacity;
} arraylist_t;

arraylist_t *new_arraylist(int capacity);
void push(arraylist_t *arraylist, void *data);
void *get_at_index(arraylist_t *arraylist, int index);
void remove_at_index(arraylist_t *arraylist, int index);
void free_arraylist(arraylist_t *arraylist);