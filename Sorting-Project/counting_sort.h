//
// Created by Michał on 06.04.2026.
//

#pragma once
#include <algorithm>
#include <cstring>

//O(n)
inline int * counting_sort_v1(int * arr, unsigned int length) {
    int max_value = arr[0];
    for (int i = 1; i < length; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    int * cnt_array = new int[max_value + 1];
    memset(cnt_array, 0, (max_value + 1) * sizeof(int));

    for (int i = 0; i < length; i++) {
        cnt_array[arr[i]]++;
    }

    for (int i = 1; i <= max_value; i++)
            cnt_array[i] += cnt_array[i - 1];

    int * ans = new int[length];
    for (int i = length - 1; i >= 0; i--) {
        ans[cnt_array[arr[i]] - 1] = arr[i];
        cnt_array[arr[i]]--;
    }
    std::copy(ans, ans+length, arr);

    delete [] ans;
    delete [] cnt_array;
    return arr;
}

//O(n)
inline int * counting_sort_v2(int * arr, unsigned int length) {
    int max_value = arr[0];
    for (int i = 1; i < length; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    int * cnt_array = new int[max_value + 1];
    memset(cnt_array, 0, (max_value + 1) * sizeof(int));

    for (int i = 0; i < length; i++) {
        cnt_array[arr[i]]++;
    }

    int v = 0;
    for (int i = 0; i < length; i++) {
        while (--cnt_array[v] < 0) {
            v++;
        }
        arr[i] = v;
    }

    delete [] cnt_array;
    return arr;
}
