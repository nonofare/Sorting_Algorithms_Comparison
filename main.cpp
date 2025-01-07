#include <iostream>
#include <random>
#include <chrono>
#include "SAC.h"

struct some_object {
    double field_1;
    char field_2;
};

std::string so_fun_str(some_object *so) {
    return "(" + std::to_string(so->field_1) + ", " + so->field_2 + ")";
}

bool so_cmp_lgreater(some_object *so1, some_object *so2) {
    if (so1->field_1 == so2->field_1) {
        return so1->field_2 > so2->field_2;
    }
    return so1->field_1 > so2->field_1;
}

bool so_cmp_equal(some_object *so1, some_object *so2) {
    if (so1->field_1 == so2->field_1 && so1->field_2 == so2->field_2) {
        return true;
    }
    return false;
}

size_t so_fun_key(some_object *so, size_t n) {
    return static_cast<size_t>(so->field_1 * n);
}

template<typename T>
std::string PrintArray(T *array, size_t n, size_t limit = 0, std::string (*cmp_string)(T) = nullptr) {
    if (limit == 0 || limit > n) {
        limit = n;
    }

    std::string text = "Array:\n";
    text += "size: " + std::to_string(static_cast<int>(n)) + "\n";
    text += "{\n";
    if (cmp_string) {
        for (int i = 0; i < limit; i++) {
            text += cmp_string(array[i]);
            text += "\n";
        }
    } else if constexpr (std::is_arithmetic_v<T>) {
        for (int i = 0; i < limit; i++) {
            text += std::to_string(array[i]);
            text += "\n";
        }
    } else {
        text = "T is not arithmetic and no cmp was provided\n";
    }

    if (limit < n) {
        text += "[...]\n";
    }

    text += "}\n";

    return text;
}

template<typename T>
bool CompareArrays(T *array1, T *array2, T *array3, size_t n, bool (*cmp_equal)(T, T) = nullptr) {
    if (cmp_equal) {
        for (size_t i = 0; i < n; i++) {
            if (!(cmp_equal(array1[i], array2[i]) && cmp_equal(array2[i], array3[i]))) {
                return false;
            }
        }
    } else if constexpr (std::is_arithmetic_v<T>) {
        for (size_t i = 0; i < n; i++) {
            if (!(array1[i] == array2[i] && array2[i] == array3[i])) {
                return false;
            }
        }
    } else {
        throw std::runtime_error("CompareArrays(): T was not arithmetic and no cmp was provided");
    }

    return true;
}

void TestForInts(const int MAX_ORDER, const int m, std::default_random_engine &dre) {
    std::uniform_int_distribution<int> rnd_num(0, m);

    for (int i = 1; i <= MAX_ORDER; i++) {
        std::cout << "==========================================================" << std::endl;
        std::cout << "Test: " << i << std::endl << std::endl;

        int n = static_cast<int>(pow(10, i));

        int *array1 = new int[n];
        int *array2 = new int[n];
        int *array3 = new int[n];

        for (int j = 0; j < n; j++) {
            array1[j] = rnd_num(dre);
        }
        memcpy(array2, array1, n * sizeof(int));
        memcpy(array3, array1, n * sizeof(int));

        std::cout << "Initial " << PrintArray(array1, n, 8) << std::endl;


        std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
        SAC::CountingSort(array1, n, m);
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> counting_sort_time = end_time - start_time;
        std::cout << "Counting " << PrintArray(array1, n, 8) << std::endl;


        auto *sbh = new SAC::SortingBinHeap<int>(array2, n, true);
        start_time = std::chrono::high_resolution_clock::now();
        sbh->Sort();
        end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> heap_sort_time = end_time - start_time;
        std::cout << "Heap " << PrintArray(array2, n, 8) << std::endl;


        start_time = std::chrono::high_resolution_clock::now();
        SAC::BucketSort(array3, n, m);
        end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> bucket_sort_time = end_time - start_time;
        std::cout << "Bucket " << PrintArray(array3, n, 8) << std::endl;


        double total_time = counting_sort_time.count() + heap_sort_time.count() + bucket_sort_time.count();
        std::cout << "-----------SUMMARY-----------" << std::endl;
        std::cout << "n             | " << n << std::endl;
        std::cout << "m             | " << m << std::endl;
        std::cout << "              | " << std::endl;
        std::cout << "Counting sort | " << counting_sort_time.count() << "s" << std::endl;
        std::cout << "Heap sort     | " << heap_sort_time.count() << "s" << std::endl;
        std::cout << "Bucket sort   | " << bucket_sort_time.count() << "s" << std::endl;
        std::cout << "              | " << std::endl;
        std::cout << "Total         | " << total_time << "s" << std::endl;
        std::cout << "Arrays equal  | " << (CompareArrays(array1, array2, array3, n) ? "yes" : "no") << std::endl;
        std::cout << "-----------------------------" << std::endl << std::endl;

        delete[] array1;
        delete[] array2;
        delete[] array3;
    }
}

