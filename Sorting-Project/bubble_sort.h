//
// Created by Michał on 03.04.2026.
//

#pragma once

inline int * bubble_sort(int * data, unsigned int length) {
    bool sorted;

    do {
        sorted = true;
        for (int i = 0; i < length; i++) {
            if (data[i] > data[i + 1]) {
                sorted = false;
                std::swap(data[i], data[i + 1]);
            }
        }
    }while(!sorted);

    return data;
}