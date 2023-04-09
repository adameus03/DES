#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#include <iostream>; //do usuniÍcia po debugowaniu

typedef unsigned char uchar;

//uchar** divide_to_chunks(uchar* data, size_t bitlen, int& n);

void initial_perm(uchar* blk, uchar* p);

void final_perm(uchar* blk, uchar* p);

void e_selection(uchar* blk, uchar* s);

void sbox_combined(uchar* u, uchar* v);

void p_permutation(uchar* d, uchar* p);


#endif // CRYPTO_H_INCLUDED
