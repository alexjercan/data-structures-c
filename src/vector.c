#include "../include/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
  unsigned char *data;
  size_t capacity;
  size_t element_size;
};

struct vector *vector_new(size_t capacity, size_t element_size) {
  struct vector *vector = malloc(sizeof(struct vector));

  vector->data = calloc(capacity, element_size);

  vector->capacity = capacity;
  vector->element_size = element_size;

  return vector;
}

void vector_copy(struct vector *dst, struct vector *src) {
  if (dst->capacity < src->capacity) {
    dst->data = realloc(dst->data, src->capacity);
  }

  memcpy(dst->data, src->data, src->capacity * src->element_size);

  dst->capacity = src->capacity;
  dst->element_size = src->element_size;
}

int vector_set(struct vector *vector, size_t index, void *data) {
  if (index < 0) {
    return -1;
  }
  if (index >= vector->capacity) {
    size_t capacity = index + 1;
    vector->data = realloc(vector->data, capacity * vector->element_size);
    memset(vector->data + vector->capacity * vector->element_size, 0,
           (capacity - vector->capacity) * vector->element_size);
    vector->capacity = capacity;
  }

  memcpy(vector->data + index * vector->element_size, data,
         vector->element_size);

  return 0;
}

int vector_get(struct vector *vector, size_t index, void *data) {
  if (index >= vector->capacity) {
    memset(data, 0, vector->element_size);
    return 0;
  }

  memcpy(data, vector->data + index * vector->element_size,
         vector->element_size);

  return 0;
}

void *vector_get_ref(struct vector *vector, size_t index) {
  if (index >= vector->capacity) {
    size_t capacity = index + 1;
    vector->data = realloc(vector->data, capacity * vector->element_size);
    memset(vector->data + vector->capacity * vector->element_size, 0,
           (capacity - vector->capacity) * vector->element_size);
    vector->capacity = capacity;
  }

  return vector->data + index * vector->element_size;
}

size_t vector_capacity(struct vector *vector) { return vector->capacity; }

int vector_contains(struct vector *vector, void *data) {
  for (size_t i = 0; i < vector->capacity; i++) {
    if (memcmp(vector->data + i * vector->element_size, data,
               vector->element_size) == 0) {
      return 1;
    }
  }

  return 0;
}

void vector_destroy(struct vector *vector) {
  free(vector->data);
  free(vector);
}
