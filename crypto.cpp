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
