#include "gui.h"
#include <stdio.h>

static t_gui_state gui_state = {0};

void gui_init(GLFWwindow *window) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Initialize GUI state with default values
  gui_state.max_iterations = 6;
  gui_state.julia_c[0] = 0.0f;
  gui_state.julia_c[1] = 0.0f;
  gui_state.julia_c[2] = 0.0f;
  gui_state.julia_c[3] = 0.0f;
  gui_state.grid_size = 50.0f;

  gui_state.wireframe_mode = true; // Start in wireframe mode
  gui_state.show_axes = false;
  gui_state.auto_rotate = false;
  gui_state.rotation_speed = 1.0f;

  gui_state.base_color[0] = 0.0f; // Background color (black)
  gui_state.base_color[1] = 0.0f;
  gui_state.base_color[2] = 0.0f;
  gui_state.edge_color[0] = 1.0f; // Line color (white)
  gui_state.edge_color[1] = 1.0f;
  gui_state.edge_color[2] = 1.0f;
  gui_state.use_gradient = false;

  gui_state.use_optimized = true;
  gui_state.show_performance = true;

  gui_state.show_demo_window = false;
  gui_state.show_fractal_controls = true;
  gui_state.show_rendering_controls = true;
  gui_state.show_performance_window = false;

  printf("GUI initialized successfully\n");
}

