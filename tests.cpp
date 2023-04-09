typedef unsigned char uchar;

#include <iostream>
#include "crypto.h"

/*uchar test_divide_to_chunks(uchar* data, size_t bitlen, int& n){

}*/

void checker(const uchar& ok, const char* description){
    std::cout << '[' << (ok?"OK":"!!") << "]  " << description << std::endl;
}

void printb(uchar* data, size_t bitlen){
    size_t len = bitlen / 8;
    uchar blm = (uchar)(bitlen%8); //bitlen mod

    for(size_t i=0; i<len; i++){
        uchar* c = data+i; //byte data chunk ptr
        for(uchar b=0; b<8u; b++){
            uchar m = 0x80u >> b; //mask
            uchar t = *c & m; //temp
            if(t >> (7u-b)) std::cout << "1 ";
            else std::cout << "0 ";
        }
    }
    if(blm != 0u){
        uchar* c = data+len;
        for(uchar b=0; b<blm; b++){
            uchar m = 0x80u >> b;
            uchar t = *c & m;
            if(t >> (7u-b)) std::cout << "1 ";
            else std::cout << "0 ";
        }
    }
}
void print_blk(uchar* d, uchar n_bytes){
    for(uchar i=0x0; i<n_bytes; i++){
        printb(d+i, 8);
        std::cout << std::endl;
    }
}
//void initial_perm(uchar* blk, uchar* p)
uchar test_initial_perm(){
    uchar* d = new uchar[8];
    *d     =  0b10110111;  //11011110
    *(d+1) =  0b01111011;  //01111011
    *(d+2) =  0b11101111;  //01111101
    *(d+3) =  0b11011111;  //11011111
    *(d+4) =  0b11011111;  //01111101
    *(d+5) =  0b10111110;  //11100111
    *(d+6) =  0b11110111;  //10111110
    *(d+7) =  0b01101011;  //11111111

    uchar * p = new uchar[8];
    initial_perm(d, p);

    delete[] d;

    uchar retval =    *p  == 0b11011110 &&
                   *(p+1) == 0b01111011 &&
                   *(p+2) == 0b01111101 &&
                   *(p+3) == 0b11011111 &&
                   *(p+4) == 0b01111101 &&
                   *(p+5) == 0b11100111 &&
                   *(p+6) == 0b10111110 &&
                   *(p+7) == 0b11111111 ;
    delete[] p;
    return retval;
}
//void test_final_perm(uchar* blk, uchar* p)
uchar test_final_perm(){
    uchar* d = new uchar[8];
    *d     =  0b11011110;
    *(d+1) =  0b01111011;
    *(d+2) =  0b01111101;
    *(d+3) =  0b11011111;
    *(d+4) =  0b01111101;
    *(d+5) =  0b11100111;
    *(d+6) =  0b10111110;
    *(d+7) =  0b11111111;

    uchar * p = new uchar[8];
    final_perm(d, p);
    delete[] d;

    uchar retval =    *p  == 0b10110111 &&
                   *(p+1) == 0b01111011 &&
                   *(p+2) == 0b11101111 &&
                   *(p+3) == 0b11011111 &&
                   *(p+4) == 0b11011111 &&
                   *(p+5) == 0b10111110 &&
                   *(p+6) == 0b11110111 &&
                   *(p+7) == 0b01101011 ;
    delete[] p;
    return retval;
}
/*
    32  1  2  3  4  5  4  5                 1  2  3  4  5  6  7  8
     6  7  8  9  8  9 10 11                 9 10 11 12 13 14 15 16
    12 13 12 13 14 15 16 17                17 18 19 20 21 22 23 24
    16 17 18 19 20 21 20 21                25 26 27 28 29 30 31 32
    22 23 24 25 24 25 26 27
    28 29 28 29 30 31 32 1
*/
//oid e_selection(uchar* blk, uchar* s)
uchar test_e_selection(){
    uchar* d = new uchar[4];
    *d     =  0b11011110;
    *(d+1) =  0b01111011;
    *(d+2) =  0b01111101;
    *(d+3) =  0b11011111;

    uchar* s = new uchar[6];
    e_selection(d, s);
    delete[] d;
    uchar retval =    *s  == 0b11101111 &&
                   *(s+1) == 0b11000011 &&
                   *(s+2) == 0b11110110 &&
                   *(s+3) == 0b10111111 &&
                   *(s+4) == 0b10111110 &&
                   *(s+5) == 0b11111111 ;
    delete[] s;
    return retval;
}

//void sbox_combined(uchar* u, uchar* v)
uchar test_sbox_combined(){
    uchar* u = new uchar[6];
       *u  = 0b11101111;  // 111011
    *(u+1) = 0b11000011;  // 111100
    *(u+2) = 0b11110110;  // 001111
    *(u+3) = 0b10111111;  // 110110
    *(u+4) = 0b10111110;  // 101111
    *(u+5) = 0b11111111;  // 111011
                          // 111011
                          // 111111

    uchar* v = new uchar[4];

    sbox_combined(u, v);
    delete[] u;

    uchar retval =    *v  == 0b11100000 &&
                   *(v+1) == 0b10001101 &&
                   *(v+2) == 0b11100111 &&
                   *(v+3) == 0b11111011 ;

    delete[] v;
    return retval;

}

//void p_permutation(uchar* d, uchar* p)

/*
    16  7 20 21 29 12 28 17           1  2  3  4  5  6  7  8
     1 15 23 26  5 18 31 10           9 10 11 12 13 14 15 16
     2  8 24 14 32 27  3  9          17 18 19 20 21 22 23 24
    19 13 30  6 22 11  4 25          25 26 27 28 29 30 31 32
*/

uchar test_p_permutation(){
    uchar* d = new uchar[4];
    *d     =  0b11011110;
    *(d+1) =  0b01111011;
    *(d+2) =  0b01111101;
    *(d+3) =  0b11011111;
    uchar* p = new uchar[4];
    p_permutation(d, p);
    delete[] d;
    print_blk(p, 4);
    uchar retval =    *p  == 0b11111110 &&
                   *(p+1) == 0b11011111 &&
                   *(p+2) == 0b10101000 &&
                   *(p+3) == 0b11111111 ;
    delete[] p;
    return retval;
}
