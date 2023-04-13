#include <iostream>
#include <cstring>

#include "tests.h"
#include "fio.h"
#include "des.h"

using namespace std;
typedef unsigned char uchar;

void usage(char* argv0){
    std::cout << std::endl <<"Usage: " << argv0 << " {-e | -d} <input_file_path> <output_file_path> [key_file_path] [-r <key_store_file>] " << std::endl;
}


int main(int argc, char** argv)
{

    cout << "DES Encrypt/Decrypt program" << endl;

    if(argc < 2) usage(*argv);
    else if(strcmp("-e", argv[1]) && strcmp("-d", argv[1])) usage(*argv);
    else if(argc == 6){
        if(strcmp("-r", argv[4])) { usage(*argv); }
        else {
            unsigned char* k = new unsigned char[8];
            rand_key(k);
            store_key(argv[5], k);
            if(strcmp("-d", argv[1])){
                file_encrypt(argv[2], argv[3], k);
            }
            else file_decrypt(argv[2], argv[3], k);
            delete[] k;
        }

    }
    else if(argc == 5){
        unsigned char* k = new unsigned char[8];
        load_key(argv[4], k);
        if(strcmp("-d", argv[1])){
            file_encrypt(argv[2], argv[3], k);
        }
        else file_decrypt(argv[2], argv[3], k);
        delete[] k;
    }
    else usage(*argv);



    return 0;
}


/*checker(test_initial_perm(), "initial_perm");
    checker(test_final_perm(), "final_perm");
    checker(test_e_selection(), "e_selection");
    checker(test_sbox_combined(), "sbox_combined");
    checker(test_p_permutation(), "p_permutation");
    checker(test_permuted_choice_1(), "permuted_choice_1");
    checker(test_permuted_choice_2(), "permuted_choice_2");
    checker(test_xor_blks(), "xor_blks");
    checker(test_lshift_blk7(), "lshift_blk7");

    //visual_test_feistel_f();
    //visual_test_encrypt();

    checker(test_feistel_f(), "feistel_f");
    checker(test_encrypt_decrypt_blk(), "encrypt_blk, decrypt_blk, get_subkeys");*/





