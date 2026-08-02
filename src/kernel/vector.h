#pragma once

#include "memoryallocator.h"
#include "definitions.h"

template <typename T>
class Vector {
    private:
        T* data = nullptr;
        size_t size = 0;
        size_t capacity = 0;
        
        void reallocate(size_t new_capacity) {
            T* new_data = new T[new_capacity];

            for (size_t i = 0; i < size; ++i) {
                new_data[i] = data[i];
            }

            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }   

    public:
        Vector() = default;

        ~Vector() {
            delete[] data;
        }

        Vector(const Vector& other) {
            size = other.size;
            capacity = other.capacity;
            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i=0;i<size;++i) {
                    data[i] = other.data[i];
                }
            }
        }
        Vector& operator=(const Vector& other) {
            if (this != &other) {
                delete[] data;

                size = other.size;
                capacity = other.capacity;

                if (capacity > 0) {
                    data = new T[capacity];
                    for (size_t i=0;i<size;++i) {
                        data[i] = other.data[i];
                    }
                } else {
                    data = nullptr;
                }
            }

            return *this;
        }

        bool operator==(const Vector& other) {
            if (size != other.size) return false;

            for (size_t i=0;i<size;i++) {
                if (data[i] != other.data[i]) return false;
            }

            return true;
        }

        void push_back(const T& value) {
            if (size >= capacity) {
                size_t new_capacity = (size == 0) ? 1 : capacity * 2;
                reallocate(new_capacity);
            }

            data[size] = value;
            size++;
        }
        void pop_back() {
            if (size > 0) {
                size--;
            }
        }

        T& operator[](size_t index) {
            return data[index];
        }
        const T& operator[](size_t index) const {
            return data[index];
        }
        T& at(size_t index) {
            if (index >= size) {
                asm("hlt");
            } // KERNEL PANIC

            return data[index];
        }

        size_t sizee() const {
            return size;
        }
        size_t capacityy() const {
            return capacity;
        }
        bool empty() const {
            return size == 0;
        }
};