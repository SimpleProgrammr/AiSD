//
// Created by Michał on 03.04.2026.
//
#pragma once
#include <filesystem>

inline int* selection_sort(int* data, unsigned int length) {
    for (int i = 0; i < length; i++) {
        int min_index = i;

        for (int j = i + 1; j < length; j++) {
            if (data[j] < data[i]) {
                min_index = j;
            }
        }

        std::swap(data[i], data[min_index]);
    }

    return data;
}
