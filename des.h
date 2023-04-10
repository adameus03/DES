#ifndef DES_H_INCLUDED
#define DES_H_INCLUDED

#include <cstring>
#include "crypto.h"

void feistel_f(uchar* r, uchar* k, uchar* f);

void encrypt(uchar* blk, uchar* k, uchar* e);


#endif // DES_H_INCLUDED
