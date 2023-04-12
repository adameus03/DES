typedef unsigned char uchar;

#include <iostream>
#include "crypto.h"
#include "des.h"

/*uchar test_divide_to_chunks(uchar* data, size_t bitlen, int& n){

}*/
/*!
    @brief
        Funkcja do wyswietlania wynikow testu
    @param ok
        Wynik testu: 0x1 - pozytywny / 0x0 - negatywny
    @param description
        Nazwa testowanej funkcjonalnosci
*/
void checker(const uchar& ok, const char* description){
    std::cout << '[' << (ok?"OK":"!!") << "]  " << description << std::endl;
}

/*!
    @brief
        Funkcja do wyswietlania w jednej linijce bitow we wszystkich bajtach bufora
    @param data
        Wskaznik do bufora danych
    @param bitlen
        Rozmiar danych w buforze (w bitach)
*/
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

/*!
    @brief
        Funkcja do ladnego wyswietlania (w formie tablicy 2D) bitow w poszczegolnych bajtach
    @param d
        Wskaznik do bufora danych
    @param n_bytes
        Liczba bajtow w buforze danych
*/
void print_blk(uchar* d, uchar n_bytes){
    for(uchar i=0x0; i<n_bytes; i++){
        printb(d+i, 8);
        std::cout << std::endl;
    }
}
//void initial_perm(uchar* blk, uchar* p)


uchar test_initial_perm(){
    /* Test 1 */

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

    uchar retval = 0x1;
    if(!(*p  == 0b11011110 &&
      *(p+1) == 0b01111011 &&
      *(p+2) == 0b01111101 &&
      *(p+3) == 0b11011111 &&
      *(p+4) == 0b01111101 &&
      *(p+5) == 0b11100111 &&
      *(p+6) == 0b10111110 &&
      *(p+7) == 0b11111111)) retval = 0x0;
    delete[] p;

    /* Test 2 */
    d = new uchar[8];
    *d     = 0b00111100;
    *(d+1) = 0b11110000;
    *(d+2) = 0b00110011;
    *(d+3) = 0b10101000;
    *(d+4) = 0b00111010;
    *(d+5) = 0b11111100;
    *(d+6) = 0b10010110;
    *(d+7) = 0b10001111;

    p = new uchar[8];
    initial_perm(d, p);
    delete[] d;

    if(!(*p  == 0b00100010 &&
      *(p+1) == 0b01110111 &&
      *(p+2) == 0b11100001 &&
      *(p+3) == 0b10000100 &&
      *(p+4) == 0b11101010 &&
      *(p+5) == 0b00111111 &&
      *(p+6) == 0b10111001 &&
      *(p+7) == 0b11010100)) retval = 0x0;
    delete[] p;

    return retval;
}
//void test_final_perm(uchar* blk, uchar* p)
uchar test_final_perm(){
    /* Test 1 */
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

    uchar retval = 0x1;
    if(!(*p  == 0b10110111 &&
      *(p+1) == 0b01111011 &&
      *(p+2) == 0b11101111 &&
      *(p+3) == 0b11011111 &&
      *(p+4) == 0b11011111 &&
      *(p+5) == 0b10111110 &&
      *(p+6) == 0b11110111 &&
      *(p+7) == 0b01101011)) retval = 0x0;
    delete[] p;

    /* Test 2 */

    d = new uchar[8];

    *d     = 0b00100010;
    *(d+1) = 0b01110111;
    *(d+2) = 0b11100001;
    *(d+3) = 0b10000100;
    *(d+4) = 0b11101010;
    *(d+5) = 0b00111111;
    *(d+6) = 0b10111001;
    *(d+7) = 0b11010100;

    p = new uchar[8];
    final_perm(d, p);
    delete[] d;

    if(!(*p  == 0b00111100 &&
      *(p+1) == 0b11110000 &&
      *(p+2) == 0b00110011 &&
      *(p+3) == 0b10101000 &&
      *(p+4) == 0b00111010 &&
      *(p+5) == 0b11111100 &&
      *(p+6) == 0b10010110 &&
      *(p+7) == 0b10001111)) retval = 0x0;

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
    /* Test 1 */
    uchar* d = new uchar[4];
    *d     =  0b11011110;
    *(d+1) =  0b01111011;
    *(d+2) =  0b01111101;
    *(d+3) =  0b11011111;

    uchar* s = new uchar[6];
    e_selection(d, s);
    delete[] d;
    uchar retval = 0x1;
    if(!(             *s  == 0b11101111 &&
                   *(s+1) == 0b11000011 &&
                   *(s+2) == 0b11110110 &&
                   *(s+3) == 0b10111111 &&
                   *(s+4) == 0b10111110 &&
                   *(s+5) == 0b11111111 )) retval = 0x0;
    delete[] s;

    /* Test 2 */

    /*
        32  1  2  3  4  5  4  5                 1  2  3  4  5  6  7  8
         6  7  8  9  8  9 10 11                 9 10 11 12 13 14 15 16
        12 13 12 13 14 15 16 17                17 18 19 20 21 22 23 24
        16 17 18 19 20 21 20 21                25 26 27 28 29 30 31 32
        22 23 24 25 24 25 26 27
        28 29 28 29 30 31 32 1
    */

    d = new uchar[4]; // aka input R to the Feistel function
    *d     = 0b00011111;
    *(d+1) = 0b11011100;
    *(d+2) = 0b01010001;
    *(d+3) = 0b11100001;

    s = new uchar[6];
    e_selection(d, s);
    delete[] d;

    if(!(             *s  == 0b10001111 &&
                   *(s+1) == 0b11111110 &&
                   *(s+2) == 0b11111000 &&
                   *(s+3) == 0b00101010 &&
                   *(s+4) == 0b00111111 &&
                   *(s+5) == 0b00000010 )) retval = 0x0;


    delete[] s;

    return retval;
}

