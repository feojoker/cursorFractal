#ifndef OPENCL_COMPAT_H
#define OPENCL_COMPAT_H

#ifdef __APPLE__
// macOS: Use native OpenCL
#include <OpenCL/opencl.h>

#else
// Linux: Simple, stable struct-based approach (no OpenCL dependencies)

// Standard types
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

// Vector types as structs (stable across all compilers)
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
  int x, y;
} int2;
typedef struct {
  int x, y, z;
} int3;
typedef struct {
  int x, y, z, w;
} int4;
typedef struct {
  uint x, y;
} uint2;
typedef struct {
  uint x, y, z;
} uint3;
typedef struct {
  uint x, y, z, w;
} uint4;

// Vector operations that mimic OpenCL behavior
static inline float3 float3_add(float3 a, float3 b) {
  return (float3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline float3 float3_subtract(float3 a, float3 b) {
  return (float3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline float3 float3_multiply_scalar(float3 a, float s) {
  return (float3){a.x * s, a.y * s, a.z * s};
}

static inline float3 float3_scale_add(float3 base, float scalar,
                                      float3 offset) {
  return (float3){base.x + scalar * offset.x, base.y + scalar * offset.y,
                  base.z + scalar * offset.z};
}

// Operator overloading macros for natural syntax
#define VEC3_ADD(a, b) float3_add(a, b)
#define VEC3_SUB(a, b) float3_subtract(a, b)
#define VEC3_MUL_SCALAR(a, s) float3_multiply_scalar(a, s)

// For expressions like: p0 + mu * (p1 - p0)
#define VEC3_INTERPOLATE(p0, mu, p1)                                           \
  float3_scale_add(p0, mu, float3_subtract(p1, p0))

#endif // __APPLE__

#endif // OPENCL_COMPAT_H
