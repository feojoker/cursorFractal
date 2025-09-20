#include "morphosis.h"

void export_fractal_json(t_data *data, const char *filename) {
  FILE *file;
  int i;

  if (!data || !data->triangles || !filename) {
    printf("Error: Invalid data or filename for JSON export\n");
    return;
  }

  file = fopen(filename, "w");
  if (!file) {
    printf("Error: Could not create JSON file %s\n", filename);
    return;
  }

  printf("Exporting fractal data to %s...\n", filename);

  // Write JSON header
  fprintf(file, "{\n");
  fprintf(file, "  \"metadata\": {\n");
  fprintf(file, "    \"triangleCount\": %d,\n", data->len.x);
  fprintf(file, "    \"vertexCount\": %d,\n", data->len.x * 3);
  fprintf(file, "    \"iterations\": %d,\n", data->fract->julia->max_iter);
  fprintf(file, "    \"gridSize\": %.0f,\n", data->fract->grid_size);
  fprintf(file, "    \"stepSize\": %f,\n", data->fract->step_size);
  fprintf(file, "    \"juliaC\": [%f, %f, %f, %f]\n", data->fract->julia->c.x,
          data->fract->julia->c.y, data->fract->julia->c.z,
          data->fract->julia->c.w);
  fprintf(file, "  },\n");

  // Write vertices array (flattened for web consumption)
  fprintf(file, "  \"vertices\": [\n");
  for (i = 0; i < data->len.x; i++) {
    // Each triangle has 3 vertices
    fprintf(file, "    %f, %f, %f", data->triangles[i][0].x,
            data->triangles[i][0].y, data->triangles[i][0].z);
    fprintf(file, ",\n");
    fprintf(file, "    %f, %f, %f", data->triangles[i][1].x,
            data->triangles[i][1].y, data->triangles[i][1].z);
    fprintf(file, ",\n");
    fprintf(file, "    %f, %f, %f", data->triangles[i][2].x,
            data->triangles[i][2].y, data->triangles[i][2].z);
    if (i < data->len.x - 1) {
      fprintf(file, ",");
    }
    fprintf(file, "\n");
  }
  fprintf(file, "  ],\n");

  // Write indices array (each triangle has 3 vertices)
  fprintf(file, "  \"indices\": [\n");
  for (i = 0; i < data->len.x; i++) {
    fprintf(file, "    %d, %d, %d", i * 3, i * 3 + 1, i * 3 + 2);
    if (i < data->len.x - 1) {
      fprintf(file, ",");
    }
    fprintf(file, "\n");
  }
  fprintf(file, "  ]\n");

  // Write JSON footer
  fprintf(file, "}\n");

  fclose(file);
  printf("JSON export complete: %d triangles exported\n", data->len.x);
}
