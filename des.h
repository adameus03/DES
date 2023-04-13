#ifndef DES_H_INCLUDED
#define DES_H_INCLUDED

#include <cstdlib>
#include <cstring>
#include "crypto.h"

void feistel_f(uchar* r, uchar* k, uchar* f);

void encrypt_blk(uchar* blk, uchar* sk, uchar* e);

void decrypt_blk(uchar* blk, uchar* sk, uchar* d);

void get_subkeys(uchar* k, uchar* sk);

void encrypt(uchar* in_buffer, uchar* out_buffer, uchar* key, const size_t& in_buffer_sz, size_t& out_blk_count);

void decrypt(uchar* in_buffer, uchar* out_buffer, uchar* key, const size_t& in_blk_count, size_t& out_buffer_sz);

void rand_key(uchar* k);

#endif // DES_H_INCLUDED
