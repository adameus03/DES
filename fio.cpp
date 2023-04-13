#include <fstream>
#include "des.h"

/*!
    @brief
        Wczytaj, szyfruj, zapisz plik
    @param source_path
        Sciezka pliku wejsciowego do zaszyfrowania
    @param destination_path
        Sciezka pliku wyjsciowego
    @param key
        Wskaznik do bufora zawierajacego 64-bitowy klucz do szyfrowania

*/
void file_encrypt(const char* source_path, const char* destination_path, uchar* key){
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size+8];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* dbuff = new uchar[sbuff_size+8];

    size_t blk_count;
    encrypt(sbuff, dbuff, key, sbuff_size, blk_count);

    delete[] sbuff;

    std::ofstream f_out(destination_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)dbuff, blk_count<<3);
    f_out.close();

    delete[] dbuff;
}


/*!
    @brief
        Wczytaj, deszyfruj, zapisz plik
    @param source_path
        Sciezka zaszyfrowanego pliku wejsciowego
    @param destination_path
        Sciezka pliku wyjsciowego
    @param key
        Wskaznik do bufora zawierajacego 64-bitowy klucz do szyfrowania

*/
void file_decrypt(const char* source_path, const char* destination_path, uchar* key){
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* dbuff = new uchar[sbuff_size];

    //size_t blk_count;
    //encrypt(sbuff, dbuff, key, sbuff_size, blk_count);
    size_t dbuff_size;
    decrypt(sbuff, dbuff, key, sbuff_size>>3, dbuff_size);

    delete[] sbuff;

    std::ofstream f_out(destination_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)dbuff, dbuff_size);
    f_out.close();

    delete[] dbuff;
}

/*!
    @brief
        Wczytaj klucz z pliku do bufora
    @param key_file_path
        Sciezka do pliku z kluczem. Rozmiar pliku powinien wynosic 8B
    @param key
        Wskaznik do bufora, w ktorym zostanie zapisany 64-bitowy klucz wczytany z pliku.
        Obszar pamieci bufora powinien byc zaalokowany przed wywolaniem funkcji.
*/
void load_key(const char* key_file_path, unsigned char* key){
    std::ifstream f_in(key_file_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    f_in_pbuf->pubseekpos(0, f_in.in);
    f_in_pbuf->sgetn((char*)key, 8);
    f_in.close();
}

/*!
    @brief
        Zapisz klucz z bufora do pliku
    @param destination_path
        Sciezka do pliku, gdzie zostanie zapisany klucz
    @param key
        Wskaznik do bufora, w ktorym jest zapisany 64-bitowy klucz
*/
void store_key(const char* destination_path, unsigned char* key){
    std::ofstream f_out(destination_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)key, 8);
    f_out.close();
}