//void sbox_combined(uchar* u, uchar* v)
uchar test_sbox_combined(){
    /* Test 1 */
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

    uchar retval = 0x1;
    if(!(*v  == 0b11100000 &&
      *(v+1) == 0b10001101 &&
      *(v+2) == 0b11100111 &&
      *(v+3) == 0b11111011)) retval = 0x0;

    delete[] v;

    /* Test 2 */
    u = new uchar[6]; // aka result of before s-box XOR operation in Feistel function


    *u     = 0b01111110;     // 01 1111 S1 => 1000
    *(u+1) = 0b11100010;     // 10 1110 S2 => 0010
    *(u+2) = 0b10000001;     // 00 1010 S3 => 1100
    *(u+3) = 0b00101011;     // 00 0001 S4 => 1101
    *(u+4) = 0b11000111;     // 00 1010 S5 => 0011
    *(u+5) = 0b01010000;     // 11 1100 S6 => 0110
                             // 01 1101 S7 => 1111
                             // 01 0000 S8 => 0001

    v = new uchar[4];
    sbox_combined(u, v);
    delete[] u;

    if(!(*v  == 0b10000010 &&
      *(v+1) == 0b11001101 &&
      *(v+2) == 0b00110110 &&
      *(v+3) == 0b11110001)) retval = 0x0;
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
    /* Test 1 */
    uchar* d = new uchar[4];
    *d     =  0b11011110;
    *(d+1) =  0b01111011;
    *(d+2) =  0b01111101;
    *(d+3) =  0b11011111;
    uchar* p = new uchar[4];
    p_permutation(d, p);
    delete[] d;
    //print_blk(p, 4);
    uchar retval = 0x1;

    if(!(*p  == 0b11111110 &&
      *(p+1) == 0b11011111 &&
      *(p+2) == 0b10101000 &&
      *(p+3) == 0b11111111)) retval = 0x0;
    delete[] p;

    /* Test 2 */

    /*
        16  7 20 21 29 12 28 17           1  2  3  4  5  6  7  8
         1 15 23 26  5 18 31 10           9 10 11 12 13 14 15 16
         2  8 24 14 32 27  3  9          17 18 19 20 21 22 23 24
        19 13 30  6 22 11  4 25          25 26 27 28 29 30 31 32
    */

    d = new uchar[4]; // aka output of sbox_combined
    *d     = 0b10000010;
    *(d+1) = 0b11001101;
    *(d+2) = 0b00110110;
    *(d+3) = 0b11110001;

    p = new uchar[4]; // aka final output of Feistel function
    p_permutation(d, p);
    delete[] d;
    if(!(*p  == 0b11100010 &&
      *(p+1) == 0b10110001 &&
      *(p+2) == 0b00011101 &&
      *(p+3) == 0b11001001)) retval = 0x0;
    delete[] p;


    return retval;
}

