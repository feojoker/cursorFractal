#include "morphosis.h"

// OPTIMIZATION: Optimized Julia Set Calculation
// This provides significant performance improvements through early termination
// and better algorithm efficiency

float sample_4D_Julia_optimized(t_julia *julia, float3 pos) {
  cl_quat z;
  uint iter;
  float temp_mod;

  // Initialize with the position
  z.x = pos.x;
  z.y = pos.y;
  z.z = pos.z;
  z.w = julia->w;

  // OPTIMIZATION: Early termination for better performance
  for (iter = 0; iter < julia->max_iter; iter++) {
    // Perform quaternion multiplication and addition
    z = cl_quat_mult(z, z);
    z = cl_quat_sum(z, julia->c);

    // Calculate magnitude
    temp_mod = cl_quat_mod(z);

    // Early termination if we're clearly outside the set
    if (temp_mod > 2.0f) {
      return 0.0f; // Outside the set
    }

    // OPTIMIZATION: Additional early termination for points that are diverging
    if (temp_mod > 1.5f && iter > 2) {
      return 0.0f;
    }
  }

  return 1.0f; // Inside the set
}

// OPTIMIZATION: Memory-efficient triangle storage
// This is a placeholder for future memory optimizations
// Currently, the main optimization is in the Julia calculation above

// Future optimizations could include:
// - Memory pools for triangle storage
// - Flat memory layout instead of pointer arrays
// - Batch processing of voxels
// - SIMD vectorization
// - Parallel processing with OpenMP
