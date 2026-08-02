#include "string.h"

static size_t kstrlen(const char* str) {
    if (!str) return 0;
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void String::reallocate(size_t new_capacity) {
    char* new_data = new char[new_capacity + 1];

    if (m_data) {
        for (size_t i = 0; i < m_length; i++) {
            new_data[i] = m_data[i];
        }

        delete[] m_data;
    }

    new_data[m_length] = '\0';
    m_data = new_data;
    m_capacity = new_capacity;
}

String::String() noexcept : m_data(nullptr), m_length(0), m_capacity(0) {
    reallocate(8);
}

String::String(const char* str) noexcept : m_data(nullptr), m_length(0), m_capacity(0) {
    size_t len = kstrlen(str);
    reallocate(len > 8 ? len : 8);

    for (size_t i = 0; i < len; i++) {
        m_data[i] = str[i];
    }

    m_length = len;
    m_data[m_length] = '\0';
}

String::String(const String& other) noexcept : m_data(nullptr), m_length(0), m_capacity(0) {
    reallocate(other.m_length > 8 ? other.m_length : 8);

    for (size_t i = 0; i < other.m_length; i++) {
        m_data[i] = other.m_data[i];
    }

    m_length = other.m_length;
    m_data[m_length] = '\0';
}

String& String::operator=(const String& other) noexcept {
    if (this != &other) {
        clear();

        if (other.m_length > m_capacity) {
            reallocate(other.m_length);
        }

        for (size_t i = 0; i < other.m_length; i++) {
            m_data[i] = other.m_data[i];
        }

        m_length = other.m_length;
        m_data[m_length] = '\0';
    }

    return *this;
}

String::~String() noexcept {
    if (m_data) delete[] m_data;
}

void String::push_back(char c) noexcept {
    if (m_length >= m_capacity) {
        reallocate(m_capacity == 0 ? 8 : m_capacity * 2);
    }

    m_data[m_length] = c;
    m_length++;
    m_data[m_length] = '\0';
}

void String::pop_back() noexcept {
    if (m_length == 0) return;

    m_length--;
    m_data[m_length] = '\0';
}

void String::clear() noexcept {
    m_length = 0;
    if (m_data) {
        m_data[m_length] = '\0';
    }
}

String& String::operator+=(char c) noexcept {
    push_back(c);
    return *this;
}

String& String::operator+=(const char* str) noexcept {
    size_t strlen = kstrlen(str);

    for (size_t i = 0; i < strlen; i++) push_back(str[i]);

    return *this;
}

String& String::operator+=(const String& other) noexcept {
    for (size_t i = 0; i < other.m_length; i++) {
        push_back(other.m_data[i]);
    }

    return *this;
}

bool String::operator==(const char* str) const noexcept {
    size_t len = kstrlen(str);
    
    if (len != m_length) return 0;

    for (size_t i = 0; i < len; i++) {
        if (m_data[i] != str[i]) return false;
    }

    return true;
}

bool String::operator==(const String& other) const noexcept {
    if (other.m_length != m_length) return 0;

    for (size_t i = 0; i < m_length; i++) {
        if (m_data[i] != other.m_data[i]) return false;
    }

    return true;
}   

bool String::operator!=(const char* str) const noexcept {
    return !(*this == str);
}

bool String::operator!=(const String& other) const noexcept {
    return !(*this == other);
}

String& String::operator=(const char* str) {
    if (*this != str) {
        clear();

        size_t len = kstrlen(str);

        if (len > m_capacity) {
            reallocate(len);
        }

        for (size_t i = 0; i < len; i++) {
            m_data[i] = str[i];
        }

        m_length = len;
        m_data[m_length] = '\0';
    }

    return *this;
}