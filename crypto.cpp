typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

#include <iostream> //do usuni�cia po debugowaniu

/* Ta funkcja mo�e si� okaza� w og�le nie potrzebna :) */
/*uchar** divide_to_chunks(uchar* data, size_t bitlen, int& n){
    static uchar** chunks = new uchar*;
    size_t blk_count = bitlen/64;
    if(bitlen % 64 != 0) blk_count++;
    for(size_t i=0; i<blk_count; i++){
        chunks[i] = data + (i<<3);
    }
    return chunks;
}*/
void printu(unsigned long long& u){
    for(char c=0x0; c<0x40; c++){
        if((0x8000000000000000>>c)&u){
            std::cout << '1';
        }
        else std::cout << '0';
    }
    std::cout << std::endl;
}


void initial_perm(uchar* blk, uchar* p){ // perm
    for(uchar s=0x0; s<0x8; s++){ //byte shift - offset prevar
        uchar o = (s%0x4) << 1u; //offset
        if(s<0x4){ o++; }
        o=0x7-o; // co-offset
        uchar* q = p+s; //perm byte ptr
        *q = 0x0;
        for(uchar b=0u; b<0x7; b++){ //source byte ix
            uchar h1 = *(blk+7-b); //help
            uchar h2 = 0x1<<o;
            uchar t = h1 & h2; //temp
            t >>= o;
            *q |= t;
            *q <<= 0x1;
        }

        uchar t = *blk & (0x1<<o);
        t >>= o;
        *q |= t;
    }
}

void final_perm(uchar* blk, uchar* p){
    for(uchar s=0x0; s<0x8; s++){
        *(p+s) = 0xff;
    }
    for(uchar s=0x0; s<0x8; s++){ //byte shift - offset prevar

        uchar o = (s%0x4) << 0x1; //offset
        if(s<0x4){ o++; }
        o=0x7-o; // co-offset

        uchar* q = (uchar*)blk+s; //perm byte ptr

        for(uchar b=0x7; b!=0xff; b--){ //dest byte ix
            uchar h = *q >> b;
            h <<= o;
            h |= ~(0x1 << o);
            *(p+b) &= h;
        }
    }
}
/*
    32  1  2  3  4  5  4  5                 1  2  3  4  5  6  7  8
     6  7  8  9  8  9 10 11                 9 10 11 12 13 14 15 16
    12 13 12 13 14 15 16 17                17 18 19 20 21 22 23 24
    16 17 18 19 20 21 20 21                25 26 27 28 29 30 31 32
    22 23 24 25 24 25 26 27
    28 29 28 29 30 31 32 1
*/
void e_selection(uchar* blk, uchar* s){
    *s = *(blk+3) << 0x7;
    *s |= (*blk >> 0x1) & 0x7c;
    *s |= (*blk >> 0x3) & 0x3;

    /* ! */

    /*
        6  7  8  0  0  0  0  0      *(s+1) = (*blk << 0x5)
        %  %  %  9 10 11 12 13      *(blk+1) >> 0x3
        6  7  8  9 10 11 12 13      *(s+1) |= *(blk+1) >> 0x3;
        4  5  6  7  8  %  %  %      *blk << 0x3
        %  %  %  %  8  %  %  %      (*blk << 0x3) & 0x8
        6  7  8  9  ...
    */
    *(s+1) = (*blk << 0x5);
    *(s+1) |= (*(blk+1) >> 0x3) & 0x10;
    //*(s+1) &= (*blk << 0x3) | 0xf7;
    *(s+1) |= (*blk << 0x3) & 0x8;
    *(s+1) |= *(blk+1) >> 0x5;

    /* ! */
    /*
    32  1  2  3  4  5  4  5                 1  2  3  4  5  6  7  8
     6  7  8  9  8  9 10 11                 9 10 11 12 13 14 15 16
    12 13 12 13 14 15 16 17                17 18 19 20 21 22 23 24
    16 17 18 19 20 21 20 21                25 26 27 28 29 30 31 32
    22 23 24 25 24 25 26 27
    28 29 28 29 30 31 32 1
*/
    *(s+2) = (*(blk+1) << 0x3) & 0xc0;
    *(s+2) |= (*(blk+1) << 0x1) & 0x3e;
    *(s+2) |= *(blk+2) >> 0x7;

    *(s+3) = *(blk+1) << 0x7;
    *(s+3) |= (*(blk+2) >> 0x1) & 0x7c;
    *(s+3) |= (*(blk+2) >> 0x3) & 0x3;

    /* ! */
    *(s+4) = *(blk+2) << 0x5;
    *(s+4) |= (*(blk+3) >> 0x3) & 0x10;
    *(s+4) |= (*(blk+2) << 0x3) & 0x8;
    *(s+4) |= *(blk+3) >> 0x5;

    *(s+5) = (*(blk+3) << 0x3) & 0xc0;
    *(s+5) |= (*(blk+3) << 0x1) & 0x3e;
    *(s+5) |= *blk >> 0x7;
}

