#ifndef FIO_H_INCLUDED
#define FIO_H_INCLUDED

void file_encrypt(const char* source_path, const char* destination_path, uchar* key);

void file_decrypt(const char* source_path, const char* destination_path, uchar* key);

void load_key(const char* key_file_path, unsigned char* key);

void store_key(const char* destination_path, unsigned char* key);

#endif // FIO_H_INCLUDED
