#pragma once

#include "definitions.h"

template <typename T>
class kVector {
    private:
        T* m_data;
        size_t m_size;
        size_t m_capacity;

    public:
        kVector();
        kVector(const kVector& other);
        kVector(kVector&& other) noexcept;
        ~kVector();

        kVector& operator=(const kVector& other);
        kVector& operator=(kVector&& other) noexcept;

        void resize(size_t new_capacity);
        void push_back(T value);
        void pop_back();
        void clear();

        bool equalTo(const kVector& other) const;
        
        bool isEmpty() const { return m_size == 0; }
        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }
        T operator[](size_t index) const { return m_data[index]; }
        T& operator[](size_t index) { return m_data[index]; }     
};