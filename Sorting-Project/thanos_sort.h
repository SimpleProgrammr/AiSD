//
// Created by Michał on 04.04.2026.
//

#pragma once
#include <list>
#include <random>

inline void snap(std::list<int> &list, unsigned int size ) {
    int to_erase = size/2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uni_rand(0, 1);

    for (auto i = list.begin(); to_erase > 0 ; ) {
        if (uni_rand(gen) == 1 && *i != -1) {
            *i = -1;
            to_erase--;
        }
        i = i == list.end() ? list.begin() : ++i;
    }
    list.remove(-1);
}

//O(nlog(n))
inline std::list<int> thanos_sort(std::list<int> data) {

    unsigned int size = data.size();
    std::list<int>::iterator end = --data.end();
    for (auto i = data.begin(); i != end;) {

        if (size < 2)
            return data;
        int prev = *i;
        ++i;
        int now = *i;
        if (prev > now) {
            snap(data, size);
            size = size - size/2;
            i = data.begin();
        }
        end = --data.end();
    }

    return data;
}