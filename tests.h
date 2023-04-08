#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <iostream>
#include "crypto.h"

typedef unsigned char uchar;

//uchar test_divide_to_chunks(uchar* data, size_t bitlen, int& n);

void checker(const uchar& ok, const char* description);

uchar test_initial_perm();

uchar test_final_perm();




#endif // TESTS_H_INCLUDED
