#ifndef DES_H_INCLUDED
#define DES_H_INCLUDED

#include <cstring>
#include "crypto.h"

void feistel_f(uchar* r, uchar* k, uchar* f);

void encrypt_blk(uchar* blk, uchar* sk, uchar* e);

void decrypt_blk(uchar* blk, uchar* sk, uchar* d);

void get_subkeys(uchar* k, uchar* sk);

#endif // DES_H_INCLUDED
