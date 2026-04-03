#pragma once

inline int* insert_sort(int* data, const unsigned int length) {
    for (int i = 1; i < length; i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j+1] = data[j] ;
            j--;
        }
        data[j+1] = key;
    }

    return data;
}

