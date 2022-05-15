#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

struct array;

struct array *array_new(size_t capacity, size_t element_size);
void array_copy(struct array *dst, struct array *src);

int array_append(struct array *array, void *data);
int array_set(struct array *array, size_t index, void *data);
int array_get(struct array *array, size_t index, void *data);
void *array_get_ref(struct array *array, size_t index);
void array_remove(struct array *array, size_t index);
int array_size(struct array *array);
int array_contains(struct array *array, void *data);
int array_pop(struct array *array, void *data);
void array_clear(struct array *array);
void array_destroy(struct array *array);

#endif // ARRAY_H
