#pragma once

#include "definitions.h"

class String {
    private:
        char* data;
        size_t length;
        size_t capacity;

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

        char operator[](size_t index) const noexcept { return data[index]; }
        char& operator[](size_t index) noexcept { return data[index]; }
        const char* c_str() const noexcept { return data ? data : ""; }
        size_t length() const noexcept { return length; }
        size_t capacity() const noexcept { return capacity; }
        bool empty() const noexcept { return length == 0; }
};