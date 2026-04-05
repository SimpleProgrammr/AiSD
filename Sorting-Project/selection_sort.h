//
// Created by Michał on 03.04.2026.
//

#pragma once
#include <filesystem>

//O(n^2)
inline int* selection_sort(int* data, unsigned int length) {
    for (int i = 0; i < length; i++) {
        int min_index = i;

        for (int j = i; j < length; j++) {
            if (data[min_index] > data[j]) {
                min_index = j;
            }
        }

        std::swap(data[i], data[min_index]);
    }

    return data;
}
