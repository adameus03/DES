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

    return 0;
}








