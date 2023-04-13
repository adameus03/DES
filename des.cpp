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
        Wskaznik do miejsca w pamieci, gdzie bedzie zapisany 32-bitowy wynik, czyli f(R, K)    [patrz dokumentacja DES]
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
    @brief
        Funkcja szyfrujaca blok 64-bitowy algorytmem DES
    @param blk
        Wskaznik do  64-bitowego bloku wejsciowego
    @param sk
        Wskaznik do bufora podkluczy dla algorytmu DES
        mieszczacego sie w 96 bajtach. Kazdy podklucz ma rozmiar 6B.
    @param e
        Wskaznik do miejsca w pamieci, gdzie bedzie zapisany 64-bitowy wynik szyfrowania bloku wejsciowego
*/
void encrypt_blk(uchar* blk, uchar* sk, uchar* e){
    uchar* lr = new uchar[8];
    initial_perm(blk, lr);
    uchar* l = lr; // L = L0
    uchar* r = lr+4; // R = R0
    uchar* f = new uchar[4];
    uchar* x = new uchar[4];
    uchar* sk_head = sk;

    for(uchar i=0x0; i<0xf; i++){
        feistel_f(r, sk_head, f);
        xor_blks(l, f, x, 0x4);
        memcpy(l, r, 4);
        memcpy(r, x, 4);
        sk_head += 6;
    }

    feistel_f(r, sk_head, f);
    xor_blks(l, f, x, 0x4);
    memcpy(l, x, 4);
    final_perm(lr, e);
    delete[] lr;
    delete[] f;
    delete[] x;
}

/*!
    @brief
        Funkcja deszyfrujaca blok 64-bitowy algorytmem DES
    @param blk
        Wskaznik do 64-bitowego bloku wejsciowego (zaszyfrowanego)
    @param k
        Wskaznik do bufora podkluczy dla algorytmu DES
        mieszczacego sie w 96 bajtach. Kazdy podklucz ma rozmiar 6B.
    @param d
        Wskaznik do miejsca w pamieci, gdzie bedzie zapisany 64-bitowy wynik deszyfrowania bloku wejsciowego
*/
void decrypt_blk(uchar* blk, uchar* sk, uchar* d){
    uchar* lr = new uchar[8];
    initial_perm(blk, lr);
    uchar* l = lr;
    uchar* r = lr+4;
    uchar* f = new uchar[4];
    uchar* x = new uchar[4];
    uchar* sk_head = sk + 90;

    for(uchar i=0x0; i<0xf; i++){
        feistel_f(r, sk_head, f);
        xor_blks(l, f, x, 0x4);
        memcpy(l, r, 4);
        memcpy(r, x, 4);
        sk_head -= 6;
    }

    feistel_f(r, sk_head, f);
    xor_blks(l, f, x, 0x4);
    memcpy(l, x, 4);
    final_perm(lr, d);
    delete[] lr;
    delete[] f;
    delete[] x;
}

/*!
    @brief
        Funkcja obliczajaca podklucze na podstawie klucza glownego DES
    @param k
        Wskaznik na bufor zawierajacy 64-bitowy klucz glowny dla algorytmu DES
    @param sk
        Wskaznik do wyjsciowego bufora podkluczy dla algorytmu DES
        mieszczacego sie w 96 bajtach. Kazdy podklucz ma rozmiar 6B.
        Wymagane zaalokowanie obszaru pamieci dla bufora przez wywolaniem funkcji.
*/
void get_subkeys(uchar* k, uchar* sk){
    uchar* cd = new uchar[7];
    permuted_choice_1(k, cd);

    uchar* sk_head = sk;

    for(uchar i=0x0; i<0x2; i++){
        lshift_blk7(cd, cd);
        permuted_choice_2(cd, sk_head);
        sk_head += 6;
    }
    for(uchar i=0x0; i<0x6; i++){
        lshift_blk7(cd, cd); lshift_blk7(cd, cd);
        permuted_choice_2(cd, sk_head);
        sk_head += 6;
    }
    lshift_blk7(cd, cd);
    permuted_choice_2(cd, sk_head);
    sk_head += 6;
    for(uchar i=0x0; i<0x6; i++){
        lshift_blk7(cd, cd); lshift_blk7(cd, cd);
        permuted_choice_2(cd, sk_head);
        sk_head += 6;
    }
    lshift_blk7(cd, cd);
    permuted_choice_2(cd, sk_head);
}

void encrypt(uchar* in_buffer, uchar* out_buffer, const size_t& in_buffer_sz, const size_t& out_blk_count){

}

void decrypt(uchar* in_buffer, uchar* out_buffer, const size_t& in_blk_count, size_t& out_buffer_sz){

}


