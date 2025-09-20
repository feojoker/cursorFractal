#include "morphosis.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  int i = 0;
  glViewport(0, 0, width, height);

  if (window)
    i++;
}

void processInput(GLFWwindow *window, t_gl *gl) {
  static int h_pressed = 0;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    gl->export_obj = 1;
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !h_pressed) {
    h_pressed = 1;
    printf("\n=== MORPHOSIS CONTROLS ===\n");
    printf("Mouse Controls:\n");
    printf("  Left click + drag: Rotate model\n");
    printf("  Right click + drag: Pan model\n");
    printf("  Scroll wheel: Zoom in/out\n");
    printf("Keyboard Controls:\n");
    printf("  ESC: Exit\n");
    printf("  S: Save model and exit\n");
    printf("  H: Show this help\n");
    printf("  R: Reset view\n");
    printf("========================\n\n");
  }
  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE)
    h_pressed = 0;

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    // Reset view to default
    gl->matrix->rotation_x = 0.0f;
    gl->matrix->rotation_y = 0.0f;
    gl->matrix->pan_x = 0.0f;
    gl->matrix->pan_y = 0.0f;
    gl->matrix->zoom = 1.0f;
    printf("View reset to default\n");
  }
}

void init_gl(t_gl *gl) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  gl->window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Morphosis", NULL, NULL);
  glfwMakeContextCurrent(gl->window);
  glewExperimental = GL_TRUE;
  glewInit();
  if (!gl->window) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    exit(1);
  }
  glfwSetFramebufferSizeCallback(gl->window, framebuffer_size_callback);
  glEnable(GL_DEPTH_TEST);
}

void terminate_gl(t_gl *gl) {
  glDeleteVertexArrays(1, &gl->vao);
  glDeleteBuffers(1, &gl->vbo);
  glDeleteProgram(gl->shaderProgram);
  glfwTerminate();
}
