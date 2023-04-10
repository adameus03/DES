#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#include <iostream> //do usuniêcia po debugowaniu

typedef unsigned char uchar;

//uchar** divide_to_chunks(uchar* data, size_t bitlen, int& n);

void initial_perm(uchar* blk, uchar* p); // [tested]

void final_perm(uchar* blk, uchar* p); // [tested]

void e_selection(uchar* blk, uchar* s); // [tested]

void sbox_combined(uchar* u, uchar* v); // [tested]

void p_permutation(uchar* d, uchar* p); // [tested]

void permuted_choice_1(uchar* k, uchar* cd); // [tested]

void permuted_choice_2(uchar* cd, uchar* k); // [tested]

void xor_blks(uchar* a, uchar* b, uchar* r, const uchar& blk_length);

void feistel_f(uchar* r, uchar* k, uchar* f);

#endif // CRYPTO_H_INCLUDED
