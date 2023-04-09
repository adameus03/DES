#include <iostream>
#include "tests.h"

using namespace std;
typedef unsigned char uchar;

/* Main function */
int main()
{

    cout << "DES Encrypt/Decrypt program" << endl;

    checker(test_initial_perm(), "initial_perm");
    checker(test_final_perm(), "final_perm");
    checker(test_e_selection(), "e_selection");
    checker(test_sbox_combined(), "sbox_combined");
    checker(test_p_permutation(), "p_permutation");
    checker(test_permuted_choice_1(), "permuted_choice_1");

    return 0;
}








