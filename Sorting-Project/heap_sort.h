//
// Created by Michał on 04.04.2026.
//

#pragma once
#include <bits/move.h>

inline int * heapify(int * data, unsigned int length, unsigned int index_largest) {

    // Initialize largest as root
    int largest = index_largest;

    // left index = 2*index_largest + 1
    int left = 2 * index_largest + 1;

    // right index = 2*index_largest + 2
    int right = 2 * index_largest + 2;

    // If left child is larger than root
    if (left < length && data[left] > data[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < length && data[right] > data[largest])
        largest = right;

    // If largest is not root
    if (largest != index_largest) {
        std::swap(data[index_largest], data[largest]);

        // Recursively heapify the affected sub-tree
        heapify(data, length, largest);
    }

    return data;
}

inline int * heap_sort(int* data, unsigned int length) {

    for (int i = length / 2 - 1; i >= 0; i--)
        heapify(data, length, i);

    // One by one extract an element from heap
    for (int i = length - 1; i > 0; i--) {

        // Move current root to end
        std::swap(data[0], data[i]);

        // Call max heapify on the reduced heap
        heapify(data, i, 0);
    }

    return data;
}
