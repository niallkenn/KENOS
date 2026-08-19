#pragma once

#include "definitions.h"
#include "heapallocator.h"

template <typename T>
class kVector {
    private:
        T* m_data;
        size_t m_size;
        size_t m_capacity;

    public:
        kVector() : m_data(nullptr), m_size(0), m_capacity(0) {}

        kVector(const kVector& other) : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity) {
            if (m_capacity == 0) return;
            m_data = new T[m_capacity];
            for (size_t i = 0; i < m_size; i++) m_data[i] = other.m_data[i];
        }

        kVector(kVector&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        ~kVector() {
            delete[] m_data;
        }

        kVector& operator=(const kVector& other) {
            if (this != &other) {
                T* new_data = new T[other.m_capacity];
                for (size_t i = 0; i < other.m_size; i++) new_data[i] = other.m_data[i];

                delete[] m_data;
                m_data = new_data;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
            }

            return *this;
        }

        kVector& operator=(kVector&& other) noexcept {
            if (this != &other) {
                delete[] m_data;

                m_data = other.m_data;
                m_size = other.m_size;
                m_capacity = other.m_capacity;

                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;
            }

            return *this;
        }


        void resize(size_t new_capacity) {
            if (m_capacity >= new_capacity) return;

            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < m_size; i++) new_data[i] = m_data[i];

            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }

        void push_back(T value) {
            if (m_size + 1 > m_capacity) {
                size_t new_capacity = (m_capacity == 0) ? 4 : m_capacity * 2;
                resize(new_capacity);
            }

            m_data[m_size] = value;
            m_size++;
        }

        void pop_back() {
            if (m_size != 0) {
                m_size--;
            }
        }

        void clear() {
            m_size = 0;
        }


        bool equalTo(const kVector& other) const {
            if (m_size != other.m_size) return false;

            for (size_t i = 0; i < m_size; i++) {
                if (m_data[i] != other.m_data[i]) return false;
            }

            return true;
        }
        

        bool isEmpty() const { return m_size == 0; }
        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }
        T operator[](size_t index) const { return m_data[index]; }
        T& operator[](size_t index) { return m_data[index]; }     
};