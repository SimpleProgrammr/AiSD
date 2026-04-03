//
// Created by Michał on 03.04.2026.
//

#pragma once
#include <bits/move.h>

inline int * quick_sort(int * data, unsigned int length) {
    int pivot = data[length - 1];

    int i = 0, j = 0;
    while (j <= length - 2) {
        if (data[j] > pivot) {
            std::swap(data[++i], data[j++]);
        }
        else {
            j++;
        }
    }

    return data;

}
