//
// Created by Michał on 04.04.2026.
//
#pragma once

#include <list>

//O(inf)
std::list<int> miracle_sort(std::list<int> data) {
    bool isSorted;
    do {
        isSorted = true;
        int prev = data.front();
        for (auto i = data.begin(); i != data.end(); ++i) {
            if (prev > *i) {
                isSorted = false;
                break;
            }
            prev = *i;
            ++i;
        }
    }while (!isSorted);

    return data;
}