//void permuted_choice_1(uchar* k, uchar* cd)
/*
        [C]
    57 49 41 33 25 17  9  1                   01 02 03 04 05 06 07 08
    58 50 42 34 26 18 10  2                   09 10 11 12 13 14 15 16
    59 51 43 35 27 19 11  3                   17 18 19 20 21 22 23 24
    60 52 44 36                               25 26 27 28 29 30 31 32
                                              33 34 35 36 37 38 39 40
                                              41 42 43 44 45 46 47 48
                                              49 50 51 52 53 54 55 56
                                              57 58 59 60 61 62 63 64
    [D]
                63 55 47 39
    31 23 15  7 62 54 46 38
    30 22 14  6 61 53 45 37
    29 21 13  5 28 20 12  4
*/
uchar test_permuted_choice_1(){
    uchar* k = new uchar[8];
    *k     =  0b11011110;
    *(k+1) =  0b01111011;
    *(k+2) =  0b01111101;
    *(k+3) =  0b11011111;
    *(k+4) =  0b01111101;
    *(k+5) =  0b11100111;
    *(k+6) =  0b10111110;
    *(k+7) =  0b11111111;
    uchar* cd = new uchar[7];
    permuted_choice_1(k, cd);
    delete[] k;
    uchar retval =    *cd  == 0b11101001 &&
                   *(cd+1) == 0b10111111 &&
                   *(cd+2) == 0b11110110 &&
                   *(cd+3) == 0b11011110 &&
                   *(cd+4) == 0b10111111 &&
                   *(cd+5) == 0b11011101 &&
                   *(cd+6) == 0b11111111 ;
    delete[] cd;
    return retval;
}

//void permuted_choice_2(uchar* cd, uchar* k)
/*
    14 17 11 24 1 5
    3 28 15 6 21 10
    23 19 12 4 26 8
    16 7 27 20 13 2
    41 52 31 37 47 55
    30 40 51 45 33 48
    44 49 39 56 34 53
    46 42 50 36 29 32


    14 17 11 24  1  5  3 28                       01 02 03 04 05 06 07 08
    15  6 21 10 23 19 12  4                       09 10 11 12 13 14 15 16
    26  8 16  7 27 20 13  2                       17 18 19 20 21 22 23 24
    41 52 31 37 47 55 30 40                       25 26 27 28 29 30 31 32
    51 45 33 48 44 49 39 56                       33 34 35 36 37 38 39 40
    34 53 46 42 50 36 29 32                       41 42 43 44 45 46 47 48
                                                  49 50 51 52 53 54 55 56
*/
uchar test_permuted_choice_2(){
    uchar* cd = new uchar[7];
    *cd     =  0b11011110;
    *(cd+1) =  0b01111011;
    *(cd+2) =  0b01111101;
    *(cd+3) =  0b11011111;
    *(cd+4) =  0b01111101;
    *(cd+5) =  0b11100111;
    *(cd+6) =  0b10111110;
    uchar* k = new uchar[6];
    permuted_choice_2(cd, k);
    delete[] cd;
    uchar retval =    *k  == 0b00111101 &&
                   *(k+1) == 0b11110111 &&
                   *(k+2) == 0b10110111 &&
                   *(k+3) == 0b11111111 &&
                   *(k+4) == 0b10010100 &&
                   *(k+5) == 0b11110111 ;
    delete[] k;
    return retval;
}

//void xor_blks(uchar* a, uchar* b, uchar* r, const uchar& blk_len)
uchar test_xor_blks(){
    /* Test 1 */
    uchar* a = new uchar[4];
    uchar* b = new uchar[4];

    *a     =  0b11011110;
    *(a+1) =  0b01111011;
    *(a+2) =  0b01111101;
    *(a+3) =  0b11011111;

    *b     =  0b01111101;
    *(b+1) =  0b11100111;
    *(b+2) =  0b10111110;
    *(b+3) =  0b11111111;

    uchar* r = new uchar[4];
    xor_blks(a, b, r, 0x4);
    delete[] a;
    delete[] b;
    uchar retval = 0x1;

    if(!(*r  == 0b10100011 &&
      *(r+1) == 0b10011100 &&
      *(r+2) == 0b11000011 &&
      *(r+3) == 0b00100000)) retval = 0x0;
    delete[] r;

    /* test 2 */
    a = new uchar[6]; //aka result of E on R
    b = new uchar[6]; //aka K

    *a     = 0b10001111;
    *(a+1) = 0b11111110;
    *(a+2) = 0b11111000;
    *(a+3) = 0b00101010;
    *(a+4) = 0b00111111;
    *(a+5) = 0b00000010;

    *b     = 0b11110001;
    *(b+1) = 0b00011100;
    *(b+2) = 0b01111001;
    *(b+3) = 0b00000001;
    *(b+4) = 0b11111000;
    *(b+5) = 0b01010010;

    r = new uchar[6];
    xor_blks(a, b, r, 0x6);
    delete[] a;
    delete[] b;

    if(!(*r  == 0b01111110 &&
      *(r+1) == 0b11100010 &&
      *(r+2) == 0b10000001 &&
      *(r+3) == 0b00101011 &&
      *(r+4) == 0b11000111 &&
      *(r+5) == 0b01010000)) retval = 0x0;

    delete[] r;

    return retval;
}


