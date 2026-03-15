
#include <iostream>
#include <ostream>
using namespace std;

int modulo(int a, int b) {
    if (a < b)
        return a;
    return modulo(a-b, b);
}

int main() {
    int a = 1012 , b =62;
    cout<< a % b << " : " << modulo(a, b) << endl;
}