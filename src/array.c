#include "../include/array.h"

#include <stdlib.h>
#include <string.h>

struct array {
  unsigned char *data;
  size_t size;
  size_t capacity;
  size_t element_size;
};

struct array *array_new(size_t capacity, size_t element_size) {
  struct array *array = malloc(sizeof(struct array));

  array->data = calloc(capacity, element_size);

  array->size = 0;
  array->capacity = capacity;
  array->element_size = element_size;

  return array;
}

void array_copy(struct array *dst, struct array *src) {
  memcpy(dst->data, src->data, src->size * src->element_size);

  dst->size = src->size;
  dst->capacity = src->capacity;
  dst->element_size = src->element_size;
}

int array_append(struct array *array, void *data) {
  if (array->size >= array->capacity) {
    array->capacity *= 2;
    array->data = realloc(array->data, array->capacity * array->element_size);
  }

  memcpy(array->data + array->size * array->element_size, data,
         array->element_size);

  array->size++;

  return 0;
}

int array_set(struct array *array, size_t index, void *data) {
  if (index >= array->size) {
    return -1;
  }

  memcpy(array->data + index * array->element_size, data, array->element_size);

  return 0;
}

int array_get(struct array *array, size_t index, void *data) {
  if (index >= array->size) {
    return -1;
  }

  memcpy(data, array->data + index * array->element_size, array->element_size);

  return 0;
}

void *array_get_ref(struct array *array, size_t index) {
  if (index >= array->size) {
    return NULL;
  }

  return array->data + index * array->element_size;
}

void array_remove(struct array *array, size_t index) {
  if (index >= array->size) {
    return;
  }

  memmove(array->data + index * array->element_size,
          array->data + (index + 1) * array->element_size,
          (array->size - index - 1) * array->element_size);

  array->size--;
}

int array_size(struct array *array) { return array->size; }

int array_contains(struct array *array, void *data) {
  for (int i = 0; i < array->size; i++) {
    if (memcmp(array->data + i * array->element_size, data,
               array->element_size) == 0) {
      return 1;
    }
  }

  return 0;
}

int array_pop(struct array *array, void *data) {
  if (array->size == 0) {
    return -1;
  }

  array->size--;
  if (data != NULL) {
    memcpy(data, array->data + array->size * array->element_size,
           array->element_size);
  }

  return 0;
}

void array_clear(struct array *array) { array->size = 0; }

void array_destroy(struct array *array) {
  free(array->data);
  free(array);
}