//void lshift(uchar* cd, uchar* r)
uchar test_lshift_blk7(){
    uchar* cd = new uchar[7];
    *cd     =  0b11011110;
    *(cd+1) =  0b01111011;
    *(cd+2) =  0b01111101;
    *(cd+3) =  0b11011111;
    *(cd+4) =  0b01111101;
    *(cd+5) =  0b11100111;
    *(cd+6) =  0b10111110;
    lshift_blk7(cd, cd);
    //print_blk(cd, 7);
    uchar retval =    *cd  == 0b10111100 &&
                   *(cd+1) == 0b11110110 &&
                   *(cd+2) == 0b11111011 &&
                   *(cd+3) == 0b10111110 &&
                   *(cd+4) == 0b11111011 &&
                   *(cd+5) == 0b11001111 &&
                   *(cd+6) == 0b01111101 ;
    delete[] cd;
    return retval;
}

//void feistel_f(uchar* r, uchar* k, uchar* f)
void visual_test_feistel_f(){
    uchar* r = new uchar[4];
    *r     =  0b11011110;
    *(r+1) =  0b01111011;
    *(r+2) =  0b01111101;
    *(r+3) =  0b11011111;
    uchar* k = new uchar[6];
    *k     =  0b11011111;
    *(k+1) =  0b01111010;
    *(k+2) =  0b01111100;
    *(k+3) =  0b11011111;
    *(k+4) =  0b01111100;
    *(k+5) =  0b11011111;
    feistel_f(r, k, r);
    delete[] k;
    printb(r, 32);
    std::cout << std::endl;
    delete[] r;
}

uchar test_feistel_f(){
    uchar* r = new uchar[4];
    uchar* k = new uchar[6];

    *r     = 0b00011111;
    *(r+1) = 0b11011100;
    *(r+2) = 0b01010001;
    *(r+3) = 0b11100001;

    *k     = 0b11110001;
    *(k+1) = 0b00011100;
    *(k+2) = 0b01111001;
    *(k+3) = 0b00000001;
    *(k+4) = 0b11111000;
    *(k+5) = 0b01010010;

    uchar* f = new uchar[4];

    feistel_f(r, k, f);
    delete[] r;
    delete[] k;

    uchar retval = 0x1;
    if(!(*f == 0b11100010 &&
     *(f+1) == 0b10110001 &&
     *(f+2) == 0b00011101 &&
     *(f+3) == 0b11001001)) retval = 0x0;

    delete[] f;

    return retval;
}

//void encrypt(uchar* blk, uchar* k, uchar* e)
void visual_test_encrypt(){
    uchar* blk = new uchar[8];

    *blk     =  0b11011110;
    *(blk+1) =  0b01111011;
    *(blk+2) =  0b01111101;
    *(blk+3) =  0b11011111;
    *(blk+4) =  0b01111101;
    *(blk+5) =  0b11100111;
    *(blk+6) =  0b10111110;
    *(blk+7) =  0b00001111;

    /*
    *blk     = 0b01100000;
    *(blk+1) = 0b00111000;
    *(blk+2) = 0b11101111;
    *(blk+3) = 0b11111101;
    *(blk+4) = 0b10100010;
    *(blk+5) = 0b01111100;
    *(blk+6) = 0b01011111;
    *(blk+7) = 0b01101110;
    */

    uchar* k = new uchar[8];
    *k     =  0b11011111;
    *(k+1) =  0b01111010;
    *(k+2) =  0b01111100;
    *(k+3) =  0b11011111;
    *(k+4) =  0b01111100;
    *(k+5) =  0b11100110;
    *(k+6) =  0b10111111;
    *(k+7) =  0b00001110;
    uchar* e = new uchar[8];
    std::cout << "--BLK-----------" << std::endl;
    print_blk(blk, 0x8);
    std::cout << "--K-------------" << std::endl;
    print_blk(k, 0x8);
    encrypt(blk, k, e);
    delete[] blk;
    delete[] k;
    std::cout << "--E-------------" << std::endl;
    print_blk(e, 0x8);
    delete[] e;
}
