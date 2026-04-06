//
// Created by Elmo on 31/03/2026.
//

#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <thread>
#include <utility>

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

int * get_descending_array(int length) {
    auto array = new int[length];
    memset(array, 0, length * sizeof(int));
    for (int i = 1; i <= length; i++) {
        array[i-1] = length - i;
    }
    return array;
}

int * get_ascending_array(int length) {
    auto array = new int[length];
    memset(array, 0, length * sizeof(int));
    for (int i = 1; i <= length; i++) {
        array[i-1] = i;
    }
    return array;
}

int * get_neighbours_flipped(int length) {
    int * arr = get_ascending_array(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uni_rand(0, length-1-1);

    int to_flip = length/10/2;

    while (to_flip>0) {
        const int x = uni_rand(gen);
        std::swap(arr[x], arr[x+1]);
        to_flip--;
    }

    return arr;
}

int * get_little_scrambled_array(int length) {
    int * arr = get_ascending_array(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uni_rand(0, length-1);

    int to_flip = length/10/2;

    while (to_flip>0) {
        int x = uni_rand(gen);
        int y = uni_rand(gen);
        std::swap(arr[x], arr[y]);
        to_flip--;
    }

    return arr;
}

std::list<int> array_to_list(const int * array, unsigned int length) {
    list<int> list;
    for (unsigned int i = 0; i < length; i++) {
        list.push_back(array[i]);
    }

    return list;
}

//Speed testers
long long speedtest(int* data, unsigned int len, int *algorithm(int*,unsigned int)) {
    int *tmp_data = new int[len];
    long long duration = 0;

    int repeats = 3;
    for (unsigned int i = 0; i < repeats; i++) {
        std::copy(data, data+len, tmp_data);

        auto start_time = std::chrono::high_resolution_clock::now();

        algorithm(tmp_data, len);

        auto end_time = std::chrono::high_resolution_clock::now();

        duration += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    }

    delete [] tmp_data;

    return duration/repeats;
}

long long speedtest(int* data, int len, int *algorithm(int*, int, int)) {
    int *tmp_data = new int[len];
    long long duration = 0;

    int repeats = 3;
    for (unsigned int i = 0; i < repeats; i++) {
        std::copy(data, data+len, tmp_data);

        auto start_time = std::chrono::high_resolution_clock::now();

        algorithm(tmp_data, 0, len-1);

        auto end_time = std::chrono::high_resolution_clock::now();

        duration += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    }
    delete [] tmp_data;
    return duration/repeats;
}

long long speedtest(list<int> data, list<int> algorithm(list<int>)) {
    long long duration = 0;
    int repeats = 3;

    for (unsigned int i = 0; i < repeats; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();

        algorithm(std::move(data));

        auto end_time = std::chrono::high_resolution_clock::now();

        duration += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    }
    return duration;
}


// Run wrappers
constexpr int START = 1000, END = 50000;
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

//Scenario I: Fully scrambled data
void scenario_1_run() {
    const auto processor_count = std::thread::hardware_concurrency();

    cout << "Counting sort: " << endl;
    std::copy(array, array+length, temp_arr);
    print_array(counting_sort_v2(temp_arr, length), length);
}

    fout << "Data_size\tInsert_sort\tSelection_sort\tBubble_sort\tQuick_sort\tShell_sort\tHeap_sort\tStalin_sort\tThanos_sort" << endl;

    for (int length = START; length <= END;) {
        cout << length << " / " << END << " (" << (static_cast<double>(length) / END * 100) << "%)" << "\n";

        int* array = get_fully_random_array(length);

        long long ins = 0, sel = 0, bub = 0, qui = 0, she = 0, hea = 0, sta = 0, tha = 0;

#pragma omp parallel num_threads(processor_count-1)
        {
#pragma omp sections
            {
                //Part I
#pragma omp section
                {
                    ins = speedtest(array, length, &insert_sort);
                    cout << "Insert sort, " ;
                }
#pragma omp section
                {
                    sel = speedtest(array, length, &selection_sort);
                    cout << "Selection sort, " ;
                }
#pragma omp section
                {
                    if (length <= END/2)
                        bub = speedtest(array, length, &bubble_sort);
                    else bub = -1;
                    cout << "Bubble sort, " ;
                }
                //Part II
#pragma omp section
                {
                    qui = speedtest(array, length, &quick_sort);
                    cout << "Quick sort, " ;
                }
#pragma omp section
                {
                    she = speedtest(array, length, &shell_sort);
                    cout << "Shell sort, " ;
                }
#pragma omp section
                {
                    hea = speedtest(array, length, &heap_sort);
                    cout << "Heap sort, " ;
                }
                //Part III
#pragma omp section
                {
                    list<int> data = array_to_list(array, length);
                    sta = speedtest(data, &stalin_sort);
                    cout << "Stalin sort, " ;
                    tha = speedtest(data, &thanos_sort);
                    cout << "Thanos sort, " ;

                }
            }
        }
        cout << endl;

        fout << length << "\t"
            << ins << "\t"
            << sel << "\t";
        if (bub != -1)
            fout<< bub;
        fout << "\t";
        fout << qui << "\t"
            << she << "\t"
            << hea << "\t"
            << sta << "\t"
            << tha << "\t"
            << endl;

        delete[] array;
        //length += static_cast<int>(pow(10, static_cast<int>(log10(length))));
        length += 1000;
    }

    fout.close();
}

//Scenario II: Descending sorted data
void scenario_2_run() {
    const auto processor_count = std::thread::hardware_concurrency();
    ofstream fout("Scenario2-desc_order.txt");
    if (!fout.is_open()) {
        cerr << "Unable to open file(Scenario 2)" << endl;
        throw std::ios_base::failure("Unable to open file(Scenario 2)");
    }

    fout << "Data_size\tInsert_sort\tSelection_sort\tBubble_sort\tQuick_sort\tShell_sort\tHeap_sort\tStalin_sort\tThanos_sort" << endl;

    for (int length = START; length <= END;) {
        cout << length << " / " << END << " (" << (static_cast<double>(length) / END * 100) << "%)" << "\n";

        int* array = get_descending_array(length);

        long long ins = 0, sel = 0, bub = 0, qui = 0, she = 0, hea = 0, sta = 0, tha = 0;

#pragma omp parallel num_threads(processor_count-1)
        {
#pragma omp sections
            {
                //Part I
#pragma omp section
                {
                    ins = speedtest(array, length, &insert_sort);
                    cout << "Insert sort, " ;
                }
#pragma omp section
                {
                    sel = speedtest(array, length, &selection_sort);
                    cout << "Selection sort, " ;
                }
#pragma omp section
                {
                    if (length <= END/2)
                        bub = speedtest(array, length, &bubble_sort);
                    else bub = -1;
                    cout << "Bubble sort, " ;
                }
                //Part II
#pragma omp section
                {
                    //limiting quicksort due to stack overflow
                    if (length <= END/2)
                        qui = speedtest(array, length, &quick_sort);
                    else qui = -1;

                    cout << "Quick sort, " ;
                }
#pragma omp section
                {
                    she = speedtest(array, length, &shell_sort);
                    cout << "Shell sort, " ;
                }
#pragma omp section
                {
                    hea = speedtest(array, length, &heap_sort);
                    cout << "Heap sort, " ;
                }
                //Part III
#pragma omp section
                {
                    list<int> data = array_to_list(array, length);
                    sta = speedtest(data, &stalin_sort);
                    cout << "Stalin sort, " ;
                    tha = speedtest(data, &thanos_sort);
                    cout << "Thanos sort, " ;

                }
            }
        }
        cout << endl;

        fout << length << "\t"
            << ins << "\t"
            << sel << "\t";
        if (bub != -1)
            fout<< bub;
        fout << "\t";
        if (qui != -1)
            fout<< qui;
        fout << "\t"
            << she << "\t"
            << hea << "\t"
            << sta << "\t"
            << tha << "\t"
            << endl;

        delete[] array;
        //length += static_cast<int>(pow(10, static_cast<int>(log10(length))));
        length += 1000;
    }

    fout.close();
}

//Scenario III: Ascending sorted data
void scenario_3_run() {
    const auto processor_count = std::thread::hardware_concurrency();
    ofstream fout("Scenario3-desc_order.txt");
    if (!fout.is_open()) {
        cerr << "Unable to open file(Scenario 3)" << endl;
        throw std::ios_base::failure("Unable to open file(Scenario 3)");
    }

    fout << "Data_size\tInsert_sort\tSelection_sort\tBubble_sort\tQuick_sort\tShell_sort\tHeap_sort\tStalin_sort\tThanos_sort" << endl;

    for (int length = START; length <= END;) {
        cout << length << " / " << END << " (" << (static_cast<double>(length) / END * 100) << "%)" << "\n";

        int* array = get_ascending_array(length);

        long long ins = 0, sel = 0, bub = 0, qui = 0, she = 0, hea = 0, sta = 0, tha = 0;

#pragma omp parallel num_threads(processor_count-1)
        {
#pragma omp sections
            {
                //Part I
#pragma omp section
                {
                    ins = speedtest(array, length, &insert_sort);
                    cout << "Insert sort, " ;
                }
#pragma omp section
                {
                    sel = speedtest(array, length, &selection_sort);
                    cout << "Selection sort, " ;
                }
#pragma omp section
                {
                    if (length <= END/2)
                        bub = speedtest(array, length, &bubble_sort);
                    else bub = -1;
                    cout << "Bubble sort, " ;
                }
                //Part II
#pragma omp section
                {
                    qui = speedtest(array, length, &quick_sort);
                    cout << "Quick sort, " ;
                }
#pragma omp section
                {
                    she = speedtest(array, length, &shell_sort);
                    cout << "Shell sort, " ;
                }
#pragma omp section
                {
                    hea = speedtest(array, length, &heap_sort);
                    cout << "Heap sort, " ;
                }
                //Part III
#pragma omp section
                {
                    list<int> data = array_to_list(array, length);
                    sta = speedtest(data, &stalin_sort);
                    cout << "Stalin sort, " ;
                    tha = speedtest(data, &thanos_sort);
                    cout << "Thanos sort, " ;

                }
            }
        }
        cout << endl;

        fout << length << "\t"
            << ins << "\t"
            << sel << "\t";
        if (bub != -1)
            fout<< bub;
        fout << "\t";
        fout << qui << "\t"
            << she << "\t"
            << hea << "\t"
            << sta << "\t"
            << tha << "\t"
            << endl;

        delete[] array;
        //length += static_cast<int>(pow(10, static_cast<int>(log10(length))));
        length += 1000;
    }

    fout.close();
}

//Scenario IV: 10% of data flipped with neighbor, rest is sorted
void scenario_4_run();

//Scenario V: 10% is randomized, rest is sorted
void scenario_5_run();

void full_run() {
    const auto processor_count = std::thread::hardware_concurrency();
    int START = 1000, END = 100000;

    // Speed test part
    //////////////////////////////////////////////////////////////////////////////////////
    scenario_1_run();

    /////////////////////////////////////////////////////////////////////////////////////

    scenario_2_run();

    ///////////////////////////////////////////////////////////////////////////////////////

    scenario_3_run();

    //////////////////////////////////////////////////////////////////////////////////////

    scenario_4_run();

    //////////////////////////////////////////////////////////////////////////////////////

    scenario_5_run();
}

//function to test optimization of counting_sort versions
void test_counting() {

    long count = 0;
    long double avg1 = 0, avg2 = 0;
    for (int i = START; i <= END*2; i+=100) {
        int * array1 = get_fully_random_array(i);
        int * array2 = new int[i];
        std::copy(array1, array1 + i, array2);

        long long t1 = speedtest(array1, i, &counting_sort_v1), t2 = speedtest(array2, i, &counting_sort_v2);
        avg1 += t1;
        avg2 += t2;
        count ++;
        cout << t1 << "\t" << t2 << endl;
        delete [] array1;
        delete [] array2;
    }
    cout << "Avg: " << "\t" << avg1/count << "\t" << avg2/count << endl;
}

int main() {
    full_run();

    return 0;
}