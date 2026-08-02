#pragma once

#include "definitions.h"

class String {
    private:
        char* m_data;
        size_t m_length;
        size_t m_capacity;

        void reallocate(size_t new_capacity);

    public:
        String() noexcept;
        String(const char* str) noexcept;
        String(const String& other) noexcept;
        String& operator=(const String& other) noexcept;
        ~String() noexcept;
        
        void push_back(char c) noexcept;
        void pop_back() noexcept;
        void clear() noexcept;
        
        String& operator+=(char c) noexcept;
        String& operator+=(const char* str) noexcept;
        String& operator+=(const String& other) noexcept;
        
        bool operator==(const char* str) const noexcept;
        bool operator==(const String& other) const noexcept;
        bool operator!=(const char* str) const noexcept;
        bool operator!=(const String& other) const noexcept;
        
        String& operator=(const char* str) noexcept;

        char operator[](size_t index) const noexcept { return m_data[index]; }
        char& operator[](size_t index) noexcept { return m_data[index]; }
        const char* c_str() const noexcept { return m_data ? m_data : ""; }
        size_t length() const noexcept { return m_length; }
        size_t capacity() const noexcept { return m_capacity; }
        bool empty() const noexcept { return m_length == 0; }
};