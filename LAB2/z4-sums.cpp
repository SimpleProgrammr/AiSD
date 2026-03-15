
#include <iostream>
using namespace std;

int find_divisions(int n, int k) {
    if (n == 0)
        return 1;
    int count = 0;
    for (int i = 1; i <= k; i++) {
        if (n>=i)
            count+= find_divisions(n-i, i);
    }

    return count;
}

int division(int n) {
    return find_divisions(n,n+1);
}

int main () {
    cout << division(6) << endl;
}