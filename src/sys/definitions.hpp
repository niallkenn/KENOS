#pragma once

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

typedef __SIZE_TYPE__ size_t;
typedef int ssize_t;

typedef int ptrdiff_t;
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;

extern "C" {
    extern uint32_t _kernel_end[];
    extern uint32_t _kernel_start[];
}