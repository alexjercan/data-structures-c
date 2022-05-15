#include "include/vector.h"
#include "include/array.h"
#include "include/queue.h"
#include "include/hashmap.h"
#include "include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR(msg)  do {                                                                    \
                        fprintf(stderr, "Error in %s:%d: %s\n", __FILE__, __LINE__, msg);   \
                        exit(1);                                                            \
                    } while (0)

#define ASSERT(cond, msg)   do {                            \
                                if (!(cond)) ERROR(msg);    \
                            } while (0)

int test_vector() {
    int capacity = 10;

    struct vector *v = vector_new(capacity, sizeof(int));
    ASSERT(v != NULL, "vector_new failed");
    ASSERT(vector_capacity(v) == capacity, "vector_capacity failed");

    for (int i = 0; i < capacity; i++) {
        vector_set(v, i, &i);
    }

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)vector_get_ref(v, i);
        ASSERT(value == i, "vector_get_ref failed");
    }

    struct vector *clone = vector_new(capacity, sizeof(int));
    vector_copy(clone, v);

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)vector_get_ref(clone, i);
        ASSERT(value == i, "vector_copy failed");
    }

    for (int i = 0; i < capacity; i++) {
        vector_set(clone, i, &capacity);
    }

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)vector_get_ref(v, i);
        ASSERT(value == i, "vector_set failed");
    }

    vector_destroy(clone);
    vector_destroy(v);

    return 0;
}

int test_array() {
    int capacity = 10;

    struct array *a = array_new(capacity, sizeof(int));
    ASSERT(a != NULL, "array_new failed");
    ASSERT(array_size(a) == 0, "array_size failed");

    for (int i = 0; i < capacity; i++) {
        array_append(a, &i);
    }

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)array_get_ref(a, i);
        ASSERT(value == i, "array_get_ref failed");
    }

    struct array *clone = array_new(capacity, sizeof(int));

    array_copy(clone, a);

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)array_get_ref(clone, i);
        ASSERT(value == i, "array_copy failed");
    }

    for (int i = 0; i < capacity; i++) {
        array_set(clone, i, &capacity);
    }

    for (int i = 0; i < capacity; i++) {
        int value = *(int *)array_get_ref(a, i);
        ASSERT(value == i, "array_set failed");
    }

    array_clear(clone);
    ASSERT(array_size(clone) == 0, "array_clear failed");

    array_destroy(clone);
    array_destroy(a);

    return 0;
}

int test_queue() {
    struct queue *q = queue_new(sizeof(int));
    ASSERT(q != NULL, "queue_new failed");
    ASSERT(queue_empty(q) != 0, "queue_empty failed");

    for (int i = 0; i < 10; i++) {
        queue_enqueue(q, &i);
    }

    ASSERT(queue_empty(q) == 0, "queue_empty failed");

    int data = 0;
    queue_peek(q, &data);
    ASSERT(data == 0, "queue_peer failed");

    for (int i = 0; i < 10; i++) {
        int value = 0;
        queue_dequeue(q, &value);
        ASSERT(value == i, "queue_dequeue failed");
    }

    ASSERT(queue_empty(q) != 0, "queue_empty failed");

    queue_destroy(q);

    return 0;
}

struct key_value {
    char *key;
    int value;
};

size_t hash(const void *kv) {
    struct key_value *kv_ptr = (struct key_value *)kv;

    return hash_sip((void *)kv_ptr->key, strlen(kv_ptr->key), 0, 0);
}

int equals(const void *kv1, const void *kv2) {
    struct key_value *kv1_ptr = (struct key_value *)kv1;
    struct key_value *kv2_ptr = (struct key_value *)kv2;

    return strcmp(kv1_ptr->key, kv2_ptr->key);
}

int test_hashmap() {
    int capacity = 10;
    struct hashmap *h = hashmap_new(capacity, sizeof(struct key_value), hash, equals);
    ASSERT(h != NULL, "hashmap_new failed");

    hashmap_set(h, &(struct key_value){"key1", 1});
    hashmap_set(h, &(struct key_value){"key2", 2});
    hashmap_set(h, &(struct key_value){"key3", 3});

    struct key_value k1 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key1"});
    ASSERT(k1.value == 1, "hashmap_get_ref failed");

    struct key_value k2 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key2"});
    ASSERT(k2.value == 2, "hashmap_get_ref failed");

    struct key_value k3 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key3"});
    ASSERT(k3.value == 3, "hashmap_get_ref failed");

    hashmap_set(h, &(struct key_value){"key1", 4});
    ASSERT(k1.value == 1, "hashmap_set failed");

    k1 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key1"});
    ASSERT(k1.value == 4, "hashmap_get_ref failed");

    hashmap_remove(h, &(struct key_value){"key1"});
    ASSERT(hashmap_get_ref(h, &(struct key_value){"key1"}) == NULL, "hashmap_remove failed");

    k2 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key2"});
    ASSERT(k2.value == 2, "hashmap_get_ref failed");

    k3 = *(struct key_value*)hashmap_get_ref(h, &(struct key_value){"key3"});
    ASSERT(k3.value == 3, "hashmap_get_ref failed");

    hashmap_destroy(h);

    return 0;
}

int main() {
    test_vector();
    test_array();
    test_queue();
    test_hashmap();

    printf("Passed\n");
    return 0;
}
