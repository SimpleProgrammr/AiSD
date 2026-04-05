//
// Created by Elmo on 31/03/2026.
//

#include <cstring>
#include <iostream>
#include <list>
#include <random>

#include "insert_sort.h"
#include "selection_sort.h"
#include "bubble_sort.h"
#include "quick_sort.h"
#include "shell_sort.h"
#include "heap_sort.h"
#include "stalin_sort.h"
#include "thanos_sort.h"
#include "miracle_sort.h"
#include "luck_sort.h"

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


unsigned long long speedtest(int* data, unsigned int len, int *algorithm(int*,unsigned int)) {
    int *tmp_data = new int[len];
    std::copy(data, data+len, tmp_data);

    auto start_time = std::chrono::high_resolution_clock::now();

    algorithm(tmp_data, len);

    auto end_time = std::chrono::high_resolution_clock::now();
    free(tmp_data);

    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

unsigned long long speedtest(int* data, int len, int *algorithm(int*, int, int)) {
    int *tmp_data = new int[len];
    std::copy(data, data+len, tmp_data);

    auto start_time = std::chrono::high_resolution_clock::now();

    algorithm(tmp_data,0, len);

    auto end_time = std::chrono::high_resolution_clock::now();
    free(tmp_data);

    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

unsigned long long speedtest(list<int> data, int *algorithm(list<int>)) {

    auto start_time = std::chrono::high_resolution_clock::now();

    algorithm(data);

    auto end_time = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

void test_run() {
    int length = 5;

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
    list<int> data = array_to_list(array, length);

    cout << "Stalin sort: " << endl;
    print_list(stalin_sort(data));

    cout << "Thanos sort: " << endl;
    print_list(thanos_sort(data));

    cout << "Luck sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(luck_sort(temp_arr, length), length);
}

int main() {


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

    cout << "Thanos sort: " << endl;
    for (int i = 1; i <= 100; i++)
        print_list(thanos_sort(list));

    cout << "Luck sort: " << endl;
    int * tmp = new int[5] {1,3,2,4,5};
    print_array(luck_sort(tmp, 5), 5);

}