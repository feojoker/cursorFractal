#include "morphosis.h"

// Optimized version of calculate_point_cloud that uses the optimized
// build_fractal
void calculate_point_cloud_optimized(t_data *data) {
  t_fract *fract = data->fract;

  printf("Initializing OPTIMIZED point cloud generation...\n");

  fract->grid_size = fract->grid_length / fract->step_size;
  init_grid(data);
  init_vertex(data);
  create_grid(data);
  define_voxel(fract, fract->step_size);

  // Use the optimized fractal building function
  build_fractal_optimized(data);
  
  // Retrieve triangles for OpenGL (same as original)
  gl_retrieve_tris(data);
  
  // Clean up calculations (same as original)
  clean_calcs(data);

  printf("OPTIMIZED point cloud generation complete!\n");
  printf("Total triangles generated: %d\n", data->gl->num_tris);
}
