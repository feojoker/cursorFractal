# 🌌 Morphosis: 4D Julia Fractal Visualizer

A program that creates beautiful 3D fractals from 4D mathematical formulas and displays them with rotating animations.

## What does this program do?

1. **Generates 4D Julia fractals** using complex mathematics
2. **Converts them to 3D shapes** using marching cubes algorithm
3. **Shows rotating animations** in an OpenGL window
4. **Exports 3D models** as `.obj` files

## Quick Start

### For Mac users:

1. **Install dependencies:**
   ```bash
   brew install glfw glew cglm
   ```

2. **Build the project:**
   ```bash
   make
   ```

3. **Run with default settings:**
   ```bash
   ./morphosis -d
   ```

### For other systems:

You'll need to install the same libraries using your system's package manager:
- **GLFW** - for window management
- **GLEW** - for OpenGL extensions
- **cglm** - for 3D mathematics

## How to use

### Default mode (easiest):
```bash
./morphosis -d
```

### Custom parameters:
```bash
./morphosis 0.05 -0.2 0.8 0.0 0.0
```
Then enter number of iterations when prompted.

### Other modes:
```bash
./morphosis -m filename.mat    # Load from matrix file
./morphosis -p filename.txt    # Load from poem file
```

## What you'll see

1. **Progress numbers** (1/60, 2/60, etc.) - the program is calculating the fractal
2. **OpenGL window** - shows the rotating 3D fractal
3. **fractal.obj file** - exported 3D model when you close the window

## Parameters explained

- **Step size** (0.05): How detailed the fractal is (smaller = more detail)
- **c.x, c.y, c.z, c.w**: Complex constant values that shape the fractal
- **Max iterations**: How deep to calculate (higher = more detail)

## Troubleshooting

**"Command not found"**: Make sure you're in the project directory
**"Library not found"**: Install the required dependencies
**No window opens**: You might be running without a display, or there could be graphics driver issues
**Program crashes**: Check that all dependencies are installed correctly

## What makes this special

- **4D mathematics**: Uses quaternions (4D complex numbers)
- **Real-time rendering**: Smooth OpenGL animations
- **Marching cubes**: Converts mathematical functions to 3D meshes
- **Export capability**: Saves results as standard 3D model files

## Files created

- `morphosis` - the compiled program
- `fractal.obj` - exported 3D model (created when you close the window)

---

*This program demonstrates the beautiful intersection of mathematics, computer graphics, and art!*
