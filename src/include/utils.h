
#ifndef _H_UTILS
#define _H_UTILS

#include <stdlib.h>
#include <assert.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

// Initial capacity of a dynamic array
#define BM_DA_INIT_CAP 256

// Append an item to a dynamic array
#define bm_da_append(da, item)                                                          \
    do {                                                                                 \
        if ((da)->count >= (da)->capacity) {                                             \
            (da)->capacity = (da)->capacity == 0 ? BM_DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                                \
                                                                                         \
        (da)->items[(da)->count++] = (item);                                             \
    } while (0)

#define bm_da_free(da) free((da).items)

// Append several items to a dynamic array
#define bm_da_join(da, new_items, new_items_count)                                  \
    do {                                                                                    \
        if ((da)->count + (new_items_count) > (da)->capacity) {                               \
            if ((da)->capacity == 0) {                                                      \
                (da)->capacity = bm_DA_INIT_CAP;                                           \
            }                                                                               \
            while ((da)->count + (new_items_count) > (da)->capacity) {                        \
                (da)->capacity *= 2;                                                        \
            }                                                                               \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                          \
        }                                                                                   \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                                                     \
    } while (0)

#endif