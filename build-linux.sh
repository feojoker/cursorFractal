#!/bin/bash

# Linux build script for morphosis
set -e

echo "🔧 Building morphosis for Linux..."

# Create obj directory
mkdir -p obj

# Compiler settings
CC=${CC:-gcc}
CXX=${CXX:-g++}
# Add OpenCL vector extension support and define OpenCL target version
FLAGS="-O3 -Wall -I./includes -I./libft -I./imgui -I./imgui/backends -DCL_TARGET_OPENCL_VERSION=120 -DOPENCL_C_VERSION=120"
GL_LIBS="-lGL -lGLEW -lglfw -lm -ldl"
OPENSSL_LIB="-lssl -lcrypto"

# C source files
SRC_FILES=(
    "main.c"
    "cleanup.c"
    "init.c"
    "errors.c"
    "utils.c"
    "point_cloud.c"
    "build_fractal.c"
    "sample_julia.c"
    "polygonisation.c"
    "write_obj.c"
    "gl_draw.c"
    "gl_utils.c"
    "gl_buffers.c"
    "gl_build.c"
    "gl_points.c"
    "gl_init.c"
    "gl_calculations.c"
    "gl_mouse_controls.c"
    "gl_regeneration.c"
    "export_json.c"
    "obj.c"
    "lib_complex.c"
    "matrix_converter.c"
    "matrix_hash.c"
    "matrix_generate_coordinates.c"
    "matrix_read.c"
    "poem.c"
)

# C++ source files
CPP_FILES=(
    "gui.cpp"
)

# ImGui source files
IMGUI_FILES=(
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
)

# Compile C files
echo "📝 Compiling C files..."
for src in "${SRC_FILES[@]}"; do
    echo "  - $src"
    $CC $FLAGS -o obj/${src%.c}.o -c srcs/$src
done

# Compile C++ files
echo "📝 Compiling C++ files..."
for src in "${CPP_FILES[@]}"; do
    echo "  - $src"
    $CXX $FLAGS -o obj/${src%.cpp}.o -c srcs/$src
done

# Compile ImGui files
echo "📝 Compiling ImGui files..."
for src in "${IMGUI_FILES[@]}"; do
    echo "  - $src"
    $CXX $FLAGS -o ${src%.cpp}.o -c $src
done

# Link everything
echo "🔗 Linking morphosis..."
# On Linux, we don't use libft.a (use compatibility functions instead)
$CXX obj/*.o imgui/*.o imgui/backends/*.o -o morphosis $GL_LIBS $OPENSSL_LIB

echo "✅ Build complete! Binary: ./morphosis"
