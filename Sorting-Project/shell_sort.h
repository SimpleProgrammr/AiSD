//
// Created by Michał on 03.04.2026.
//
#pragma once
#include <bits/move.h>

inline int * shell_sort(int * data, unsigned int length) {

    for (int gap = length / 2; gap > 0; gap /= 2) {

        for (int i = gap; i < length; i++) {
        int key = data[i];
        int j = i;
        while (j >= gap && data[j-gap] > key) {
            data[j] = data[j-gap] ;
            j-=gap;
        }
        data[j] = key;
    }


    }



    return data;
}
