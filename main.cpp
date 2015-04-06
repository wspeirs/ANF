#include <iostream>

#include "anf.h"

using namespace std;

int main() {
    anf32 eq1('a');
    anf32 eq2('b');

    cout << eq1 << endl;
    cout << eq2 << endl;

    cout << eq1.XOR(eq2) << endl;
    cout << eq2.AND(eq2) << endl;

    return 0;
}