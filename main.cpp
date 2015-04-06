#include <iostream>

#include "anf.h"

using namespace std;

int main() {
    anf_c32 x('x');
    anf_c32 y('y');
    anf_c32 z('z');

    // see http://en.wikipedia.org/wiki/Algebraic_normal_form#Performing_operations_within_algebraic_normal_form
    //cout << x.OR(y.AND(z.NOT())) << endl;

    // this is just x XOR y
    auto t = x.AND(y.NOT()).OR(x.NOT().AND(y));

    cout << t << t.size() << endl;

    /*
    auto t = x.AND(y).XOR(y.AND(x));

    cout << t << endl;
    */

    return 0;
}