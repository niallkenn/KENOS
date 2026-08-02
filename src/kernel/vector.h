#pragma once

#include "memoryallocator.h"
#include "definitions.h"

template <Typename T>
class Vector {
    private:
        T* data;
        size_t size;
        size_t capacity;
        
        void reallocate(size_t new_capacity);

    public:
        Vector() = default;

        ~Vector() {
            delete[] data;
        }

        Vector(const Vector& other);
        Vector& operator=(const Vector& other);

        void push_back(const T& value);
        void pop_back();

        T& operator[](size_t index);
        T& at(size_t index);

        size_t size() const;
        size_t capacity() const;
        bool empty() const;
};