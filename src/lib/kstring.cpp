#include "kstring.h"
#include "heapallocator.h"

size_t kString::strlen(const char* str) const {
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void kString::memcpy(char* dst, const char* src, size_t n) {
    for (size_t i = 0; i < n; i++) dst[i] = src[i];
}

kString::kString() : m_data(nullptr), m_size(0), m_capacity(0) {
    m_data = new char[1];
    m_data[0] = '\0';
}

kString::kString(const char* str) : m_data(nullptr), m_size(strlen(str)), m_capacity(strlen(str)) {
    m_data = new char[m_capacity + 1];
    memcpy(m_data, str, m_size);
    m_data[m_size] = '\0';
}

kString::kString(const kString& other) : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity) {
    m_data = new char[m_capacity + 1];
    memcpy(m_data, other.m_data, m_size + 1);
}

kString::kString(kString&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

kString::~kString() {
    delete[] m_data;
}

kString& kString::operator=(const kString& other) {
    if (this != &other) {
        char* new_data = new char[other.m_capacity + 1];
        memcpy(new_data, other.m_data, other.m_size + 1);

        delete[] m_data;
        m_data = new_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }

    return *this;
}

kString& kString::operator=(kString&& other) noexcept {
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

void kString::resize(size_t new_capacity) {
    if (m_capacity >= new_capacity) return;

    char* new_data = new char[new_capacity + 1];
    memcpy(new_data, m_data, m_size + 1);

    delete[] m_data;

    m_data = new_data;
    m_capacity = new_capacity;
}

void kString::push_back(const char* str) {
    if (!str) return;
    size_t len = strlen(str);

    if (len == 0) return;

    if (m_size + len + 1 > m_capacity) {
        size_t new_cap = (m_capacity == 0) ? 16 : m_capacity * 2;
        while (new_cap < m_size + len + 1) {
            new_cap *= 2;
        }
        resize(new_cap);
    }

    for (size_t i = 0; i < len; i++) {
        m_data[m_size + i] = str[i];
    }

    m_size += len;

    m_data[m_size] = '\0';
}

void kString::push_back(const kString& other) {
    push_back(other.c_str());
}

void kString::push_back(char c) {
    char str[2] = {c, '\0'};
    push_back(str);
}

void kString::pop_back() {
    if (m_size != 0) {
        m_size--;
        m_data[m_size] = '\0';
    }
}

void kString::clear() {
    delete[] m_data;
    m_data = new char[1];
    m_data[0] = '\0';
    m_size = 0;
}

bool kString::equalTo(const char* str) const {
    size_t len = strlen(str);
    if (m_size != len) return false;

    for (size_t i = 0; i < m_size; i++) if (m_data[i] != str[i]) return false;

    return true;
}

bool kString::equalTo(const kString& other) const {
    return equalTo(other.c_str());
}

uint32_t kString::toInt() const {
    uint32_t result = 0;
    size_t start = 0;

    if (m_data[0] == '-') {
        start = 1;
    }

    for (size_t i = start; i < strlen(m_data); i++) {
        if (m_data[i] < '0' || m_data[i] > '9') return __UINT32_MAX__;

        result = (result * 10) + (m_data[i] - '0');
    }

    if (start == 1) result = -result;

    return result;
}

bool kString::isNumber() const {
    size_t start = 0;
    if (m_data[0] == '-') {
        start = 1;
    }

    for (size_t i = start; i < m_size; i++) {
        if (m_data[i] < '0' || m_data[i] > '9') return false;
    }

    return true;
}