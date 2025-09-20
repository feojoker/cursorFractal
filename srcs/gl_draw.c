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

  // Initialize GUI
  gui_init_c(gl->window);

  gl_render(gl);

  // Cleanup GUI
  gui_shutdown_c();

  terminate_gl(gl);
}

void gl_render(t_gl *gl) {
  while (!glfwWindowShouldClose(gl->window)) {
    processInput(gl->window, gl);

    // Check if fractal needs regeneration
    if (gl->data && gl->data->needs_regeneration) {
      regenerate_fractal_fast(gl->data);
    }

    // Start the Dear ImGui frame
    gui_new_frame_c();

    // Apply rendering mode (wireframe or solid)
    if (gl->wireframe_mode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Apply background color
    glClearColor(gl->background_color[0], gl->background_color[1],
                 gl->background_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply interactive transformations or auto-rotation
    if (gl->auto_rotate) {
      apply_auto_rotation(gl);
    } else {
      apply_interactive_transforms(gl);
    }

    // Render the 3D fractal
    glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);

    // Render GUI on top
    if (gl->data) {
      gui_render_all_c(gl->data);
    }
    gui_render_c();

    glfwSwapBuffers(gl->window);
    glfwPollEvents();
  }
}
