#ifndef STDLIBH
#define STDLIBH

#define COLOR_RGB(r, g, b)  (((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))

extern char* itoa(int num, char* str);
extern int strlen(const char* str);

#endif