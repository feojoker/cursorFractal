#include "morphosis.h"

// Mouse control state variables
static double last_mouse_x = 0.0;
static double last_mouse_y = 0.0;
static int mouse_left_pressed = 0;
static int mouse_right_pressed = 0;
static int first_mouse = 1;

// Camera control parameters
static float camera_sensitivity = 0.005f;
static float zoom_sensitivity = 0.1f;
static float pan_sensitivity = 0.01f;

// Auto-rotation state
static float auto_rotation_time = 0.0f;

// Mouse button callback
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      mouse_left_pressed = 1;
      first_mouse = 1; // Reset to avoid jump on first click
    } else if (action == GLFW_RELEASE) {
      mouse_left_pressed = 0;
    }
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      mouse_right_pressed = 1;
      first_mouse = 1; // Reset to avoid jump on first click
    } else if (action == GLFW_RELEASE) {
      mouse_right_pressed = 0;
    }
  }

  (void)gl;   // Suppress unused warning
  (void)mods; // Suppress unused warning
}

// Mouse movement callback
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);

  if (first_mouse) {
    last_mouse_x = xpos;
    last_mouse_y = ypos;
    first_mouse = 0;
    return;
  }

  double xoffset = xpos - last_mouse_x;
  double yoffset =
      last_mouse_y - ypos; // Reversed since y-coordinates go from bottom to top

  last_mouse_x = xpos;
  last_mouse_y = ypos;

  if (mouse_left_pressed) {
    // Left mouse drag: rotate model
    gl->matrix->rotation_x += (float)yoffset * camera_sensitivity;
    gl->matrix->rotation_y += (float)xoffset * camera_sensitivity;

    // Clamp vertical rotation to avoid flipping
    if (gl->matrix->rotation_x > 1.5f)
      gl->matrix->rotation_x = 1.5f;
    if (gl->matrix->rotation_x < -1.5f)
      gl->matrix->rotation_x = -1.5f;
  } else if (mouse_right_pressed) {
    // Right mouse drag: pan model
    gl->matrix->pan_x += (float)xoffset * pan_sensitivity;
    gl->matrix->pan_y += (float)yoffset * pan_sensitivity;
  }
}

// Mouse scroll callback for zooming
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);

  gl->matrix->zoom -= (float)yoffset * zoom_sensitivity;

  // Clamp zoom to reasonable values
  if (gl->matrix->zoom < 0.1f)
    gl->matrix->zoom = 0.1f;
  if (gl->matrix->zoom > 10.0f)
    gl->matrix->zoom = 10.0f;

  (void)xoffset; // Suppress unused warning
}

// Initialize mouse controls
void init_mouse_controls(t_gl *gl) {
  // Set the gl pointer as user data so callbacks can access it
  glfwSetWindowUserPointer(gl->window, gl);

  // Set up mouse callbacks
  glfwSetCursorPosCallback(gl->window, mouse_callback);
  glfwSetMouseButtonCallback(gl->window, mouse_button_callback);
  glfwSetScrollCallback(gl->window, scroll_callback);

  // Initialize transformation values
  gl->matrix->rotation_x = 0.0f;
  gl->matrix->rotation_y = 0.0f;
  gl->matrix->pan_x = 0.0f;
  gl->matrix->pan_y = 0.0f;
  gl->matrix->zoom = 1.0f;

  printf("Mouse controls initialized:\n");
  printf("  Left click + drag: Rotate model\n");
  printf("  Right click + drag: Pan model\n");
  printf("  Scroll wheel: Zoom in/out\n");
  printf("  ESC: Exit\n");
  printf("  S: Save model and exit\n");
}

// Apply interactive transformations to the model matrix
void apply_interactive_transforms(t_gl *gl) {
  t_matrix *matrix = gl->matrix;

  // Reset model matrix
  glm_mat4_identity(matrix->model_mat);

  // Apply zoom (scale)
  vec3 scale_vec = {matrix->zoom, matrix->zoom, matrix->zoom};
  glm_scale(matrix->model_mat, scale_vec);

  // Apply rotations
  vec3 x_axis = {1.0f, 0.0f, 0.0f};
  vec3 y_axis = {0.0f, 1.0f, 0.0f};
  glm_rotate(matrix->model_mat, matrix->rotation_x, x_axis);
  glm_rotate(matrix->model_mat, matrix->rotation_y, y_axis);

  // Apply translation (panning)
  vec3 translation = {matrix->pan_x, matrix->pan_y, 0.0f};
  glm_translate(matrix->model_mat, translation);

  // Update the shader uniform
  glUniformMatrix4fv(matrix->model, 1, GL_FALSE, (float *)matrix->model_mat);
}

// Auto-rotation function
void apply_auto_rotation(t_gl *gl) {
  if (!gl || !gl->matrix)
    return;

  t_matrix *matrix = gl->matrix;

  // Update rotation time
  auto_rotation_time += 0.016f * gl->rotation_speed; // Assume ~60 FPS

  // Apply automatic rotation
  glm_mat4_identity(matrix->model_mat);

  // Apply zoom first
  glm_scale_uni(matrix->model_mat, matrix->zoom);

  // Apply automatic rotation around both axes
  vec3 x_axis = {1.0f, 0.0f, 0.0f};
  vec3 y_axis = {0.0f, 1.0f, 0.0f};
  glm_rotate(matrix->model_mat, auto_rotation_time * 0.5f, x_axis);
  glm_rotate(matrix->model_mat, auto_rotation_time, y_axis);

  // Apply translation (panning) - user can still pan during auto-rotation
  vec3 translation = {matrix->pan_x, matrix->pan_y, 0.0f};
  glm_translate(matrix->model_mat, translation);

  // Update the shader uniform
  glUniformMatrix4fv(matrix->model, 1, GL_FALSE, (float *)matrix->model_mat);
}
