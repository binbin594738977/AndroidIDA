//
// Created by qumiao on 2020/3/25.
//

#ifndef DOUYINENCRPT_UTIL_H
#define DOUYINENCRPT_UTIL_H

#include <stdio.h>
#include <string.h>
#include <string>

#include <iostream>
#include <fstream>

typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned char u8;
typedef signed char i8;

typedef union {
    void *ptr;
    u8 bytes[4];
} ptr_bytes_convector;


typedef union {
    u32 dwords[2];
    u64 product;
} MultiplyUnion;

typedef union {
    u32 val;
    u8 bytes[4];
} U32ByteArrayUnion;

u8 reverseBit(u8 val);
u32 bitFetch(u32 src, int offset, int count);
u32 reverseByte(u32 src);
u32 computeEncryptLoopCount(const u8 *vector);
void eorPart8(u8 *vector, u8 *content);
u8 *createBufferFromFile(const char *filePath, u32 &bufLen);

#endif //DOUYINENCRPT_UTIL_H
