#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stdlib.h>

uint64_t hash_sip(const void *data, size_t len, uint64_t seed0, uint64_t seed1);
uint64_t hash_murmur(const void *data, size_t len, uint64_t seed0,
                     uint64_t seed1);

#endif // HASH_H
