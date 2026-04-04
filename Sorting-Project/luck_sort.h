//
// Created by Michał on 04.04.2026.
//

#pragma once

inline int * shuffle_list(int * data, unsigned int length) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uni_rand(0, length-1);

    for (auto i = 0; i < length; i++) {
        int x = uni_rand(gen);
        std::swap(data[i], data[x]);
    }
    return data;
}

inline bool isSorted(int* data, unsigned int length) {

    for (int i = 1; i < length; i++) {
        if (data[i-1] > data[i]) {
            return false;
        }
    }

    return true;
}

//O(n!)
inline int * luck_sort(int* data, unsigned int length) {

    do {
        shuffle_list(data, length);
    }while (!isSorted(data, length));

    return data;
}
