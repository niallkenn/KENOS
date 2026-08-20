#pragma once

#include "definitions.h"


class kString {
    private:
        char* m_data;
        size_t m_size;
        size_t m_capacity;
        
        size_t strlen(const char* str) const;
        void memcpy(char* dst, const char* src, size_t n);
        
    public:
        kString();
        kString(const char* str);
        kString(const kString& other);
        kString(kString&& other) noexcept;
        ~kString();

        kString& operator=(const kString& other);
        kString& operator=(kString&& other) noexcept;

        void resize(size_t new_capacity);
        void push_back(const char* str);
        void push_back(const kString& other);
        void push_back(char c);
        void pop_back();
        void clear();

        bool equalTo(const kString& str) const;
        bool equalTo(const char* str) const;
        bool isNumber() const;

        uint32_t toInt() const;

        bool isEmpty() const { return m_size == 0; }
        const char* c_str() const { return m_data; }
        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }
        char operator[](size_t index) const { return m_data[index]; }
        char& operator[](size_t index) { return m_data[index]; }     
};