void TestForObjects(const int MAX_ORDER, const int m, std::default_random_engine &dre) {
    constexpr int LETTERS_SIZE = 36;
    constexpr char LETTERS[LETTERS_SIZE] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
    };

    std::uniform_int_distribution<int> rnd_num(0, m);
    std::uniform_int_distribution<int> rnd_let(0, LETTERS_SIZE - 1);

    for (int i = 1; i <= MAX_ORDER; i++) {
        std::cout << "==========================================================" << std::endl;
        std::cout << "Test: " << i << std::endl << std::endl;

        int n = static_cast<int>(pow(10, i));

        some_object **array1 = new some_object *[n];
        some_object **array2 = new some_object *[n];

        for (int j = 0; j < n; j++) {
            some_object *so = new some_object();
            so->field_1 = static_cast<double>(rnd_num(dre)) / m;
            so->field_2 = LETTERS[rnd_let(dre)];
            array1[j] = so;
        }
        memcpy(array2, array1, n * sizeof(some_object *));

        std::cout << "Initial " << PrintArray(array1, n, 8, so_fun_str) << std::endl;


        auto *sbh = new SAC::SortingBinHeap<some_object *>(array1, n, true, so_cmp_lgreater);
        std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
        sbh->Sort(so_cmp_lgreater);
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> heap_sort_time = end_time - start_time;
        std::cout << "Heap " << PrintArray(array1, n, 8, so_fun_str) << std::endl;


        start_time = std::chrono::high_resolution_clock::now();
        SAC::BucketSort(array2, n, m, so_cmp_lgreater, so_fun_key);
        end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> bucket_sort_time = end_time - start_time;
        std::cout << "Bucket " << PrintArray(array2, n, 8, so_fun_str) << std::endl;


        double total_time = heap_sort_time.count() + bucket_sort_time.count();
        std::cout << "-----------SUMMARY-----------" << std::endl;
        std::cout << "n             | " << n << std::endl;
        std::cout << "m             | " << m << std::endl;
        std::cout << "              | " << std::endl;
        std::cout << "Heap sort     | " << heap_sort_time.count() << "s" << std::endl;
        std::cout << "Bucket sort   | " << bucket_sort_time.count() << "s" << std::endl;
        std::cout << "              | " << std::endl;
        std::cout << "Total         | " << total_time << "s" << std::endl;
        std::cout << "Arrays equal  | " << (CompareArrays(array1, array2, array2, n, so_cmp_equal) ? "yes" : "no") <<
                std::endl;
        std::cout << "-----------------------------" << std::endl << std::endl;

        for (size_t j = 0; j < n; j++) {
            delete array1[j];
        }
        delete[] array1;
        delete[] array2;
    }
}

int main() {
    constexpr int MAX_ORDER = 6;
    const int m = static_cast<int>(pow(10, 7));

    static std::random_device rd;
    static std::default_random_engine dre(rd());

    //TestForInts(MAX_ORDER, m, dre);
    TestForObjects(MAX_ORDER, m, dre);

    return 0;
}
