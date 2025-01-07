#ifndef SAC_H
#define SAC_H
#include "BH.h"
#include <vector>

namespace SAC {
    template<typename T>
    class SortingBinHeap : public BH::BinHeap<T> {
    public:
        SortingBinHeap(T *arr, size_t n, bool top_down = false, bool (*cmp_lgreater)(T, T) = nullptr) : BH::BinHeap<T>(
            n) {
            this->arr = arr;
            this->size = n;
            this->capacity = n + 1;

            if (top_down) {
                for (size_t i = this->NodeParentIndex(n - 1); i != -1; i--) {
                    this->HeapifyDown(i, cmp_lgreater);
                }
            } else {
                for (size_t i = 0; i < this->Size(); i++) {
                    this->HeapifyUp(i, cmp_lgreater);
                }
            }
        }

        void Sort(bool (*cmp_lgreater)(T, T) = nullptr) {
            size_t temp_size = this->Size();
            for (size_t i = this->size - 1; i > 0; i--) {
                this->Swap(0, i);
                this->size--;
                this->HeapifyDown(0, cmp_lgreater);
            }
            this->size = temp_size;
        }
    };

    void CountingSort(int *arr, size_t n, int m) {
        int *cnt_arr = new int[m + 1]();
        int *out_arr = new int[n];

        for (size_t i = 0; i < n; i++) {
            cnt_arr[arr[i]]++;
        }

        for (size_t i = 1; i <= m; i++) {
            cnt_arr[i] += cnt_arr[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            out_arr[cnt_arr[arr[i]] - 1] = arr[i];
            cnt_arr[arr[i]]--;
        }

        memcpy(arr, out_arr, n * sizeof(int));

        delete[] cnt_arr;
        delete[] out_arr;
    }

    template<typename T>
    void InsertionSort(std::vector<T> &arr, bool (*cmp_lgreater)(T, T) = nullptr) {
        if (cmp_lgreater) {
            for (size_t i = 1; i < arr.size(); i++) {
                T key = arr[i];
                int j = i - 1;
                while (j >= 0 && cmp_lgreater(arr[j], key)) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
        } else if constexpr (std::is_arithmetic_v<T>) {
            for (size_t i = 1; i < arr.size(); i++) {
                T key = arr[i];
                int j = i - 1;
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
        } else {
            throw std::runtime_error("SAC::InsertionSort(): T was not arithmetic and no cmp was provided");
        }
    }

    void BucketSort(int *arr, size_t n, int m) {
        auto *buckets = new std::vector<int>[n];

        for (size_t i = 0; i < n; i++) {
            size_t bucket_index = (arr[i] * n) / m;
            buckets[bucket_index].push_back(arr[i]);
        }

        size_t index = 0;
        for (size_t i = 0; i < n; i++) {
            if (!buckets[i].empty()) {
                InsertionSort(buckets[i]);
                for (size_t j = 0; j < buckets[i].size(); j++) {
                    arr[index++] = buckets[i][j];
                }
            }
        }

        delete[] buckets;
    }

    template<typename T>
    void BucketSort(T *arr, size_t n, int m, bool (*cmp_lgreater)(T, T), size_t (*fun_key)(T, size_t)) {
        auto *buckets = new std::vector<T>[n];

        for (size_t i = 0; i < n; i++) {
            size_t bucket_index = fun_key(arr[i], n);
            buckets[bucket_index].push_back(arr[i]);
        }

        size_t index = 0;
        for (size_t i = 0; i < n; i++) {
            if (!buckets[i].empty()) {
                InsertionSort(buckets[i], cmp_lgreater);
                for (size_t j = 0; j < buckets[i].size(); j++) {
                    arr[index++] = buckets[i][j];
                }
            }
        }

        delete[] buckets;
    }
}

#endif
