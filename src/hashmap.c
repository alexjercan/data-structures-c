#include "../include/hashmap.h"
#include "../include/array.h"
#include "../include/vector.h"

#include <stdlib.h>

#define BUCKET_SIZE 16

struct hashmap {
  struct vector *buckets;
  size_t capacity;
  size_t element_size;

  size_t (*hash)(const void *);
  int (*equals)(const void *, const void *);
};

struct hashmap *hashmap_new(size_t capacity, size_t element_size,
                            size_t (*hash)(const void *),
                            int (*equals)(const void *, const void *)) {
  struct hashmap *map = malloc(sizeof(struct hashmap));

  map->buckets = vector_new(capacity, sizeof(struct array *));
  for (size_t i = 0; i < capacity; i++) {
    struct array *bucket = array_new(BUCKET_SIZE, element_size);
    vector_set(map->buckets, i, &bucket);
  }

  map->capacity = capacity;
  map->element_size = element_size;

  map->hash = hash;
  map->equals = equals;

  return map;
}

void hashmap_copy(struct hashmap *dest, struct hashmap *src) {
  for (size_t i = 0; i < src->capacity; i++) {
    struct array *bucket_src =
        *(struct array **)vector_get_ref(src->buckets, i);
    struct array *bucket_dest =
        *(struct array **)vector_get_ref(dest->buckets, i);

    array_copy(bucket_dest, bucket_src);
  }

  dest->capacity = src->capacity;
  dest->element_size = src->element_size;

  dest->hash = src->hash;
  dest->equals = src->equals;
}

int hashmap_set(struct hashmap *map, void *kv) {
  size_t hash = map->hash(kv) % map->capacity;

  struct array *bucket = *(struct array **)vector_get_ref(map->buckets, hash);

  for (size_t i = 0; i < array_size(bucket); i++) {
    void *element = array_get_ref(bucket, i);

    if (map->equals(kv, element) == 0) {
      return array_set(bucket, i, kv);
    }
  }

  return array_append(bucket, kv);
}

int hashmap_get(struct hashmap *map, void *kv) {
  size_t hash = map->hash(kv) % map->capacity;

  struct array *bucket = *(struct array **)vector_get_ref(map->buckets, hash);

  for (size_t i = 0; i < array_size(bucket); i++) {
    void *element = array_get_ref(bucket, i);

    if (map->equals(kv, element) == 0) {
      return array_get(bucket, i, kv);
    }
  }

  return -1;
}

void *hashmap_get_ref(struct hashmap *map, void *kv) {
  size_t hash = map->hash(kv) % map->capacity;

  struct array *bucket = *(struct array **)vector_get_ref(map->buckets, hash);

  for (size_t i = 0; i < array_size(bucket); i++) {
    void *element = array_get_ref(bucket, i);

    if (map->equals(kv, element) == 0) {
      return element;
    }
  }

  return NULL;
}

void hashmap_remove(struct hashmap *map, void *kv) {
  size_t hash = map->hash(kv) % map->capacity;

  struct array *bucket = *(struct array **)vector_get_ref(map->buckets, hash);

  for (size_t i = 0; i < array_size(bucket); i++) {
    void *element = array_get_ref(bucket, i);

    if (map->equals(kv, element) == 0) {
      return array_remove(bucket, i);
    }
  }
}

void hashmap_destroy(struct hashmap *map) {
  for (size_t i = 0; i < map->capacity; i++) {
    struct array *bucket = *(struct array **)vector_get_ref(map->buckets, i);
    array_destroy(bucket);
  }

  vector_destroy(map->buckets);
  free(map);
}
