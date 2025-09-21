#ifndef OPENCL_COMPAT_H
#define OPENCL_COMPAT_H

#ifdef __APPLE__
// macOS: Use native OpenCL
#include <OpenCL/opencl.h>

#else
// Linux: Create full OpenCL C compatibility layer

// Define OpenCL target version to suppress warnings BEFORE including headers
#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

// Standard OpenCL types (define BEFORE including OpenCL headers)
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

// Include basic OpenCL headers
#include <CL/opencl.h>

// OpenCL vector types with GCC vector extensions
typedef float float2 __attribute__((ext_vector_type(2)));
typedef float float3 __attribute__((ext_vector_type(3)));
typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int2 __attribute__((ext_vector_type(2)));
typedef int int3 __attribute__((ext_vector_type(3)));
typedef int int4 __attribute__((ext_vector_type(4)));
typedef unsigned int uint2 __attribute__((ext_vector_type(2)));
typedef unsigned int uint3 __attribute__((ext_vector_type(3)));
typedef unsigned int uint4 __attribute__((ext_vector_type(4)));

// If vector extensions don't work, fallback to structs
#ifndef __has_extension
#define __has_extension(x) 0
#endif

#if !__has_extension(attribute_ext_vector_type)
// Fallback struct definitions
#undef float2
#undef float3
#undef float4
#undef uint2
#undef uint3
#undef uint4

typedef struct {
  float x, y;
} float2;
typedef struct {
  float x, y, z;
} float3;
typedef struct {
  float x, y, z, w;
} float4;
typedef struct {
  unsigned int x, y;
} uint2;
typedef struct {
  unsigned int x, y, z;
} uint3;
typedef struct {
  unsigned int x, y, z, w;
} uint4;

// Vector operation helpers for struct fallback
static inline float3 float3_add(float3 a, float3 b) {
  return (float3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline float3 float3_sub(float3 a, float3 b) {
  return (float3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline float3 float3_mul_scalar(float3 a, float s) {
  return (float3){a.x * s, a.y * s, a.z * s};
}

// Override operators for structs (C doesn't support operator overloading, so we
// need macros)
#define VECTOR_ADD(a, b) float3_add(a, b)
#define VECTOR_SUB(a, b) float3_sub(a, b)
#define VECTOR_MUL_SCALAR(a, s) float3_mul_scalar(a, s)

#endif // !__has_extension(attribute_ext_vector_type)

#endif // __APPLE__

#endif // OPENCL_COMPAT_H
