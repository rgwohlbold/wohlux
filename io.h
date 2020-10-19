#ifndef IO_H
#define IO_H
#include <stdint.h>

// uint8_t in8(uint16_t port);
// uint16_t in16(uint16_t port);
// uint32_t in32(uint16_t port);
// void out8(uint16_t port, uint8_t data);
// void out16(uint16_t port, uint16_t data);
// void out32(uint16_t port, uint32_t data);

static __attribute__((always_inline)) inline uint8_t in8(uint16_t port) {
    uint8_t b;
    asm volatile("inb %1, %0" : "=a"(b) : "Nd"(port));
    return b;
}

static __attribute__((always_inline)) inline uint16_t in16(uint16_t port) {
    uint16_t w;
    asm volatile("inw %1, %0" : "=a"(w) : "Nd"(port));
    return w;
} 

static __attribute__((always_inline)) inline uint32_t in32(uint16_t port) {
    uint32_t l;
    asm volatile("inl %1, %0" : "=a"(l) : "Nd"(port));
    return l;
}

static __attribute__((always_inline)) inline void out8(uint16_t port, uint8_t data) {
    asm volatile("outb %1, %0" : : "Nd"(port), "a"(data));
} 

static __attribute__((always_inline)) inline void out16(uint16_t port, uint16_t data) {
    asm volatile("outw %1, %0" : : "Nd"(port), "a"(data));
}

static __attribute__((always_inline)) inline void out32(uint16_t port, uint32_t data) {
    asm volatile("outl %1, %0" : : "Nd"(port), "a"(data));
}

#endif