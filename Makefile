NAME = morphosis

SRC_DIR = ./srcs/
SRC = 	main.c \
		cleanup.c \
		init.c \
		errors.c \
		utils.c \
		point_cloud.c \
		build_fractal.c \
		sample_julia.c \
		polygonisation.c \
		write_obj.c \
		\
		gl_draw.c \
        gl_utils.c \
        gl_buffers.c \
        gl_build.c \
        gl_points.c \
        gl_init.c \
        gl_calculations.c \
        gl_mouse_controls.c \
        gl_regeneration.c \
        \
        obj.c \
        \
        lib_complex.c \
        \
        matrix_converter.c \
        matrix_hash.c \
        matrix_generate_coordinates.c \
        matrix_read.c \
        poem.c

# Optimized sources
SRC_OPTIMIZED = utils_optimized.c \
                build_fractal_optimized.c \
                point_cloud_optimized.c

# GUI sources (C++)
SRC_GUI = gui.cpp

SRCS = $(addprefix $(SRC_DIR), $(SRC))
SRCS_GUI = $(addprefix $(SRC_DIR), $(SRC_GUI))
OBJS_GUI = $(addprefix $(OBJ_DIR), $(SRC_GUI:.cpp=.o))
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))
OBJ_DIR = ./obj/
OBJ = $(SRC:.c=.o)

INCS = $(addprefix $(INC_DIR), $(INC))
INC_DIR = ./includes/
INC = 	morphosis.h \
		gl_includes.h \
		stb_image.h \
		errors.h \
		lib_complex.h \
		structures.h \
		look-up.h \
		obj.h \
		matrix.h

LIB_INC = libft.h get_next_line.h
LIB_INC_DIR = ./libft/
LIB_INCS = $(addprefix $(LIB_INC_DIR), $(LIB_INC))

FLAGS = -O3 -Wall -I$(INC_DIR) -I$(LIB_INC_DIR) -I/opt/homebrew/include -I./imgui -I./imgui/backends
FLAGS_OPTIMIZED = -O3 -march=native -Wall -I$(INC_DIR) -I$(LIB_INC_DIR) -I/opt/homebrew/include -I./imgui -I./imgui/backends -DOPTIMIZED
FLAGS_PARALLEL = -O3 -march=native -Wall -I$(INC_DIR) -I$(LIB_INC_DIR) -I/opt/homebrew/include -I./imgui -I./imgui/backends -DOPTIMIZED -DPARALLEL -fopenmp

GL_LIBS = -framework OpenGL -lGLEW -lglfw -L/opt/homebrew/lib
OPENSSL_LIB = -lssl -lcrypto -L/opt/homebrew/lib -I/opt/homebrew/include

# ImGui sources
IMGUI_DIR = ./imgui/
IMGUI_SOURCES = $(IMGUI_DIR)imgui.cpp \
                $(IMGUI_DIR)imgui_demo.cpp \
                $(IMGUI_DIR)imgui_draw.cpp \
                $(IMGUI_DIR)imgui_tables.cpp \
                $(IMGUI_DIR)imgui_widgets.cpp \
                $(IMGUI_DIR)backends/imgui_impl_glfw.cpp \
                $(IMGUI_DIR)backends/imgui_impl_opengl3.cpp

IMGUI_OBJECTS = $(IMGUI_SOURCES:.cpp=.o)

# Source and object files for optimized version
SRCS_OPTIMIZED = $(addprefix $(SRC_DIR), $(SRC_OPTIMIZED))
OBJS_OPTIMIZED = $(addprefix $(OBJ_DIR), $(SRC_OPTIMIZED:.c=.o))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS) $(OBJS_GUI) $(IMGUI_OBJECTS)
		clang++ $(OBJS) $(OBJS_GUI) $(IMGUI_OBJECTS) ./libft/libft.a -o $(NAME) $(GL_LIBS) $(OPENSSL_LIB)

# Optimized version
$(NAME)_optimized: $(OBJ_DIR) $(OBJS) $(OBJS_OPTIMIZED) $(OBJ_DIR)main_optimized.o $(OBJS_GUI) $(IMGUI_OBJECTS)
		clang++ $(filter-out $(OBJ_DIR)main.o,$(OBJS)) $(OBJ_DIR)main_optimized.o $(OBJS_OPTIMIZED) $(OBJS_GUI) $(IMGUI_OBJECTS) ./libft/libft.a -o $(NAME)_optimized $(GL_LIBS) $(OPENSSL_LIB)

# Parallel version (requires OpenMP)
$(NAME)_parallel: $(OBJ_DIR) $(OBJS) $(OBJS_OPTIMIZED) $(OBJ_DIR)main_optimized.o
		clang $(filter-out $(OBJ_DIR)main.o,$(OBJS)) $(OBJ_DIR)main_optimized.o $(OBJS_OPTIMIZED) ./libft/libft.a -o $(NAME)_parallel $(GL_LIBS) $(OPENSSL_LIB) -fopenmp

$(OBJ_DIR):
		mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCS)
		clang $(FLAGS) -o $@ -c $<

# Compile main.c with optimized flags for optimized version
$(OBJ_DIR)main_optimized.o: $(SRC_DIR)main.c $(INCS)
		clang $(FLAGS_OPTIMIZED) -o $@ -c $<

# Compile optimized sources with special flags
$(OBJ_DIR)utils_optimized.o: $(SRC_DIR)utils_optimized.c $(INCS)
		clang $(FLAGS_OPTIMIZED) -o $@ -c $<

$(OBJ_DIR)build_fractal_optimized.o: $(SRC_DIR)build_fractal_optimized.c $(INCS)
		clang $(FLAGS_OPTIMIZED) -o $@ -c $<

$(OBJ_DIR)point_cloud_optimized.o: $(SRC_DIR)point_cloud_optimized.c $(INCS)
		clang $(FLAGS_OPTIMIZED) -o $@ -c $<

# ImGui compilation rules
%.o: %.cpp
		clang++ $(FLAGS) -o $@ -c $<

# GUI compilation rules
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INCS)
		clang++ $(FLAGS) -o $@ -c $<

clean:
		@rm -f $(OBJS)
		@rm -rf $(OBJ_DIR)

fclean: clean
		@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
