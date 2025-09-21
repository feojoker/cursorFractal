#ifndef _LIB_COMPLEX_H
#define _LIB_COMPLEX_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
// Linux: Include OpenCL but define missing vector types
#include <CL/opencl.h>

// Define OpenCL vector types for C compilation
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

// Helper functions for vector operations
static inline float3 float3_subtract(float3 a, float3 b) {
  float3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
  return result;
}

static inline float3 float3_add(float3 a, float3 b) {
  float3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
  return result;
}

static inline float3 float3_multiply(float3 a, float scalar) {
  float3 result = {a.x * scalar, a.y * scalar, a.z * scalar};
  return result;
}

static inline float3 float3_scale_add(float3 p0, float scalar, float3 diff) {
  float3 result = {p0.x + scalar * diff.x, p0.y + scalar * diff.y,
                   p0.z + scalar * diff.z};
  return result;
}

// Macro to handle vector operations for Linux compatibility
#define VECTOR_OP_ADD(a, b) float3_add(a, b)
#define VECTOR_OP_SUB(a, b) float3_subtract(a, b)
#define VECTOR_OP_MUL(a, s) float3_multiply(a, s)
#define VECTOR_OP_SCALE_ADD(p0, s, diff) float3_scale_add(p0, s, diff)

#endif
#include "math.h"

#if TEST_DOUBLE_SUPPORT
#if defined(cl_khr_fp64) // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#elif defined(cl_amd_fp64) // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE
#endif
#endif

#if defined(DOUBLE_SUPPORT_AVAILABLE)
typedef double2 cl_complex;
typedef cl_double4 cl_quat;
typedef double TYPE;
#else
typedef float2 cl_complex;
typedef float4 cl_quat;
typedef float TYPE;
#endif

#define I_cl ((cl_complex)(0.0, 1.0))
#define Pi_cl 3.14159265358979323846
#define E_cl 2.718281828459045235360

TYPE cl_creal(cl_complex n);
TYPE cl_cimag(cl_complex n);
TYPE cl_cmod(cl_complex n);
cl_complex cl_cadd(cl_complex a, cl_complex b);
cl_complex cl_cmult(cl_complex a, cl_complex b);
cl_complex cl_cpow(cl_complex base, int exp);
cl_complex cl_cdiv(cl_complex a, cl_complex b);
TYPE cl_carg(cl_complex a);
cl_complex cl_csqrt(cl_complex n);
cl_complex cl_cexp(cl_complex n);
cl_complex cl_clog(cl_complex z);
TYPE cl_cdot(cl_complex a, cl_complex b);
cl_quat cl_quat_mult(cl_quat q1, cl_quat q2);
cl_quat cl_quat_sum(cl_quat q1, cl_quat q2);
cl_quat cl_quat_conjugate(cl_quat q);
TYPE cl_quat_mod(cl_quat q);

#endif
