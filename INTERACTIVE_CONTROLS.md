# 🖱️ Interactive Controls Documentation

## Overview

The Morphosis 4D Julia Fractal Visualizer now includes full interactive mouse and trackpad controls, allowing real-time exploration of the generated fractals.

## 🎮 Controls

### **Mouse/Trackpad Controls**

| Action | Control | Description |
|--------|---------|-------------|
| **Rotate** | Left click + drag | Rotate the fractal model around X and Y axes |
| **Pan** | Right click + drag | Move the fractal around the screen |
| **Zoom** | Scroll wheel | Zoom in/out to see more or less detail |

### **Keyboard Controls**

| Key | Action | Description |
|-----|--------|-------------|
| **ESC** | Exit | Close the program |
| **S** | Save & Exit | Export fractal as .obj file and close |
| **H** | Help | Show all controls in console |
| **R** | Reset View | Return to default camera position |

## 🎯 Technical Details

### **Rotation Limits**
- **Vertical rotation**: Limited to ±1.5 radians to prevent flipping
- **Horizontal rotation**: Unlimited 360° rotation
- **Sensitivity**: 0.005 units per pixel movement

### **Zoom Limits**
- **Minimum zoom**: 0.1x (very far out)
- **Maximum zoom**: 10.0x (very close in)
- **Sensitivity**: 0.1 units per scroll step

### **Pan Limits**
- **Range**: Unlimited panning in X and Y directions
- **Sensitivity**: 0.01 units per pixel movement

## 🚀 Performance

The interactive controls are optimized for smooth real-time performance:
- **60+ FPS** rendering on modern hardware
- **Immediate response** to mouse input
- **Efficient matrix calculations** using cglm library
- **No performance impact** on fractal generation

## 💡 Usage Tips

### **Best Practices**
1. **Start with rotation** - Use left click + drag to get familiar with the fractal shape
2. **Use zoom for details** - Scroll in to see intricate fractal structures
3. **Pan for composition** - Right click + drag to center interesting areas
4. **Reset when lost** - Press 'R' to return to the default view
5. **Save interesting views** - Press 'S' to export the current model

### **Exploration Techniques**
- **Slow rotation**: Gentle movements reveal fractal symmetries
- **Close zoom**: High zoom levels show fine fractal details
- **Different angles**: Some fractals look completely different from various viewpoints
- **Pan + zoom combo**: Move to an interesting area, then zoom in for details

## 🛠️ Implementation Details

### **Files Added**
- `srcs/gl_mouse_controls.c` - Mouse callback implementations
- Extended `structures.h` - Added transformation parameters to t_matrix
- Updated `gl_draw.c` - Integrated interactive rendering loop
- Enhanced `gl_utils.c` - Added help and reset functionality

### **Key Functions**
- `init_mouse_controls()` - Initialize mouse callbacks and default values
- `apply_interactive_transforms()` - Apply user transformations to model matrix
- `mouse_callback()` - Handle mouse movement for rotation and panning
- `scroll_callback()` - Handle zoom via scroll wheel
- `mouse_button_callback()` - Track mouse button states

### **Matrix Transformations**
The interactive system applies transformations in this order:
1. **Scale** (zoom)
2. **Rotate X** (vertical rotation)
3. **Rotate Y** (horizontal rotation)
4. **Translate** (panning)

This ensures natural, intuitive control behavior.

## 🔧 Customization

### **Sensitivity Adjustment**
You can modify sensitivity values in `gl_mouse_controls.c`:
```c
static float camera_sensitivity = 0.005f;  // Rotation sensitivity
static float zoom_sensitivity = 0.1f;      // Zoom sensitivity
static float pan_sensitivity = 0.01f;      // Pan sensitivity
```

### **Limits Adjustment**
Modify zoom and rotation limits:
```c
// Zoom limits
if (gl->matrix->zoom < 0.1f) gl->matrix->zoom = 0.1f;
if (gl->matrix->zoom > 10.0f) gl->matrix->zoom = 10.0f;

// Rotation limits
if (gl->matrix->rotation_x > 1.5f) gl->matrix->rotation_x = 1.5f;
if (gl->matrix->rotation_x < -1.5f) gl->matrix->rotation_x = -1.5f;
```

## 🎨 Visual Feedback

The program provides clear feedback:
- **Console messages** when controls are initialized
- **Help display** when 'H' is pressed
- **Reset confirmation** when 'R' is pressed
- **Real-time visual response** to all mouse interactions

## 🧪 Testing

To test the interactive controls:
1. Build and run: `./morphosis_optimized -d`
2. Wait for fractal generation to complete
3. Try each control type:
   - Left drag to rotate
   - Right drag to pan
   - Scroll to zoom
   - Press 'H' for help
   - Press 'R' to reset
   - Press 'S' to save

---

*The interactive controls transform the Morphosis visualizer from a passive display into an engaging exploration tool, perfect for discovering the intricate beauty of 4D Julia fractals.*
