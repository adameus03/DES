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
void print_blk(uchar* d){
    for(uchar i=0x0; i<0x8; i++){
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
    print_blk(p);

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
