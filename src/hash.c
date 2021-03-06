#include "../include/hash.h"

//-----------------------------------------------------------------------------
// SipHash reference C implementation
//
// Copyright (c) 2012-2016 Jean-Philippe Aumasson
// <jeanphilippe.aumasson@gmail.com>
// Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//
// default: SipHash-2-4
//-----------------------------------------------------------------------------
static uint64_t SIP64(const uint8_t *in, const size_t inlen, uint64_t seed0,
                      uint64_t seed1) {
#define U8TO64_LE(p)                                                           \
  {                                                                            \
    (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) |                        \
     ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) |                 \
     ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) |                 \
     ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56))                  \
  }
#define U64TO8_LE(p, v)                                                        \
  {                                                                            \
    U32TO8_LE((p), (uint32_t)((v)));                                           \
    U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));                                 \
  }
#define U32TO8_LE(p, v)                                                        \
  {                                                                            \
    (p)[0] = (uint8_t)((v));                                                   \
    (p)[1] = (uint8_t)((v) >> 8);                                              \
    (p)[2] = (uint8_t)((v) >> 16);                                             \
    (p)[3] = (uint8_t)((v) >> 24);                                             \
  }
#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND                                                               \
  {                                                                            \
    v0 += v1;                                                                  \
    v1 = ROTL(v1, 13);                                                         \
    v1 ^= v0;                                                                  \
    v0 = ROTL(v0, 32);                                                         \
    v2 += v3;                                                                  \
    v3 = ROTL(v3, 16);                                                         \
    v3 ^= v2;                                                                  \
    v0 += v3;                                                                  \
    v3 = ROTL(v3, 21);                                                         \
    v3 ^= v0;                                                                  \
    v2 += v1;                                                                  \
    v1 = ROTL(v1, 17);                                                         \
    v1 ^= v2;                                                                  \
    v2 = ROTL(v2, 32);                                                         \
  }
  uint64_t k0 = U8TO64_LE((uint8_t *)&seed0);
  uint64_t k1 = U8TO64_LE((uint8_t *)&seed1);
  uint64_t v3 = UINT64_C(0x7465646279746573) ^ k1;
  uint64_t v2 = UINT64_C(0x6c7967656e657261) ^ k0;
  uint64_t v1 = UINT64_C(0x646f72616e646f6d) ^ k1;
  uint64_t v0 = UINT64_C(0x736f6d6570736575) ^ k0;
  const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));
  for (; in != end; in += 8) {
    uint64_t m = U8TO64_LE(in);
    v3 ^= m;
    SIPROUND;
    SIPROUND;
    v0 ^= m;
  }
  const int left = inlen & 7;
  uint64_t b = ((uint64_t)inlen) << 56;
  switch (left) {
  case 7:
    b |= ((uint64_t)in[6]) << 48;
  case 6:
    b |= ((uint64_t)in[5]) << 40;
  case 5:
    b |= ((uint64_t)in[4]) << 32;
  case 4:
    b |= ((uint64_t)in[3]) << 24;
  case 3:
    b |= ((uint64_t)in[2]) << 16;
  case 2:
    b |= ((uint64_t)in[1]) << 8;
  case 1:
    b |= ((uint64_t)in[0]);
    break;
  case 0:
    break;
  }
  v3 ^= b;
  SIPROUND;
  SIPROUND;
  v0 ^= b;
  v2 ^= 0xff;
  SIPROUND;
  SIPROUND;
  SIPROUND;
  SIPROUND;
  b = v0 ^ v1 ^ v2 ^ v3;
  uint64_t out = 0;
  U64TO8_LE((uint8_t *)&out, b);
  return out;
}

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
//
// Murmur3_86_128
//-----------------------------------------------------------------------------
static void MM86128(const void *key, const int len, uint32_t seed, void *out) {
#define ROTL32(x, r) ((x << r) | (x >> (32 - r)))
#define FMIX32(h)                                                              \
  h ^= h >> 16;                                                                \
  h *= 0x85ebca6b;                                                             \
  h ^= h >> 13;                                                                \
  h *= 0xc2b2ae35;                                                             \
  h ^= h >> 16;
  const uint8_t *data = (const uint8_t *)key;
  const int nblocks = len / 16;
  uint32_t h1 = seed;
  uint32_t h2 = seed;
  uint32_t h3 = seed;
  uint32_t h4 = seed;
  uint32_t c1 = 0x239b961b;
  uint32_t c2 = 0xab0e9789;
  uint32_t c3 = 0x38b34ae5;
  uint32_t c4 = 0xa1e38b93;
  const uint32_t *blocks = (const uint32_t *)(data + nblocks * 16);
  for (int i = -nblocks; i; i++) {
    uint32_t k1 = blocks[i * 4 + 0];
    uint32_t k2 = blocks[i * 4 + 1];
    uint32_t k3 = blocks[i * 4 + 2];
    uint32_t k4 = blocks[i * 4 + 3];
    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
    h1 = ROTL32(h1, 19);
    h1 += h2;
    h1 = h1 * 5 + 0x561ccd1b;
    k2 *= c2;
    k2 = ROTL32(k2, 16);
    k2 *= c3;
    h2 ^= k2;
    h2 = ROTL32(h2, 17);
    h2 += h3;
    h2 = h2 * 5 + 0x0bcaa747;
    k3 *= c3;
    k3 = ROTL32(k3, 17);
    k3 *= c4;
    h3 ^= k3;
    h3 = ROTL32(h3, 15);
    h3 += h4;
    h3 = h3 * 5 + 0x96cd1c35;
    k4 *= c4;
    k4 = ROTL32(k4, 18);
    k4 *= c1;
    h4 ^= k4;
    h4 = ROTL32(h4, 13);
    h4 += h1;
    h4 = h4 * 5 + 0x32ac3b17;
  }
  const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);
  uint32_t k1 = 0;
  uint32_t k2 = 0;
  uint32_t k3 = 0;
  uint32_t k4 = 0;
  switch (len & 15) {
  case 15:
    k4 ^= tail[14] << 16;
  case 14:
    k4 ^= tail[13] << 8;
  case 13:
    k4 ^= tail[12] << 0;
    k4 *= c4;
    k4 = ROTL32(k4, 18);
    k4 *= c1;
    h4 ^= k4;
  case 12:
    k3 ^= tail[11] << 24;
  case 11:
    k3 ^= tail[10] << 16;
  case 10:
    k3 ^= tail[9] << 8;
  case 9:
    k3 ^= tail[8] << 0;
    k3 *= c3;
    k3 = ROTL32(k3, 17);
    k3 *= c4;
    h3 ^= k3;
  case 8:
    k2 ^= tail[7] << 24;
  case 7:
    k2 ^= tail[6] << 16;
  case 6:
    k2 ^= tail[5] << 8;
  case 5:
    k2 ^= tail[4] << 0;
    k2 *= c2;
    k2 = ROTL32(k2, 16);
    k2 *= c3;
    h2 ^= k2;
  case 4:
    k1 ^= tail[3] << 24;
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0] << 0;
    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  };
  h1 ^= len;
  h2 ^= len;
  h3 ^= len;
  h4 ^= len;
  h1 += h2;
  h1 += h3;
  h1 += h4;
  h2 += h1;
  h3 += h1;
  h4 += h1;
  FMIX32(h1);
  FMIX32(h2);
  FMIX32(h3);
  FMIX32(h4);
  h1 += h2;
  h1 += h3;
  h1 += h4;
  h2 += h1;
  h3 += h1;
  h4 += h1;
  ((uint32_t *)out)[0] = h1;
  ((uint32_t *)out)[1] = h2;
  ((uint32_t *)out)[2] = h3;
  ((uint32_t *)out)[3] = h4;
}

// hash_sip returns a hash value for `data` using SipHash-2-4.
uint64_t hash_sip(const void *data, size_t len, uint64_t seed0,
                  uint64_t seed1) {
  return SIP64((uint8_t *)data, len, seed0, seed1);
}

// hash_murmur returns a hash value for `data` using Murmur3_86_128.
uint64_t hash_murmur(const void *data, size_t len, uint64_t seed0,
                     uint64_t seed1) {
  char out[16];
  MM86128(data, len, seed0, &out);
  return *(uint64_t *)out;
}
