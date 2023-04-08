#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

typedef unsigned char uchar;

//uchar** divide_to_chunks(uchar* data, size_t bitlen, int& n);

void initial_perm(uchar* blk, uchar* p);

void final_perm(uchar* blk, uchar* p);

void e_selection(uchar* blk, uchar* s);


#endif // CRYPTO_H_INCLUDED
