#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

#define Kilobytes(size) (i32)(size*1024)
#define Megabytes(size) (i32)(Kilobytes(size)*1024)
#define Gigabytes(size) (i64)(Megabytes(size)*1024)
#define Terabytes(size) (i64)(Gigabytes(size)*1024)

#endif

