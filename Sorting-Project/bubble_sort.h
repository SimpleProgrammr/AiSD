//
// Created by Michał on 03.04.2026.
//

#pragma once

inline int * bubble_sort(int * data, unsigned int length) {
    bool sorted = true;
    int i = 0;

    do {
        if (data[i] > data[i + 1]) {
            sorted = false;
            std::swap(data[i], data[i + 1]);
        }
        i++;
    }while(!sorted && i < length-1);

    return data;
}