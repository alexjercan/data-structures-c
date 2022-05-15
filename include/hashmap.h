#ifndef HASHMAP_C
#define HASHMAP_C

#include <stdlib.h>

struct hashmap;

struct hashmap *hashmap_new(size_t capacity, size_t element_size,
                            size_t (*hash)(const void *),
                            int (*equals)(const void *, const void *));
void hashmap_copy(struct hashmap *dest, struct hashmap *src);

int hashmap_set(struct hashmap *map, void *kv);
int hashmap_get(struct hashmap *map, void *kv);
void *hashmap_get_ref(struct hashmap *map, void *kv);
void hashmap_remove(struct hashmap *map, void *kv);

void hashmap_destroy(struct hashmap *map);

#endif // HASHMAP_C
