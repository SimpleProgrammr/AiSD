//
// Created by Michał on 03.04.2026.
//

#pragma once
#include <bits/move.h>

int pivot(int* data, int low, int high) {
    int piv = data[high];

    int i = low-1;
    for (int j = low; j <= high-1; j++) {
        if (data[j] < piv) {
            std::swap(data[++i], data[j]);
        }
    }
    std::swap(data[++i], data[high]);
    return i;
}

inline int * quick_sort(int * data, int low, int high) {
    if (low < high) {
        int pi = pivot(data, low, high);

        quick_sort(data, low, pi - 1);
        quick_sort(data, pi + 1, high);

    }
    return data;
}
