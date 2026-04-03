//
// Created by Elmo on 31/03/2026.
//
#include <cstring>
#include <iostream>
#include <random>

#include "insert_sort.h"

using namespace std;

void print_array(int *array, int lenght) {
    for (int i = 0; i < lenght; i++) {
        cout << array[i] << " ";
    }
}

int *get_fully_random_array(int lenght) {
    auto array = new int[lenght];
    memset(array, 0, lenght * sizeof(int));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> urand(0, lenght-1);

    for (int i = 1; i <= lenght; i++) {
        int x = urand(gen);
        cout << x << ": ";
        while (array[x] > 0) {
            x = (x+1)%(lenght);
        }

        array[x] = i;

        print_array(array, lenght);
        cout << " : " << x << endl;
    }


    return array;
}


int main() {
    int lenght = 20;

    int *array = get_fully_random_array(lenght);
    print_array(array, lenght);
    // creating copy to maintain consistent data for test
    int* temp_arr = new int[lenght];

}