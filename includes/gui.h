#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "morphosis.h"

// GUI state structure
typedef struct {
  // Fractal parameters
  int max_iterations;
  float julia_c[4]; // Quaternion parameters for Julia set
  float grid_size;

  // Rendering options
  bool wireframe_mode;
  bool show_axes;
  bool auto_rotate;
  float rotation_speed;

  // Color settings
  float base_color[3];
  float edge_color[3];
  bool use_gradient;

  // Performance settings
  bool use_optimized;
  bool show_performance;

  // UI state
  bool show_demo_window;
  bool show_fractal_controls;
  bool show_rendering_controls;
  bool show_performance_window;
} t_gui_state;

// Function prototypes
void gui_init(GLFWwindow *window);
void gui_shutdown();
void gui_new_frame();
void gui_render();
void gui_handle_events();

// GUI panels
void gui_render_main_menu(t_data *data, t_gui_state *gui_state);
void gui_render_fractal_controls(t_data *data, t_gui_state *gui_state);
void gui_render_rendering_controls(t_data *data, t_gui_state *gui_state);
void gui_render_performance_window(t_data *data, t_gui_state *gui_state);

// Utility functions
void gui_apply_fractal_changes(t_data *data, t_gui_state *gui_state);
void gui_apply_rendering_changes(t_data *data, t_gui_state *gui_state);

#endif
