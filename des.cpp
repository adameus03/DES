#include <cstring>
#include "crypto.h"

/*!
    @brief
        Funkcja Feistela struktury Feistela - ozn. f(R, K)
    @param r
        Wskaznik do  32-bitowego argumentu R funkcji f(R, K)    [patrz dokumentacja DES]
    @param k
        Wskaznik do 48-bitowego argumentu K funkcji f(R, K)     [patrz dokumentacja DES]
    @param f
        Wskaznik do miejsca w pamieci, gdzie bedzie zapisany wynik, czyli f(R, K)    [patrz dokumentacja DES]
*/
void feistel_f(uchar* r, uchar* k, uchar* f){
    uchar* s = new uchar[6];
    e_selection(r, s);
    xor_blks(s, k, s, 0x6);
    sbox_combined(s, s);
    p_permutation(s, f);
    delete[] s; // move outside the func for optimization?
}

/*!

    @param blk
        Wskaznik do  64-bitowego bloku wejsciowego
    @param k
        Wskaznik do 64-bitowego klucza glownego dla algorytmu DES
    @param e
        Wskaznik do miejsca w pamieci, gdzie bedzie zapisany 64-bitowy wynik szyfrowania bloku wejsciowego
*/
void encrypt(uchar* blk, uchar* k, uchar* e){
    uchar* lr = new uchar[8];
    initial_perm(blk, lr);
    uchar* l = lr;
    uchar* r = lr+4;
    uchar* cd = new uchar[7];
    permuted_choice_1(k, cd);

    uchar* _k = new uchar[6];
    uchar* f = new uchar[4];
    uchar* x = new uchar[4];

    for(uchar i=0x0; i<0x2; i++){
        lshift_blk7(cd, cd);
        permuted_choice_2(cd, _k);
        feistel_f(r, _k, f);
        xor_blks(l, f, x, 0x4);
        memcpy(l, r, 4);
        memcpy(r, x, 4);
    }
    for(uchar i=0x0; i<0x6; i++){
        lshift_blk7(cd, cd); lshift_blk7(cd, cd);
        permuted_choice_2(cd, _k);
        feistel_f(r, _k, f);
        xor_blks(l, f, x, 0x4);
        memcpy(l, r, 4);
        memcpy(r, x, 4);
    }
    lshift_blk7(cd, cd);
    permuted_choice_2(cd, _k);
    feistel_f(r, _k, f);
    xor_blks(l, f, x, 0x4);
    memcpy(l, r, 4);
    memcpy(r, x, 4);
    for(uchar i=0x0; i<0x6; i++){
        lshift_blk7(cd, cd); lshift_blk7(cd, cd);
        permuted_choice_2(cd, _k);
        feistel_f(r, _k, f);
        xor_blks(l, f, x, 0x4);
        memcpy(l, r, 4);
        memcpy(r, x, 4);
    }
    lshift_blk7(cd, cd);
    permuted_choice_2(cd, _k);
    feistel_f(r, _k, f);
    xor_blks(l, f, x, 0x4);
    memcpy(l, x, 4);

    final_perm(lr, e);
}


