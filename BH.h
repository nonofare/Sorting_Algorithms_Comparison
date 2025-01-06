#ifndef BH_H
#define BH_H
#include <string>
#include "DA.h"

namespace BH {
    template<typename T>
    class BinHeap : public DA::DynArr<T> {
    protected:
        size_t NodeParentIndex(size_t index) const {
            return (index - 1) / 2;
        }

        size_t NodeLeftIndex(size_t index) const {
            return (2 * index + 1);
        }

        size_t NodeRightIndex(size_t index) const {
            return (2 * index + 2);
        }

        void HeapifyUp(size_t index, bool (*cmp_lgreater)(T, T) = nullptr) {
            if (index >= this->Size()) {
                throw std::length_error(
                    "BH::HeapifyUp(): index (" + std::to_string(index) + ") was greater or egual to heap size (" +
                    std::to_string(int(this->Size())) + ")");
            }

            if (index) {
                size_t parent = NodeParentIndex(index);

                try {
                    if (cmp_lgreater) {
                        if (cmp_lgreater((*this)[index], (*this)[parent])) {
                            Swap(index, parent);
                            HeapifyUp(parent, cmp_lgreater);
                        }
                    } else if constexpr (std::is_arithmetic_v<T>) {
                        if ((*this)[index] > (*this)[parent]) {
                            Swap(index, parent);
                            HeapifyUp(parent, cmp_lgreater);
                        }
                    } else {
                        throw std::runtime_error("T was not arithmetic and no cmp was provided");
                    }
                } catch (const std::exception &ex) {
                    throw std::runtime_error("BH::HeapifyUp() -> " + std::string(ex.what()));
                }
            }
        }

        void HeapifyDown(size_t index, bool (*cmp_lgreater)(T, T) = nullptr) {
            if (index >= this->Size()) {
                throw std::length_error(
                    "BH::HeapifyDown(): index (" + std::to_string(index) + ") was greater or egual to heap size (" +
                    std::to_string(int(this->Size())) + ")");
            }

            size_t left = NodeLeftIndex(index);
            size_t right = NodeRightIndex(index);
            size_t largest = index;

            try {
                if (cmp_lgreater) {
                    if (left < this->Size() && cmp_lgreater((*this)[left], (*this)[largest])) {
                        largest = left;
                    }
                    if (right < this->Size() && cmp_lgreater((*this)[right], (*this)[largest])) {
                        largest = right;
                    }
                } else if constexpr (std::is_arithmetic_v<T>) {
                    if (left < this->Size() && (*this)[left] > (*this)[largest]) {
                        largest = left;
                    }
                    if (right < this->Size() && (*this)[right] > (*this)[largest]) {
                        largest = right;
                    }
                } else {
                    throw std::runtime_error("T was not arithmetic and no cmp was provided");
                }

                if (largest != index) {
                    Swap(index, largest);
                    HeapifyDown(largest, cmp_lgreater);
                }
            } catch (const std::exception &ex) {
                throw std::runtime_error("BH::HeapifyDown() -> " + std::string(ex.what()));
            }
        }

        void Swap(size_t index1, size_t index2) {
            if (index1 != index2) {
                T temp = (*this)[index1];

                (*this)[index1] = (*this)[index2];
                (*this)[index2] = temp;
            }
        }

    public:
        BinHeap(size_t in_capacity = 1) : DA::DynArr<T>(in_capacity) {
        }

        ~BinHeap() {
            Erase();
        }

        size_t Size() const {
            return this->size;
        }

        size_t Capacity() const {
            return this->capacity;
        }

        void Push(T data, bool (*cmp_lgreater)(T, T) = nullptr) {
            try {
                this->Push(data);
                HeapifyUp(this->Size() - 1, cmp_lgreater);
            } catch (const std::exception &ex) {
                throw std::runtime_error("BH::Push() -> " + std::string(ex.what()));
            }
        }

        void Pop(size_t index = 0, bool (*cmp_lgreater)(T, T) = nullptr) {
            if (index >= this->Size()) {
                throw std::length_error(
                    "BH::Pop(): index (" + std::to_string(index) + ") was greater or equal to this->size (" +
                    std::to_string(int(this->Size())) + ")");
            }

            try {
                Swap(index, this->Size() - 1);
                this->Pop(this->Size() - 1);
                if (this->Size()) {
                    HeapifyDown(index, cmp_lgreater);
                }
            } catch (const std::exception &ex) {
                throw std::runtime_error("BH::Pop() -> " + std::string(ex.what()));
            }
        }

        T Poll(bool (*cmp_greater)(T, T) = nullptr) {
            if (!this->Size()) { throw std::length_error("BH::Poll(): heap was empty"); }

            T root = (*this)[0];
            try {
                Pop(0, cmp_greater);
            } catch (const std::exception &ex) {
                throw std::runtime_error("BH::Poll() -> " + std::string(ex.what()));
            }

            return root;
        }

        void Erase() {
            try {
                this->Erase();
            } catch (const std::exception &ex) {
                throw std::runtime_error("BH::Erase() -> " + std::string(ex.what()));
            }
        }

        std::string ToString(unsigned int limit = 0, std::string (*cmp_string)(T) = nullptr) const {
            std::string text = ">>> Binary Heap <<<\n";
            text += "is based on\n";

            text += this->ToString(limit, cmp_string);

            return text;
        }
    };
}

#endif
