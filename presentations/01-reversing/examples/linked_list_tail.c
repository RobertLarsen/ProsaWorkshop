#include <stddef.h>
typedef struct _linked_list_t linked_list_t;
struct _linked_list_t {
    linked_list_t * next;
    void * data;
};

linked_list_t * linked_list_tail(linked_list_t * elem) {
    return elem == NULL || elem->next == NULL ? 
           elem : 
           linked_list_tail(elem->next);
}
