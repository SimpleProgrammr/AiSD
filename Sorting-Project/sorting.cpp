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


void print_array(int *array, int lenght) {
    for (int i = 0; i < lenght; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

int *get_fully_random_array(int lenght) {
    auto array = new int[lenght];
    memset(array, 0, lenght * sizeof(int));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> urand(0, lenght-1);

    for (int i = 1; i <= lenght; i++) {
        int x = urand(gen);

        while (array[x] > 0) {
            x = (x+1)%(lenght);
        }

        array[x] = i;
    }


    return array;
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
    print_array(insert_sort(array, length), length);

    cout << "Selection sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(selection_sort(array, length), length);

    cout << "Bubble sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(bubble_sort(array, length), length);

    //Part II
    cout << "Quick sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(quick_sort(array, length), length);
}