/*
    u : 8*6 = 48b = 6B
    v : 8*4 = 32b = 4B
*/
void sbox_combined(uchar* _u, uchar* _v){
    /*
        xxxxxx xxxxxx xxxxxx xxxxxx xxxxxx xxxxxx xxxxxx xxxxxx
         xxxx   xxxx   xxxx   xxxx   xxxx   xxxx   xxxx   xxxx
    */
    /*
                                  vb3                                                        vb2                                                        vb1                                                        vb0
          S1     S2     S3     S4     S5     S6     S7     S8        S1     S2     S3     S4     S5     S6     S7     S8        S1     S2     S3     S4     S5     S6     S7     S8        S1     S2     S3     S4     S5     S6     S7     S8
        000000 000000 000000 000001 000001 000000 000001 000000    000000 000000 000011 000000 000001 000000 000000 000000    000000 000000 000000 000000 000000 000010 000001 000001    000010 000000 000010 000000 000011 000001 000001 000000
        000010 000010 000010 000010 000101 000010 000011 000010    000001 000011 000100 000001 000010 000011 000011 000011    000100 000011 000011 000010 000100 000011 000010 000100    000011 000001 000101 000001 000100 000011 000100 000101
        000101 000011 000011 000110 000110 000011 000100 000101    000010 000100 000110 000010 000100 000110 000100 000100    000101 000100 000100 000011 000101 000101 000011 000101    000101 000101 000110 000011 000110 000100 000111 000110
        000110 000101 000110 000111 001000 000100 000110 000110    000101 000111 000111 000101 000111 001001 000111 000101    000110 000101 000101 000101 000110 000110 000100 000110    000110 000110 000111 000110 001001 001001 001000 000111
        000111 001000 001001 001010 001011 000111 000111 001000    001010 001001 001001 001011 001001 001011 001001 001011    001000 000110 000110 000111 000111 001010 001000 001000    001000 001000 001000 001000 001010 001010 001010 001001
        001001 001011 001010 001100 001100 001001 001001 001001    001011 001011 001010 001101 001011 001100 001011 001100    001001 001001 001011 001001 001010 001100 001011 001010    001100 001001 001001 001011 001011 001101 001011 001010
        001011 001100 001100 001101 001110 001100 001010 001011    001100 001100 001011 001110 001100 001101 001100 001110    001010 001010 001100 001100 001011 001101 001101 001011    001101 001011 001011 001100 001100 001110 001100 001100
        001101 001111 001111 001111 001111 001111 001101 001110    001111 001110 001101 001111 001110 001110 001110 001111    001111 001111 001110 001111 001110 001111 001110 001111    001111 001110 001100 001111 001111 001111 001111 001111
        010001 010001 010000 010000 010000 010000 010000 010001    010001 010001 010000 010000 010000 010001 010000 010001    010001 010000 010001 010010 010000 010000 010010 010001    010001 010000 010000 010000 010001 010001 010000 010000
        010100 010100 010011 010001 010001 010001 010010 010010    010010 010010 010001 010011 010011 010010 010011 010010    010010 010011 010100 010100 010001 010001 010011 010100    010010 010001 010001 010010 010101 010100 010010 010001
        010110 010110 010111 010010 010011 010101 010101 010011    010011 010011 010101 010100 010100 010100 010100 010110    010100 010100 010110 010101 010010 010011 010111 010101    010110 010011 010011 010011 010110 010110 010011 010010
        011000 010111 011001 010101 010110 010110 010111 010100    010100 010100 010110 010101 010101 010101 011000 010111    010101 010101 010111 010111 010101 010100 011000 010110    010111 010100 010100 010101 010111 010111 010101 010101
        011010 011000 011011 011011 011010 011010 011000 011000    010110 010111 011010 011000 010110 010111 011010 011000    011000 010111 011000 011001 011010 011000 011001 011010    011011 011010 011010 010111 011000 011001 010110 010110
        011011 011011 011100 011101 011011 011011 011011 011011    011001 011000 011011 011001 011000 011000 011011 011001    011001 011011 011011 011010 011011 011011 011100 011011    011100 011101 011101 011001 011010 011010 011001 011001
        011100 011101 011101 011110 011101 011101 011101 011101    011010 011100 011100 011011 011010 011010 011101 011010    011011 011100 011101 011101 011100 011101 011101 011101    011101 011110 011110 011100 011100 011101 011010 011011
        011111 011110 011110 011111 011110 011111 011110 011110    011101 011111 011110 011110 011111 011011 011111 011101    011110 011110 011110 011110 011111 011110 011111 011111    011110 011111 011111 011111 011101 011110 011101 011110
        100010 100001 100000 100000 100011 100000 100010 100001    100000 100001 100000 100001 100000 100001 100001 100000    100010 100001 100001 100000 100001 100001 100010 100000    100001 100010 100000 100010 100010 100000 100000 100000
        100011 100011 100011 100010 100100 100001 100011 100100    100010 100010 100001 100100 100101 100010 100011 100010    100101 100010 100101 100001 100011 100010 100101 100001    100100 100011 100011 100101 100011 100010 100010 100001
        100100 100100 100100 100100 100101 100010 100100 100101    100100 100101 100010 100110 100110 100011 100100 100101    100110 100011 100110 100101 100100 100100 100110 100110    100111 100110 100101 100110 100101 100011 100011 100011
        100111 100110 100101 100101 100111 100101 100111 100110    100101 100110 100101 100111 101000 100110 100110 100110    100111 100100 101000 100110 100110 100111 100111 100111    101000 100111 100110 100111 100110 100111 100101 100100
        101000 101001 101000 100111 101000 100110 101000 101010    101000 101000 101011 101000 101010 101000 100111 101001    101000 101011 101010 101000 101000 101000 101000 101001    101010 101000 101000 101000 101000 101000 100110 101011
        101001 101010 101011 101000 101001 101011 101001 101011    101001 101010 101100 101011 101011 101010 101001 101011    101011 101101 101101 101010 101100 101011 101001 101010    101011 101100 101001 101001 101001 101100 101001 101100
        101010 101100 101101 101011 101010 101101 101011 101100    101011 101011 101110 101100 101100 101101 101010 101100    101100 101110 101110 101011 101101 101110 101010 101100    101100 101101 101100 101010 101011 101101 101101 101101
        101101 101111 101110 101110 101111 101110 101110 101111    101110 101111 101111 101111 101111 101111 101101 101110    101101 101111 101111 101101 101111 101111 101111 101101    101110 101111 101111 101100 101101 101110 101110 101110
        110000 110000 110001 110001 110000 110011 110001 110010    110000 110000 110010 110001 110010 110000 110000 110010    110000 110010 110001 110000 110000 110001 110000 110000    110000 110000 110000 110000 110000 110001 110001 110001
        110001 110001 110010 110100 110001 110100 110010 110101    110001 110101 110100 110011 110011 110011 110010 110011    110011 110100 110100 110001 110011 110010 110001 110010    110101 110011 110010 110001 110011 110100 110010 110011
        110010 110010 110101 110110 110010 110110 110011 110110    110100 110110 110111 110110 110101 110101 110101 110100    110111 110101 110111 110011 110101 110110 110110 110011    110110 110100 110101 110101 110100 110101 110100 110111
        110101 110101 110110 110111 110101 110111 110110 110111    110111 111001 111000 111001 110111 110110 110111 110111    111001 110111 111001 110100 110110 110111 110111 110101    110111 110101 110111 110110 110111 110110 110111 111000
        111001 111000 111001 111000 110111 111000 111000 111000    111000 111010 111001 111010 111000 111001 111001 111000    111010 111000 111010 111011 111000 111000 111011 111000    111000 111000 111001 111000 111001 111000 111000 111010
        111011 111011 111010 111011 111001 111001 111011 111001    111011 111011 111010 111100 111001 111011 111011 111001    111011 111001 111011 111101 111001 111001 111100 111100    111001 111010 111011 111010 111011 111010 111001 111100
        111100 111110 111100 111100 111011 111110 111100 111010    111110 111101 111101 111101 111101 111100 111100 111101    111100 111010 111100 111110 111100 111011 111101 111110    111010 111101 111100 111011 111110 111011 111011 111101
        111111 111111 111111 111111 111100 111111 111111 111111    111111 111110 111111 111111 111110 111111 111111 111110    111110 111110 111110 111111 111111 111100 111110 111111    111111 111111 111101 111101 111111 111111 111110 111111
    */


    /*
                             v+3                                                      v+2                                                      v+1                                                        v+0
                            S1-8                                                     S1-8                                                     S1-8                                                       S1-8
    00000000 00000000 00000001 00000100 00000000 01000000    00000000 00000000 11000000 00000100 00000000 00000000    00000000 00000000 00000000 00000000 00100000 01000001      00001000 00000000 10000000 00001100 00010000 01000000
    00001000 00100000 10000010 00010100 00100000 11000010    00000100 00110001 00000001 00001000 00110000 11000011    00010000 00110000 11000010 00010000 00110000 10000100      00001100 00010001 01000001 00010000 00110001 00000101
    00010100 00110000 11000110 00011000 00110001 00000101    00001000 01000001 10000010 00010000 01100001 00000100    00010100 01000001 00000011 00010100 01010000 11000101      00010100 01010001 10000011 00011000 01000001 11000110
    00011000 01010001 10000111 00100000 01000001 10000110    00010100 01110001 11000101 00011100 10010001 11000101    00011000 01010001 01000101 00011000 01100001 00000110      00011000 01100001 11000110 00100100 10010010 00000111
    00011100 10000010 01001010 00101100 01110001 11001000    00101000 10010010 01001011 00100100 10110010 01001011    00100000 01100001 10000111 00011100 10100010 00001000      00100000 10000010 00001000 00101000 10100010 10001001
    00100100 10110010 10001100 00110000 10010010 01001001    00101100 10110010 10001101 00101100 11000010 11001100    00100100 10010010 11001001 00101000 11000010 11001010      00110000 10010010 01001011 00101100 11010010 11001010
    00101100 11000011 00001101 00111000 11000010 10001011    00110000 11000010 11001110 00110000 11010011 00001110    00101000 10100011 00001100 00101100 11010011 01001011      00110100 10110010 11001100 00110000 11100011 00001100
    00110100 11110011 11001111 00111100 11110011 01001110    00111100 11100011 01001111 00111000 11100011 10001111    00111100 11110011 10001111 00111000 11110011 10001111      00111100 11100011 00001111 00111100 11110011 11001111
    01000101 00010100 00010000 01000001 00000100 00010001    01000101 00010100 00010000 01000001 00010100 00010001    01000101 00000100 01010010 01000001 00000100 10010001      01000101 00000100 00010000 01000101 00010100 00010000
    01010001 01000100 11010001 01000101 00010100 10010010    01001001 00100100 01010011 01001101 00100100 11010010    01001001 00110101 00010100 01000101 00010100 11010100      01001001 00010100 01010010 01010101 01000100 10010001
    01011001 01100101 11010010 01001101 01010101 01010011    01001101 00110101 01010100 01010001 01000101 00010110    01010001 01000101 10010101 01001001 00110101 11010101      01011001 00110100 11010011 01011001 01100100 11010010
    01100001 01110110 01010101 01011001 01100101 11010100    01010001 01000101 10010101 01010101 01010110 00010111    01010101 01010101 11010111 01010101 01000110 00010110      01011101 01000101 00010101 01011101 01110101 01010101
    01101001 10000110 11011011 01101001 10100110 00011000    01011001 01110110 10011000 01011001 01110110 10011000    01100001 01110110 00011001 01101001 10000110 01011010      01101101 10100110 10010111 01100001 10010101 10010110
    01101101 10110111 00011101 01101101 10110110 11011011    01100101 10000110 11011001 01100001 10000110 11011001    01100101 10110110 11011010 01101101 10110111 00011011      01110001 11010111 01011001 01101001 10100110 01011001
    01110001 11010111 01011110 01110101 11010111 01011101    01101001 11000111 00011011 01101001 10100111 01011010    01101101 11000111 01011101 01110001 11010111 01011101      01110101 11100111 10011100 01110001 11010110 10011011
    01111101 11100111 10011111 01111001 11110111 10011110    01110101 11110111 10011110 01111101 10110111 11011101    01111001 11100111 10011110 01111101 11100111 11011111      01111001 11110111 11011111 01110101 11100111 01011110
    10001010 00011000 00100000 10001110 00001000 10100001    10000010 00011000 00100001 10000010 00011000 01100000    10001010 00011000 01100000 10000110 00011000 10100000      10000110 00101000 00100010 10001010 00001000 00100000
    10001110 00111000 11100010 10010010 00011000 11100100    10001010 00101000 01100100 10010110 00101000 11100010    10010110 00101001 01100001 10001110 00101001 01100001      10010010 00111000 11100101 10001110 00101000 10100001
    10010010 01001001 00100100 10010110 00101001 00100101    10010010 01011000 10100110 10011010 00111001 00100101    10011010 00111001 10100101 10010010 01001001 10100110      10011110 01101001 01100110 10010110 00111000 11100011
    10011110 01101001 01100101 10011110 01011001 11100110    10010110 01101001 01100111 10100010 01101001 10100110    10011110 01001010 00100110 10011010 01111001 11100111      10100010 01111001 10100111 10011010 01111001 01100100
    10100010 10011010 00100111 10100010 01101010 00101010    10100010 10001010 11101000 10101010 10001001 11101001    10100010 10111010 10101000 10100010 10001010 00101001      10101010 10001010 00101000 10100010 10001001 10101011
    10100110 10101010 11101000 10100110 10111010 01101011    10100110 10101011 00101011 10101110 10101010 01101011    10101110 11011011 01101010 10110010 10111010 01101010      10101110 11001010 01101001 10100110 11001010 01101100
    10101010 11001011 01101011 10101010 11011010 11101100    10101110 10111011 10101100 10110010 11011010 10101100    10110010 11101011 10101011 10110110 11101010 10101100      10110010 11011011 00101010 10101110 11011011 01101101
    10110110 11111011 10101110 10111110 11101011 10101111    10111010 11111011 11101111 10111110 11111011 01101110    10110110 11111011 11101101 10111110 11111011 11101101      10111010 11111011 11101100 10110110 11101011 10101110
    11000011 00001100 01110001 11000011 00111100 01110010    11000011 00001100 10110001 11001011 00001100 00110010    11000011 00101100 01110000 11000011 00011100 00110000      11000011 00001100 00110000 11000011 00011100 01110001
    11000111 00011100 10110100 11000111 01001100 10110101    11000111 01011101 00110011 11001111 00111100 10110011    11001111 01001101 00110001 11001111 00101100 01110010      11010111 00111100 10110001 11001111 01001100 10110011
    11001011 00101101 01110110 11001011 01101100 11110110    11010011 01101101 11110110 11010111 01011101 01110100    11011111 01011101 11110011 11010111 01101101 10110011      11011011 01001101 01110101 11010011 01011101 00110111
    11010111 01011101 10110111 11010111 01111101 10110111    11011111 10011110 00111001 11011111 01101101 11110111    11100111 01111110 01110100 11011011 01111101 11110101      11011111 01011101 11110110 11011111 01101101 11111000
    11100111 10001110 01111000 11011111 10001110 00111000    11100011 10101110 01111010 11100011 10011110 01111000    11101011 10001110 10111011 11100011 10001110 11111000      11100011 10001110 01111000 11100111 10001110 00111010
    11101111 10111110 10111011 11100111 10011110 11111001    11101111 10111110 10111100 11100111 10111110 11111001    11101111 10011110 11111101 11100111 10011111 00111100      11100111 10101110 11111010 11101111 10101110 01111100
    11110011 11101111 00111100 11101111 11101111 00111010    11111011 11011111 01111101 11110111 11001111 00111101    11110011 10101111 00111110 11110011 10111111 01111110      11101011 11011111 00111011 11111011 10111110 11111101
    11111111 11111111 11111111 11110011 11111111 11111111    11111111 11101111 11111111 11111011 11111111 11111110    11111011 11101111 10111111 11111111 11001111 10111111      11111111 11111111 01111101 11111111 11111111 10111111

    */
    ull S[128] = {0xfffffefbffbf,0xf7df7debdf3d,0xebcf39e7cefa,0xe7ae78dfbe79,0xe37db5d38e37,0xdb4d73cf6db6,0xd33cf2c73d74,0xcb0c30c30cb1,
                  0xbaebefbefbee,0xaebb2ebaeb6d,0xa69a2db2aaac,0x9e89aaa69a2b,0x9679249659e7,0x9248e2924924,0x8e28a18a28a2,0x821860860861,
                  0x75e7df71f75e,0x71c71d6de71b,0x6db6db69d6da,0x61969a61a619,0x5d65d85d95d5,0x595517594594,0x553494552513,0x490451411450,
                  0x3cf38e3cc38d,0x38e34b38b34a,0x34d289349309,0x28a248288248,0x1871872071c7,0x104144186106,0xc10c31010c5,0xc0000,
                  0xffff3ffbffff,0xfbcefef7cf3c,0xf7be7def9efb,0xeb8e3ae36e3a,0xd76db4db4db4,0xd34d72d33d33,0xcf3d31cf2cf1,0xc31cb0c71c70,
                  0xbaebefbeebee,0xb6dbacb2db2d,0xb2caabaebae9,0xaeba6aaaa9e8,0xa68967a68967,0x9a79269e7926,0x9638e49658a5,0x8a0861824822,
                  0x7de7de7de79f,0x75d79b69d71d,0x6da75965c6da,0x6996985d9659,0x5d7517557616,0x5954d4515594,0x5144534d2553,0x450410410491,
                  0x3cf34e34f3cd,0x38a2cc30c34c,0x2c920928a28b,0x2061c6209208,0x1c51851c6187,0x104143184106,0x420820830c2,0x1000040001,
                  0xffffffffdfbe,0xefcf3defcf7b,0xebbefcebaf3a,0xe7aebae79ef9,0xdf9e78e35df7,0xdb6d36d73d35,0xd75cf3d32cb4,0xc30c70c70c70,
                  0xbafbadbefb6e,0xb6caebbaeb2b,0xaeba6ab6ca2a,0xaaaa28aab9e9,0x9e89e69679a5,0x9a49259248e4,0x9238a28e28a2,0x861861821820,
                  0x75e79f79e75f,0x69d69e71d69e,0x65c65a6db659,0x61b5d9658618,0x5d45575d75d6,0x5124954d4595,0x4d1454491553,0x490412410412,
                  0x3cd38f3ce3cf,0x30b2ce30d38d,0x20a20c28924c,0x18818b24820a,0x1471441c7107,0x1061021860c3,0xc50c10c4081,0x41040003040,
                  0xf7ff7ff3efbf,0xf3eebeefcefa,0xebae7ce7be39,0xe79e39db6df8,0xdf7df7d75d76,0xcf6db4d32d35,0xcb4d33cf1cf3,0xc31cf0c30c30,
                  0xbafbefbaebef,0xb6ebadaabb6e,0xa6cb2ca69b2d,0xa2baeaa28aaa,0x9259689e6a69,0x8e28a69659a6,0x8a18638e2964,0x8608208a18a1,
                  0x79d7dd75f7df,0x6dc71a71d75e,0x61969969c71c,0x5d865865869b,0x5575d75d7654,0x513596593593,0x4d24d5512492,0x450413491451,
                  0x3cf3cf3ce38e,0x28c34e30b34c,0x24b28a2ca2c8,0x20a209209207,0x1c71871871c5,0x185105105183,0x1420c4044102,0x82000040};


    ull u = ((ull)(*_u)<<0x28) | ((ull)(*(_u+1))<<0x20) | ((ull)(*(_u+2))<<0x18) | ((ull)(*(_u+3))<<0x10) | ((ull)(*(_u+4))<<0x8) | (ull)(*(_u+5));
    ull v = 0x0;

    ull* h;
    for(uchar i=0; i<0x4; i++){
        ull a;
        ull b = 0x0;
        h = S+(int)(i<<0x5);

        for(uchar j=0; j<0x20; j++){
            a = u ^ *h++;
            a &= (a<<0x1) & (a<<0x2) & (a<<0x3) & (a<<0x4) & (a<<0x5);
            b |= a;
        }

        b &= 0x820820820820;

        /*
            00000000 00000000 A00000B0 0000C000 00D00000 E00000F0 0000G000 00H00000    b

            00000000 00000000 00000A00 000B0000 0C00000D 00000E00 000F0000 0G00000H    b>>0x5
            00000000 00000000 00000000 00A00000 B00000C0 0000D000 00E00000 F00000G0    b>>0xa
            00000000 00000000 00000000 0000000A 00000B00 000C0000 0D00000E 00000F00    b>>0xf
            00000000 00000000 00000000 00000000 0000A000 00B00000 C00000D0 0000E000    b>>0x14

            00000000 00000000 00000A00 00AB000A BC00ABCD 00BCDE00 CDEF000E FG00EFGH    b=(b>>0x5)|(b>>0xa)|(b>>0xf)|(b>>0x14)
            00000000 00000000 00000000 00000000 00001111 00000000 00000000 00001111    0xf00000f
            00000000 00000000 00000000 00000000 0000ABCD 00000000 00000000 0000EFGH    b &= 0xf00000f
            00000000 00000000 00000000 00000000 00000000 00000000 00000000 ABCD0000    b >> 0x14
            00000000 00000000 00000000 00000000 0000ABCD 00000000 00000000 ABCDEFGH    b |= b >> 0x14
            00000000 00000000 00000000 00000000 00000000 00000000 00000000 ABCDEFGH    b &= 0xff


        */
        b = (b>>0x5) | (b>>0xa) | (b>>0xf) | (b>>0x14);
        b &= 0xf00000f;
        b |= b >> 0x14;
        b &= 0xff;

        ull m = ((b<<0x3)&0x8) | ((b<<0x6)&0x80) | ((b<<0x9)&0x800) | ((b<<0xc)&0x8000) | ((b<<0xf)&0x80000) | ((b<<0x12)&0x800000) | ((b<<0x15)&0x8000000) | ((b<<0x18)&0x80000000);
        v |= m >> i;
    }

    /*
        v00 v01 v02 v03 v04 v05 v06 v07    v08 v09 v10 v11 v12 v13 v14 v15    v16 v17 v18 v19 v20 v21 v22 v23    v24 v25 v26 v27 v28 v29 v30 v31        v
          0   0   0   0   0   0   0   0      0   0   0   0   0   0   0   0      0   0   0   0   0   0   0   0    v00 v01 v02 v03 v04 v05 v06 v07        v >> 0x18
          0   0   0   0   0   0   0   0      0   0   0   0   0   0   0   0    v00 v01 v02 v03 v04 v05 v06 v07    v08 v09 v10 v11 v12 v13 v14 v15        v >> 0x10
          0   0   0   0   0   0   0   0    v00 v01 v02 v03 v04 v05 v06 v07    v08 v09 v10 v11 v12 v13 v14 v15    v16 v17 v18 v19 v20 v21 v22 v23        v >> 0x8
    */

    *_v = v >> 0x18;
    *(_v+1) = (v >> 0x10) & 0xff;
    *(_v+2) = (v >> 0x8) & 0xff;
    *(_v+3) = v & 0xff;

}
/*
    16  7 20 21 29 12 28 17           1  2  3  4  5  6  7  8
     1 15 23 26  5 18 31 10           9 10 11 12 13 14 15 16
     2  8 24 14 32 27  3  9          17 18 19 20 21 22 23 24
    19 13 30  6 22 11  4 25          25 26 27 28 29 30 31 32
*/
void p_permutation(uchar* d, uchar* p){
    *p = (*(d+1)<<0x7) | ((*d<<0x5)&0x40) | ((*(d+2)<<0x1)&0x20) | ((*(d+2)<<0x1)&0x10) | (*(d+3)&0x8) | ((*(d+1)>>0x2)&0x4) | ((*(d+3)>>0x3)&0x2) | (*(d+2)>>0x7);
    *(p+1) = (*d&0x80) | ((*(d+1)<<0x5)&0x40) | ((*(d+2)<<0x4)&0x20) | ((*(d+3)>>0x2)&0x10) | (*d&0x8) | ((*(d+2)>>0x4)&0x4) | (*(d+3)&0x2) | ((*(d+1)>>0x6)&0x1);
    *(p+2) = ((*d<<0x1)&0x80) | ((*d<<0x6)&0x40) | ((*(d+2)<<0x5)&0x20) | ((*(d+1)<<0x2)&0x10) | ((*(d+3)<<0x3)&0x8) | ((*(d+3)>>0x3)&0x4) | ((*d>>0x4)&0x2) | (*(d+1)>>0x7);
    *(p+3) = ((*(d+2)<<0x2)&0x80) | ((*(d+1)<<0x3)&0x40) | ((*(d+3)<<0x3)&0x20) | ((*d<<0x2)&0x10) | ((*(d+2)<<0x1)&0x8) | ((*(d+1)>>0x3)&0x4) | ((*d>>0x3)&0x2) | (*(d+3)>>0x7);
}