void gui_shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void gui_new_frame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void gui_render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui_render_main_menu(t_data *data, t_gui_state *gui_state) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Fractal Controls", NULL,
                      &gui_state->show_fractal_controls);
      ImGui::MenuItem("Rendering Controls", NULL,
                      &gui_state->show_rendering_controls);
      ImGui::MenuItem("Performance", NULL, &gui_state->show_performance_window);
      ImGui::Separator();
      ImGui::MenuItem("ImGui Demo", NULL, &gui_state->show_demo_window);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Presets")) {
      if (ImGui::MenuItem("Default Julia Set")) {
        gui_state->julia_c[0] = 0.0f;
        gui_state->julia_c[1] = 0.0f;
        gui_state->julia_c[2] = 0.0f;
        gui_state->julia_c[3] = 0.0f;
        gui_state->max_iterations = 6;
        gui_apply_fractal_changes(data, gui_state);
      }
      if (ImGui::MenuItem("Complex Julia Set")) {
        gui_state->julia_c[0] = -0.4f;
        gui_state->julia_c[1] = 0.6f;
        gui_state->julia_c[2] = 0.0f;
        gui_state->julia_c[3] = 0.0f;
        gui_state->max_iterations = 8;
        gui_apply_fractal_changes(data, gui_state);
      }
      if (ImGui::MenuItem("Quaternion Julia Set")) {
        gui_state->julia_c[0] = 0.18f;
        gui_state->julia_c[1] = 0.0f;
        gui_state->julia_c[2] = 0.0f;
        gui_state->julia_c[3] = 0.78f;
        gui_state->max_iterations = 10;
        gui_apply_fractal_changes(data, gui_state);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Export")) {
      if (ImGui::MenuItem("Save as OBJ")) {
        data->gl->export_obj = 1;
        printf("Export flag set - model will be saved on exit\n");
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void gui_render_fractal_controls(t_data *data, t_gui_state *gui_state) {
  if (!gui_state->show_fractal_controls)
    return;

  if (ImGui::Begin("Fractal Controls", &gui_state->show_fractal_controls)) {
    ImGui::Text("4D Julia Set Parameters");
    ImGui::Separator();

    // Iteration control
    int old_iterations = gui_state->max_iterations;
    if (ImGui::SliderInt("Iterations", &gui_state->max_iterations, 1, 20)) {
      if (gui_state->max_iterations != old_iterations) {
        gui_apply_fractal_changes(data, gui_state);
      }
    }

    ImGui::Text("Current: %d iterations", gui_state->max_iterations);

    ImGui::Separator();
    ImGui::Text("Quaternion Parameters (c = a + bi + cj + dk)");

    // Quaternion parameters
    bool params_changed = false;
    params_changed |= ImGui::SliderFloat("a (real)", &gui_state->julia_c[0],
                                         -2.0f, 2.0f, "%.3f");
    params_changed |= ImGui::SliderFloat("b (i)", &gui_state->julia_c[1], -2.0f,
                                         2.0f, "%.3f");
    params_changed |= ImGui::SliderFloat("c (j)", &gui_state->julia_c[2], -2.0f,
                                         2.0f, "%.3f");
    params_changed |= ImGui::SliderFloat("d (k)", &gui_state->julia_c[3], -2.0f,
                                         2.0f, "%.3f");

    if (params_changed) {
      gui_apply_fractal_changes(data, gui_state);
    }

    ImGui::Separator();

    // Grid size control
    float old_grid_size = gui_state->grid_size;
    if (ImGui::SliderFloat("Grid Resolution", &gui_state->grid_size, 20.0f,
                           100.0f, "%.0f")) {
      if (gui_state->grid_size != old_grid_size) {
        // Grid size change requires full regeneration
        printf("Grid size changed to %.0f - full regeneration needed\n",
               gui_state->grid_size);
        // This would require more extensive changes to the data structure
      }
    }

    ImGui::Separator();

    // Performance options
    bool old_optimized = gui_state->use_optimized;
    ImGui::Checkbox("Use Optimized Generation", &gui_state->use_optimized);
    if (gui_state->use_optimized != old_optimized) {
      printf("Optimization mode changed - restart required for full effect\n");
    }

    if (ImGui::Button("Regenerate Fractal")) {
      gui_apply_fractal_changes(data, gui_state);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset to Default")) {
      gui_state->max_iterations = 6;
      gui_state->julia_c[0] = 0.0f;
      gui_state->julia_c[1] = 0.0f;
      gui_state->julia_c[2] = 0.0f;
      gui_state->julia_c[3] = 0.0f;
      gui_apply_fractal_changes(data, gui_state);
    }
  }
  ImGui::End();
}

void gui_render_rendering_controls(t_data *data, t_gui_state *gui_state) {
  if (!gui_state->show_rendering_controls)
    return;

  if (ImGui::Begin("Rendering Controls", &gui_state->show_rendering_controls)) {
    ImGui::Text("Display Mode");
    ImGui::Separator();

    // Wireframe toggle
    bool old_wireframe = gui_state->wireframe_mode;
    ImGui::Checkbox("Wireframe Mode", &gui_state->wireframe_mode);
    if (gui_state->wireframe_mode != old_wireframe) {
      gui_apply_rendering_changes(data, gui_state);
    }

    ImGui::Checkbox("Show Axes", &gui_state->show_axes);

    ImGui::Separator();
    ImGui::Text("Animation");

    bool auto_rotate_changed =
        ImGui::Checkbox("Auto Rotate", &gui_state->auto_rotate);
    bool speed_changed = false;
    if (gui_state->auto_rotate) {
      speed_changed = ImGui::SliderFloat(
          "Rotation Speed", &gui_state->rotation_speed, 0.1f, 5.0f, "%.1fx");
    }

    if (auto_rotate_changed || speed_changed) {
      gui_apply_rendering_changes(data, gui_state);
    }

    ImGui::Separator();
    ImGui::Text("Colors");

    bool color_changed = false;
    color_changed |=
        ImGui::ColorEdit3("Background Color", gui_state->base_color);
    color_changed |= ImGui::ColorEdit3("Line Color", gui_state->edge_color);
    ImGui::Checkbox("Use Color Gradient", &gui_state->use_gradient);

    if (color_changed) {
      gui_apply_rendering_changes(data, gui_state);
    }

    ImGui::Separator();

    if (ImGui::Button("Reset View")) {
      // Reset camera position
      if (data->gl && data->gl->matrix) {
        data->gl->matrix->rotation_x = 0.0f;
        data->gl->matrix->rotation_y = 0.0f;
        data->gl->matrix->pan_x = 0.0f;
        data->gl->matrix->pan_y = 0.0f;
        data->gl->matrix->zoom = 1.0f;
      }
      printf("View reset to default\n");
    }
  }
  ImGui::End();
}

void gui_render_performance_window(t_data *data, t_gui_state *gui_state) {
  if (!gui_state->show_performance_window)
    return;

  if (ImGui::Begin("Performance Monitor",
                   &gui_state->show_performance_window)) {
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Text("Application Performance");
    ImGui::Separator();

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

    ImGui::Separator();
    ImGui::Text("Fractal Statistics");

    if (data->gl) {
      ImGui::Text("Triangles: %d", data->gl->num_tris);
      ImGui::Text("Vertices: %d", data->gl->num_pts);
    }

    if (data->fract) {
      ImGui::Text("Grid Size: %.0f", data->fract->grid_size);
      ImGui::Text("Iterations: %d", data->fract->julia->max_iter);
    }

    ImGui::Separator();
    ImGui::Text("Memory Usage");

    // Simple memory estimation
    if (data->gl && data->fract) {
      size_t triangle_memory =
          data->gl->num_tris * sizeof(float) * 9; // 3 vertices * 3 coords
      size_t grid_memory = data->fract->grid_size * data->fract->grid_size *
                           data->fract->grid_size * 8 * sizeof(float);

      ImGui::Text("Triangle Data: %.2f KB", triangle_memory / 1024.0f);
      ImGui::Text("Grid Data: %.2f KB", grid_memory / 1024.0f);
      ImGui::Text("Total Est.: %.2f KB",
                  (triangle_memory + grid_memory) / 1024.0f);
    }

    ImGui::Separator();

    static bool show_metrics = false;
    ImGui::Checkbox("Show ImGui Metrics", &show_metrics);
    if (show_metrics) {
      ImGui::ShowMetricsWindow(&show_metrics);
    }
  }
  ImGui::End();
}

void gui_apply_fractal_changes(t_data *data, t_gui_state *gui_state) {
  if (!data || !data->fract || !data->fract->julia)
    return;

  printf("Applying fractal changes: iterations=%d, c=(%.3f,%.3f,%.3f,%.3f)\n",
         gui_state->max_iterations, gui_state->julia_c[0],
         gui_state->julia_c[1], gui_state->julia_c[2], gui_state->julia_c[3]);

  // Update iteration count
  data->fract->julia->max_iter = gui_state->max_iterations;

  // Update Julia set parameters (quaternion c)
  // Note: This requires the data structure to support quaternion parameters
  // For now, we'll just update what we can

  // Trigger regeneration
  data->needs_regeneration = 1;
}

void gui_apply_rendering_changes(t_data *data, t_gui_state *gui_state) {
  if (!data || !data->gl)
    return;

  printf("Applying rendering changes: wireframe=%s, auto_rotate=%s\n",
         gui_state->wireframe_mode ? "on" : "off",
         gui_state->auto_rotate ? "on" : "off");

  // Update OpenGL state
  data->gl->wireframe_mode = gui_state->wireframe_mode;
  data->gl->auto_rotate = gui_state->auto_rotate;
  data->gl->rotation_speed = gui_state->rotation_speed;

  // Copy colors
  data->gl->background_color[0] = gui_state->base_color[0];
  data->gl->background_color[1] = gui_state->base_color[1];
  data->gl->background_color[2] = gui_state->base_color[2];

  data->gl->line_color[0] = gui_state->edge_color[0];
  data->gl->line_color[1] = gui_state->edge_color[1];
  data->gl->line_color[2] = gui_state->edge_color[2];
}

// C interface functions for integration with existing C code
extern "C" {
void gui_init_c(GLFWwindow *window) { gui_init(window); }

void gui_shutdown_c() { gui_shutdown(); }

void gui_new_frame_c() { gui_new_frame(); }

void gui_render_c() { gui_render(); }

void gui_render_all_c(t_data *data) {
  // Sync GUI state with OpenGL state
  if (data && data->gl) {
    gui_state.wireframe_mode = data->gl->wireframe_mode;
    gui_state.auto_rotate = data->gl->auto_rotate;
    gui_state.rotation_speed = data->gl->rotation_speed;

    gui_state.base_color[0] = data->gl->background_color[0];
    gui_state.base_color[1] = data->gl->background_color[1];
    gui_state.base_color[2] = data->gl->background_color[2];

    gui_state.edge_color[0] = data->gl->line_color[0];
    gui_state.edge_color[1] = data->gl->line_color[1];
    gui_state.edge_color[2] = data->gl->line_color[2];
  }

  gui_render_main_menu(data, &gui_state);
  gui_render_fractal_controls(data, &gui_state);
  gui_render_rendering_controls(data, &gui_state);
  gui_render_performance_window(data, &gui_state);

  // Show ImGui demo window if requested
  if (gui_state.show_demo_window) {
    ImGui::ShowDemoWindow(&gui_state.show_demo_window);
  }
}
}
