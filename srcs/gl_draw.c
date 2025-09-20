#include "morphosis.h"

void run_graphics(t_gl *gl, float3 max, float3 min) {
  gl_scale_tris(gl, max, min);

  init_gl(gl);
  createVAO(gl);
  createVBO(gl, gl->num_pts * sizeof(float), (GLfloat *)gl->tris);

  makeShaderProgram(gl);
  gl_set_attrib_ptr(gl, "pos", 3, 3, 0);
  gl_calc_transforms(gl);

  // Initialize mouse controls
  init_mouse_controls(gl);

  gl_render(gl);

  terminate_gl(gl);
}

void gl_render(t_gl *gl) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(gl->window)) {
    processInput(gl->window, gl);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply interactive transformations instead of automatic rotation
    apply_interactive_transforms(gl);

    glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);

    glfwSwapBuffers(gl->window);
    glfwPollEvents();
  }
}
