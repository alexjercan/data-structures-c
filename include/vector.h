#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector;

struct vector *vector_new(size_t capacity, size_t element_size);
void vector_copy(struct vector *dst, struct vector *src);

int vector_set(struct vector *vector, size_t index, void *data);
int vector_get(struct vector *vector, size_t index, void *data);
void *vector_get_ref(struct vector *vector, size_t index);
size_t vector_capacity(struct vector *vector);
int vector_contains(struct vector *vector, void *data);

void vector_destroy(struct vector *vector);

#endif // VECTOR_H
