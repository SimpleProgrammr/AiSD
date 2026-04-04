//
// Created by Elmo on 31/03/2026.
//
#include <cstring>
#include <iostream>
#include <random>

#include "insert_sort.h"
#include "selection_sort.h"
#include "bubble_sort.h"
#include "quick_sort.h"

using namespace std;


void print_array(const int *array, int length) {
    for (int i = 0; i < length; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}


void print_list(list<int> list) {
    ;
    for (auto it = list.begin(); it != list.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int *get_fully_random_array(int length) {
    auto array = new int[length];
    memset(array, 0, length * sizeof(int));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uni_rand(0, length-1);

    for (int i = 1; i <= length; i++) {
        int x = uni_rand(gen);

        while (array[x] > 0) {
            x = (x+1)%(length);
        }

        array[x] = i;
    }


    return array;
}

std::list<int> array_to_list(const int * array, unsigned int length) {
    list<int> list;
    for (unsigned int i = 0; i < length; i++) {
        list.push_back(array[i]);
    }

    return list;
}


int main() {
    int length = 20;

    int *array = get_fully_random_array(length);
    print_array(array, length);

    // creating copy to maintain consistent data for test
    int* temp_arr = new int[length];


    //Part I
    cout << "Insert sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(insert_sort(temp_arr, length), length);

    cout << "Selection sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(selection_sort(temp_arr, length), length);

    cout << "Bubble sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(bubble_sort(temp_arr, length), length);

    //Part II
    cout << "Quick sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(quick_sort(temp_arr,0, length-1) , length);

    cout << "Shell sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(shell_sort(temp_arr, length), length);

    cout << "Heap sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(heap_sort(temp_arr, length), length);

    //Part III
    list<int> list = array_to_list(array, length);

    cout << "Stalin sort: " << endl;
    print_list(stalin_sort(list));
}