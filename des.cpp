#include <cstdlib>
#include <cstring>
#include "crypto.h"
#include "tests.h"
#include <iostream>

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
    std::cout << "Szyfrowanie bloku: " << std::endl;
    print_blk(blk, 8);
    std::cout << "-----------------------------" << std::endl;


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
    std::cout << "Deszyfrowanie bloku: " << std::endl;
    print_blk(blk, 8);
    std::cout << "-----------------------------" << std::endl;


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

/*!
    @brief
        Funkcja szyfrujaca bufor algorytmem DES
    @param in_buffer
        Wskaznik na bufor wejsciowy. Bufor powinien byc przygotowany na dopisanie 8B dodatkowych danych.
    @param out_buffer
        Wskaznik na bufor wyjsciowy. Obszar pamieci buforu powinien zostac zaalokowany zanim zostanie wywolana ta funkcja.
    @param key
        Wskaznik na zawartosc 64-bitowego klucza glownego dla algorytmu DES
    @param in_buffer_sz
        Rozmiar danych w buforze in_buffer przeznaczonych do zaszyfrowania i znajdujacych sie na poczatku bufora
    @param out_blk_count
        Referencja do zmiennej, w ktorej zapisana bedzie ilosc 64-bitowych blokow wygenerowanego szyfrogramu

*/
void encrypt(uchar* in_buffer, uchar* out_buffer, uchar* key, const size_t& in_buffer_sz, size_t& out_blk_count){
    std::cout << "in_buffer_sz: " << in_buffer_sz << std::endl;
    std::cout << "in_buffer: " << std::endl;
    print_blk(in_buffer, 8);
    std::cout << "++++++++++++++++++++++++++++++" << std::endl;

    uchar* sk = new uchar[96];
    get_subkeys(key, sk);

    out_blk_count = 1+(in_buffer_sz>>3);

    uchar* in_buff_head = in_buffer;
    uchar* out_buff_head = out_buffer;

    for(size_t i=0; i<out_blk_count-1; i++){
        encrypt_blk(in_buff_head, sk, out_buff_head);
        in_buff_head += 8;
        out_buff_head += 8;
    }

    uchar rem = in_buffer_sz  % 8;

    if(!in_buffer_sz){
        ++out_blk_count;
        encrypt_blk(in_buff_head, sk, out_buff_head);
        in_buff_head += 8;
        out_buff_head += 8;
    }
    else in_buff_head += rem;

    for(uchar i=rem; i<0x7; i++){
        *in_buff_head++ = 0x0;
    }

    *in_buff_head = rem;

    encrypt_blk(in_buff_head-7, sk, out_buff_head);

    std::cout << "out_blk_count: " << out_blk_count << std::endl;
    std::cout << "out_buffer: " << std::endl;
    print_blk(out_buffer, 8);
    std::cout << "++++++++++++++++++++++++++++++" << std::endl;

}

/*!
    @brief
        Funkcja deszyfrujaca bufor algorytmem DES
    @param in_buffer
        Wskaznik na zaszyfrowany bufor wejsciowy.
    @param out_buffer
        Wskaznik na zdeszyfrowany bufor wyjsciowy. Obszar pamieci buforu powinien zostac zaalokowany zanim zostanie wywolana ta funkcja.
    @param key
        Wskaznik na zawartosc 64-bitowego klucza glownego dla algorytmu DES
    @param in_blk_count
        Ilosc 64-bitowych blokow szyfrogramu zawartego w buforze in_buffer
    @param out_buffer_sz
        Referencja do zmiennej, w ktorej zapisana zostanie wykryta liczba bajtow danych zdeszyfrowanych do bufora out_buffer

*/
void decrypt(uchar* in_buffer, uchar* out_buffer, uchar* key, const size_t& in_blk_count, size_t& out_buffer_sz){
    uchar* sk = new uchar[96];
    get_subkeys(key, sk);

    uchar* in_buff_head = in_buffer;
    uchar* out_buff_head = out_buffer;

    for(size_t i=0; i<in_blk_count; i++){
        decrypt_blk(in_buff_head, sk, out_buff_head);
        in_buff_head += 8;
        out_buff_head += 8;
    }

    uchar rem = *(out_buff_head-1);
    out_buffer_sz = rem+((in_blk_count-1)<<3);

}

/*!
    @brief
        Generate a random odd-parity 64-bit key
    @param k
        Wskaznik na 64-bitowy bufor wyjsciowy. Obszar pamieci buforu powinien zostac zaalokowany zanim zostanie wywolana ta funkcja

*/
void rand_key(uchar* k){
    uchar* h = k;
    uchar t;
    for(uchar i=0x0; i<0x8; i++){
        *h = rand() % 256;
        t = *h;
        t ^= t << 0x1;
        t ^= t << 0x2;
        t ^= t << 0x4;
        if(!t) *h ^= 0x1;
        h++;
    }
}


