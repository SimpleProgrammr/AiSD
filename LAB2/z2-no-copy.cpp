
#include <iostream>
#include <string>
using namespace std;

bool is_clear(string str) {
    if (str.empty())
        return true;
    if (str[0] == str[1]) {
        return false;
    }
    else {
        return is_clear(&str[1]);
    }
}

string remove_char(string s, char c) {
    if (s.empty() || s[0] != c) {
        return s;
    }
    else return remove_char(&s[1], c);
}

string remove_copies(string str) {
    if (str.empty())
        return "";

    static string ss = "";
    if (str[0] == str[1]) {
        string tmp = remove_char(&str[0], str[0]);
        tmp = ss + tmp;
        ss = "";
        string r = remove_copies(tmp);
        return r;
    }
    else {
        ss += str[0];
        string r = ss+ remove_copies(&str[1]);
        ss = "";
        return r;
    }

}

string remove_copies_runner(string s){

    string cleared = remove_copies(s);

    if (is_clear(cleared))
        return cleared;
    else
        return remove_copies_runner(cleared);
}

int main() {
    string s = "ABBBBCCCDA";
    cout << s << " : " << remove_copies(s);
}
