#include "morphosis.h"

// Fast fractal regeneration for real-time iteration changes
void regenerate_fractal(t_data *data) {
  if (!data->needs_regeneration)
    return;

  printf("Regenerating fractal with %d iterations...\n",
         data->fract->julia->max_iter);

  // Clean up old triangle data
  if (data->triangles) {
    clean_trigs(data->triangles, data->len.x);
    data->triangles = NULL;
  }

  // Reset counters
  data->len.x = 0;
  data->len.y = 0;

  // Use optimized fractal generation if available
#ifdef OPTIMIZED
  build_fractal_optimized(data);
#else
  build_fractal(data);
#endif

  // Update GL data
  gl_retrieve_tris(data);

  // Update VBO with new triangle data
  if (data->gl->vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, data->gl->vbo);
    glBufferData(GL_ARRAY_BUFFER, data->gl->num_pts * sizeof(float),
                 (GLfloat *)data->gl->tris, GL_DYNAMIC_DRAW);
  }

  // Clean up calculation data
  clean_calcs(data);

  data->needs_regeneration = 0;
  printf("Fractal regenerated! %d triangles generated.\n", data->gl->num_tris);
}

// Safe fractal regeneration using existing functions
void regenerate_fractal_fast(t_data *data) {
  if (!data->needs_regeneration)
    return;

  printf("Fast regenerating fractal with %d iterations...\n",
         data->fract->julia->max_iter);

  // Clean up old triangle data first
  if (data->triangles) {
    clean_trigs(data->triangles, data->len.x);
    data->triangles = NULL;
  }

  // Reset counters
  data->len.x = 0;
  data->len.y = 0;

  // Use the existing, safe fractal generation functions
#ifdef OPTIMIZED
  calculate_point_cloud_optimized(data);
#else
  calculate_point_cloud(data);
  gl_retrieve_tris(data);
  clean_calcs(data);
#endif

  // Update VBO with new triangle data
  if (data->gl->vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, data->gl->vbo);
    glBufferData(GL_ARRAY_BUFFER, data->gl->num_pts * sizeof(float),
                 (GLfloat *)data->gl->tris, GL_DYNAMIC_DRAW);
  }

  data->needs_regeneration = 0;
  printf("Fast regeneration complete! Generated %d triangles\n", data->gl->num_tris);
}
