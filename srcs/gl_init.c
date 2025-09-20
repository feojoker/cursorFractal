#include "morphosis.h"

t_matrix *initGlMatrices(void) {
  t_matrix *matrix;

  if (!(matrix = (t_matrix *)malloc(sizeof(t_matrix))))
    error(MALLOC_FAIL_ERR, NULL);

  glm_mat4_identity(matrix->model_mat);
  glm_mat4_identity(matrix->projection_mat);
  glm_mat4_identity(matrix->view_mat);

  matrix->eye[0] = 1.5f;
  matrix->eye[1] = 1.5f;
  matrix->eye[2] = 0.0f;

  matrix->center[0] = 0.0f;
  matrix->center[1] = 0.0f;
  matrix->center[2] = 0.0f;

  matrix->up[0] = 0.8f;
  matrix->up[1] = 0.3f;
  matrix->up[2] = 0.7f;

  return matrix;
}

t_gl *init_gl_struct(void) {
  t_gl *gl;

  if (!(gl = (t_gl *)malloc(sizeof(t_gl))))
    error(MALLOC_FAIL_ERR, NULL);
  gl->window = NULL;
  gl->export_obj = 0;
  gl->shaderProgram = 0;
  gl->vertexShader = 0;
  gl->fragmentShader = 0;
  gl->vbo = 0;
  gl->vao = 0;
  gl->tris = NULL;
  gl->num_pts = 0;
  gl->matrix = initGlMatrices();

  // Initialize rendering state
  gl->wireframe_mode = 1; // Start in wireframe mode
  gl->background_color[0] = 0.0f;
  gl->background_color[1] = 0.0f;
  gl->background_color[2] = 0.0f;
  gl->line_color[0] = 1.0f;
  gl->line_color[1] = 1.0f;
  gl->line_color[2] = 1.0f;
  gl->auto_rotate = 0;
  gl->rotation_speed = 1.0f;

  return gl;
}
