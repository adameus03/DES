#include <iostream>

using namespace std;
typedef unsigned char uchar;

/* DES functions */
uchar** divide_to_chunks(uchar* data, size_t bitlen);
uchar* initial_perm(uchar* blk);
uchar* final_perm(uchar* blk);
uchar* des_encrypt(uchar* plain, size_t bitlen);
uchar* des_decrypt(uchar* ciphertext);
/* Control functions */
bool test();
void printb(uchar* data, size_t );

/* Main function */
int main()
{   if(test()) return 0;

    cout << "DES Encrypt/Decrypt program" << endl;

    return 0;
}

uchar* des_encrypt(uchar* plain, size_t bitlen){
    uchar** chunks = divide_to_chunks(plain, bitlen);

}

uchar* des_decrypt(uchar* ciphertext){

}

uchar** divide_to_chunks(uchar* data, size_t bitlen){
    uchar** chunks = new uchar*;
    size_t blk_count = bitlen/64;
    if(bitlen % 64 != 0) blk_count++;
    for(size_t i=0; i<blk_count; i++){
        chunks[i] = data + (i<<3);
    }
    return chunks;
}

uchar* initial_perm(uchar* blk){
    uchar* p = new uchar[8]; //perm
    for(uchar s=0u; s<8u; s++){ //byte shift - offset prevar

        uchar o = (s%4u) << 1; //offset
        if(s<4u){ o++; }
        o=7u-o; // co-offset

        uchar* q = p+s; //perm byte ptr

        for(uchar b=0u; b<7u; b++){ //source byte ix
            uchar h1 = *(blk+7-b); //help
            uchar h2 = 1u<<o;
            uchar t = h1 & h2; //temp
            t >>= o;
            *q |= t;
            *q <<= 1;
        }

        uchar t = *blk & (1u<<o);
        t >>= o;
        *q |= t;
    }
    return p;
}

uchar* final_perm(uchar* blk){

}



/* TESTING */
bool test(){
    uchar* d = new uchar[8];
    *d     =  0b10110111;
    *(d+1) =  0b01111011;
    *(d+2) =  0b11101111;
    *(d+3) =  0b11011111;
    *(d+4) =  0b11011111;
    *(d+5) =  0b10111110;
    *(d+6) =  0b11110111;
    *(d+7) =  0b01101011;
    for(size_t i=0; i<8; i++){
        printb(d+i, 8);
        cout << endl;
    }
    cout << "----------------" << endl;
    cout << "Calculating IP..." << endl;
    uchar* p = initial_perm(d);
    cout << "Calculated IP successfully" << endl;
    cout << "----------------" << endl;

    for(size_t i=0; i<8; i++){
        printb(p+i, 8);
        cout << endl;
    }

    return 0;
}

void printb(uchar* data, size_t bitlen){
    size_t len = bitlen / 8;
    uchar blm = (uchar)bitlen%8; //bitlen mod

    for(size_t i=0; i<len; i++){
        uchar* c = data+i; //byte data chunk ptr
        for(uchar b=0; b<8u; b++){
            uchar m = 0x80 >> b; //mask
            uchar t = *c & m; //temp
            if(t >> (7u-b)) cout << "1 ";
            else cout << "0 ";
        }
    }
    if(blm != 0){
        uchar* c = data+len;
        for(uchar b=0; b<blm; b++){
            uchar m = 0x80 >> b;
            uchar t = *c & m;
            if(t >> (7u-b)) cout << "1 ";
            else cout << "0 ";
        }
    }
}
