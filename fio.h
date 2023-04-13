#ifndef FIO_H_INCLUDED
#define FIO_H_INCLUDED

void file_encrypt(const char* source_path, const char* destination_path, uchar* key);

void file_decrypt(const char* source_path, const char* destination_path, uchar* key);

#endif // FIO_H_INCLUDED
