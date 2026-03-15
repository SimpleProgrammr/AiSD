
#include <iostream>
#include <string>
using namespace std;

string compress_it(string raw) {
    string compressed = "";

    if (raw.empty()) {
        return  compressed;
    }

    char last_ch = raw[0];
    int mod_count = 1;

    for (int i = 1; i < raw.length(); i++) {
        if (last_ch != raw[i]) {
            compressed += to_string(mod_count) + last_ch;
            mod_count = 1;
            last_ch = raw[i];
        }
        else
            mod_count++;
    }
    compressed += to_string(mod_count) + last_ch;

    return compressed;
}

string compress_rek(string raw) {
    if (raw.empty()) {
        return "";
    }
    int mod_count = 1;
    char last_ch = raw[0];
    for (int i = 1; i < raw.length(); i++) {
        if (last_ch != raw[i]) {
            return to_string(mod_count) + last_ch + compress_it(&raw[i]);
        }
        else
            mod_count++;
    }
    return to_string(mod_count) + last_ch;
}

string decompress_it(string comp) {
    if (comp.empty())
        return "";
    string raw = "";
    int char_repeats = 0;
    for (int i = 0; i < comp.length(); i++) {
        if (std::isdigit(comp[i])) {
            char_repeats*=10;
            char_repeats += atoi(&comp[i]);
        }
        else {
            for (int j = 0; j < char_repeats; j++)
                raw += comp[i];
            char_repeats = 0;
        }
    }
    return raw;
}

string decompress_rek(string comp) {
    static string raw = "";
    if (comp.empty())
        return raw;

    static int char_repeats = 0;

    if (isdigit(comp[0])) {
        char_repeats*=10;
        char_repeats += atoi(&comp[0]);
        return decompress_rek(&comp[1]);
    }
    else {
        for (int j = 0; j < char_repeats; j++)
            raw += comp[0];
        char_repeats = 0;
        return decompress_rek(&comp[1]);
    }
}

int main() {
    string test_txt = "AAAABBCBBBCCCCC";
    string comp = compress_it(test_txt);
    cout << test_txt << " : " << comp  << endl;
    cout << comp << " : " << decompress_it(comp) << endl << endl;

    comp = compress_rek(test_txt);
    cout << test_txt << " : " << comp  << endl;
    cout << comp << " : " << decompress_rek(comp) << endl << endl;
}