typedef unsigned char uchar;

/* Ta funkcja mo¿e siê okazaæ w ogóle nie potrzebna :) */
/*uchar** divide_to_chunks(uchar* data, size_t bitlen, int& n){
    static uchar** chunks = new uchar*;
    size_t blk_count = bitlen/64;
    if(bitlen % 64 != 0) blk_count++;
    for(size_t i=0; i<blk_count; i++){
        chunks[i] = data + (i<<3);
    }
    return chunks;
}*/

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
