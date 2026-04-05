//
// Created by Michał on 04.04.2026.
//
#pragma once
#include <list>

//O(n)
inline std::list<int> stalin_sort(std::list<int> list) {
    auto prev = list.begin();
    auto present = ++list.begin();
    while (present != list.end()) {
        if (*prev > *present) {
            auto temp = present;

            ++present;
            list.erase(temp);
        }
        else {
            ++present;
            ++prev;
        }
    }
    return list;
}