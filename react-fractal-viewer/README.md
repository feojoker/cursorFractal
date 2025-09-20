# 🌀 React 4D Julia Fractal Viewer

A modern, stable, and highly interactive **React-based** 4D Julia fractal viewer with **Three.js** integration and real-time parameter controls.

## 🚀 **Quick Start**

```bash
# Install dependencies
npm install

# Start both backend and frontend
npm start

# Or run separately:
npm run server  # Backend on port 3005
npm run dev     # Frontend on port 3000
```

**Access:** http://localhost:3000

## ✨ **Why React?**

Switched from vanilla JavaScript to **React** for maximum stability and reliability:

- ✅ **Component-based architecture** - Better organization and maintainability
- ✅ **State management** - Reliable parameter synchronization
- ✅ **React Three Fiber** - Industry-standard Three.js integration
- ✅ **Proven ecosystem** - Battle-tested libraries and patterns
- ✅ **Automatic re-rendering** - No manual DOM manipulation bugs
- ✅ **Developer tools** - Superior debugging and development experience

## 🎯 **Features**

### **🎮 Fully Functional Controls**
- **Iterations Slider** (1-20): Real-time fractal detail adjustment
- **Grid Resolution** (16-64): Quality vs performance control
- **4D Quaternion Controls**: Complete a + bi + cj + dk parameter control
- **Wireframe/Solid Toggle**: Instant rendering mode switching
- **Auto-Rotation**: Smooth automatic model rotation
- **Preset Configurations**: Default, Complex, and 4D presets
- **Performance Stats**: Live triangle/vertex count display

### **🖱️ Interactive 3D Navigation**
- **Mouse Controls**: Rotate, pan, zoom with OrbitControls
- **Smooth Animations**: 60fps rendering with automatic optimization
- **Responsive Design**: Works on desktop, tablet, and mobile
- **Auto-Centering**: Fractals automatically center in viewport

### **⚡ High-Performance Backend**
- **Optimized Algorithm**: True 4D Julia set computation
- **Marching Cubes**: Proper isosurface extraction
- **Real-time Generation**: Fast server-side processing
- **Memory Efficient**: Optimized for large datasets

## 🏗️ **Architecture**

```
react-fractal-viewer/
├── server.js              # Express backend (port 3005)
├── src/
│   ├── App.jsx            # Main React application
│   ├── App.css            # Modern styling
│   └── components/
│       ├── FractalMesh.jsx    # Three.js mesh component
│       ├── ControlPanel.jsx   # Interactive GUI controls
│       └── LoadingOverlay.jsx # Loading state component
├── package.json           # Dependencies and scripts
└── vite.config.js        # Development server config
```

## 🔧 **Technology Stack**

### **Frontend**
- **React 18** - Component framework
- **Vite** - Fast development server
- **Three.js** - 3D graphics engine
- **@react-three/fiber** - React Three.js integration
- **@react-three/drei** - Three.js helpers and controls

### **Backend**
- **Node.js** - Runtime environment
- **Express** - Web framework
- **CORS** - Cross-origin resource sharing

## 🎛️ **Controls Guide**

### **Fractal Parameters**
- **Iterations**: Higher values = more detail, slower generation
- **Grid Resolution**: Higher values = smoother surfaces, more triangles
- **Quaternion (c)**: The complex constant in Julia iteration q = q² + c
  - **Real (a)**: Real component
  - **i (b)**: First imaginary component
  - **j (c)**: Second imaginary component
  - **k (d)**: Third imaginary component

### **Rendering Options**
- **Wireframe Mode**: Shows triangle edges (faster)
- **Solid Mode**: Filled surfaces with lighting (prettier)
- **Auto Rotate**: Continuous rotation animation

### **Mouse Controls**
- **Left Click + Drag**: Rotate the fractal
- **Right Click + Drag**: Pan the view
- **Scroll Wheel**: Zoom in/out
- **Double Click**: Reset view

## 🎭 **Preset Configurations**

1. **Default**: `c = (0, 0, 0, 0)` - Classic Julia set
2. **Complex**: `c = (-0.4, 0.6, 0, 0)` - Complex plane fractal
3. **4D**: `c = (0.18, 0, 0, 0.78)` - True quaternion fractal

## 📊 **API Endpoints**

### `POST /api/fractal`
Generate fractal data with custom parameters.

**Request:**
```json
{
  "iterations": 6,
  "c": [0, 0, 0, 0],
  "gridSize": 32
}
```

**Response:**
```json
{
  "vertices": [...],
  "indices": [...],
  "triangleCount": 2840,
  "vertexCount": 8520
}
```

## 🚀 **Development**

```bash
# Install dependencies
npm install

# Development mode (auto-reload)
npm run dev

# Production build
npm run build

# Run backend only
npm run server

# Lint code
npm run lint
```

## 🎯 **Usage Tips**

1. **Start with Default preset** to see basic Julia set
2. **Adjust iterations** between 6-12 for good balance
3. **Use wireframe mode** for faster interaction while tweaking parameters
4. **Try Complex preset** for interesting 2D-like fractals
5. **Use 4D preset** to see true quaternion fractals
6. **Enable auto-rotate** for presentations and screenshots

## 🌟 **What Makes This Special**

1. **React Architecture**: Professional, maintainable codebase
2. **Real-time Interaction**: All parameters update instantly
3. **True 4D Mathematics**: Actual quaternion-based Julia sets
4. **Modern UI/UX**: Intuitive, responsive interface
5. **Performance Optimized**: Efficient rendering and computation
6. **Cross-platform**: Works on any modern browser

## 🔍 **Troubleshooting**

### **Common Issues**

**Backend not starting:**
```bash
# Check if port 3005 is available
lsof -i :3005
# Kill existing process if needed
kill -9 <PID>
```

**Frontend not loading:**
```bash
# Check if port 3000 is available
lsof -i :3000
# Try different port
npm run dev -- --port 3001
```

**No fractal visible:**
- Check browser console for errors
- Ensure WebGL is supported
- Try different parameter values
- Check network tab for API responses

## 🎉 **Success!**

You now have a **rock-solid, React-based** 4D Julia fractal viewer that's:
- ✅ **Stable and reliable** with React's proven architecture
- ✅ **Fully interactive** with real-time parameter controls
- ✅ **High performance** with optimized rendering
- ✅ **Professional grade** with modern development practices

**Enjoy exploring the infinite beauty of 4D Julia fractals!** 🌀✨