/*
    [C]
    57 49 41 33 25 17  9
     1 58 50 42 34 26 18
    10  2 59 51 43 35 27
    19 11  3 60 52 44 36

    [D]
    63 55 47 39 31 23 15
     7 62 54 46 38 30 22
    14  6 61 53 45 37 29
    21 13  5 28 20 12  4

----------------------------------------------------------------------------
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
void permuted_choice_1(uchar* k, uchar* cd){
    for(uchar i=0x0; i<0x3; i++){
        *(cd+i) = 0x0;
        for(uchar j=0x0; j<i; j++){
            *(cd+i) |= (*(k+7-j)<<(i-j)) & (0x80>>j);
        }
        for(uchar j=i; j<0x8; j++){
            *(cd+i) |= (*(k+7-j)>>(j-i)) & (0x80>>j);
        }
    }

    *(cd+3) = 0x0;
    for(uchar i=0x0; i<0x4; i++){
        *(cd+3) |= (*(k+7-i)<<(0x3-i)) & (0x80>>i);
    }

    for(uchar i=0x0; i<0x3; i++){
        *(cd+3) |= (*(k+7-i)<<(0x2-i)) & (0x8>>i);
    }
    *(cd+3) |= (*(k+4)>>0x1) & 0x40;

    for(uchar i=0x0; i<0x3; i++){
        *(cd+4+i) = 0x0;
        for(uchar j=0; j<0x4; j++){
            *(cd+4+i) |= (*(k+3-j)<<(0x6-i-j)) & (0x80>>j);
        }
    }
    *(cd+4) |= (*(k+7)<<0x1) & 0x8;
    for(uchar i=0x1; i<0x4; i++){
        *(cd+4) |= (*(k+7-i)>>(i-0x1)) & (0x8>>i);
    }
    for(uchar i=0x0; i<0x4; i++){
        *(cd+5) |= (*(k+7-i)>>i) & (0x8>>i);
    }
    for(uchar i=0x0; i<0x4; i++){
        *(cd+6) |= (*(k+3-i)>>(i+0x1)) & (0x8>>i);
    }
}
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
void permuted_choice_2(uchar* cd, uchar* k){
    *k = ((*(cd+1)<<0x5)&0x80) | ((*(cd+2)>>0x1)&0x40) | (*(cd+1)&0x20) | ((*(cd+2)<<0x4)&0x10) | ((*cd>>0x4)&0x8) | ((*cd>>0x1)&0x4) | ((*cd>>0x4)&0x2) | ((*(cd+3)>>0x4)&0x1);
    *(k+1) = ((*(cd+1)<<0x6)&0x80) | ((*cd<<0x4)&0x40) | ((*(cd+2)<<0x2)&0x20) | ((*(cd+1)>>0x2)&0x10) | ((*(cd+2)<<0x2)&0x8) | ((*(cd+2)>>0x3)&0x4) | ((*(cd+1)>>0x3)&0x2) | ((*cd>>0x4)&0x1);
    *(k+2) = ((*(cd+3)<<0x1)&0x80) | ((*cd<<0x6)&0x40) | ((*(cd+1)<<0x5)&0x20) | ((*cd<<0x3)&0x10) | ((*(cd+3)>>0x2)&0x8) | ((*(cd+2)>>0x2)&0x4) | ((*(cd+1)>>0x2)&0x2) | ((*cd>>0x6)&0x1);
    *(k+3) = (*(cd+5)&0x80) | ((*(cd+6)<<0x2)&0x40) | ((*(cd+3)<<0x4)&0x20) | ((*(cd+4)<<0x1)&0x10) | ((*(cd+5)<<0x2)&0x8) | ((*(cd+6)<<0x1)&0x4) | ((*(cd+3)>>0x1)&0x2) | (*(cd+4)&0x1);
    *(k+4) = ((*(cd+6)<<0x2)&0x80) | ((*(cd+5)<<0x3)&0x40) | ((*(cd+4)>>0x2)&0x20) | ((*(cd+5)<<0x4)&0x10) | ((*(cd+5)>>0x1)&0x8) | ((*(cd+6)>>0x5)&0x4) | (*(cd+4)&0x2) | (*(cd+6)&0x1);
    *(k+5) = ((*(cd+4)<<0x1)&0x80) | ((*(cd+6)<<0x3)&0x40) | ((*(cd+5)<<0x3)&0x20) | ((*(cd+5)>>0x2)&0x10) | ((*(cd+6)>>0x3)&0x8) | ((*(cd+4)>>0x2)&0x4) | ((*(cd+3)>>0x2)&0x2) | (*(cd+3)&0x1);

}

void xor_blks(uchar* a, uchar* b, uchar* r, const uchar& blk_len){
    for(uchar i=0x0; i<blk_len; i++){
        *(r+i) = *(a+i) ^ *(b+i);
    }
}

void feistel_f(uchar* r, uchar* k, uchar* f){
    uchar* s = new uchar[6];
    e_selection(r, s);
    xor_blks(s, k, s, 0x6);
    sbox_combined(s, s);
    p_permutation(s, f);
}
