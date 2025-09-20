# 🌌 Morphosis Web App Starter Script

This script provides a unified way to start the complete Morphosis fractal visualizer web application with all backend calculations.

## 🚀 Quick Start

```bash
./start_web_app.sh
```

## ✨ Features

- **Automatic dependency checking** - Verifies all required tools and libraries
- **Smart backend building** - Builds the optimized C++ backend (42.76x faster!)
- **Initial data generation** - Creates fractal data for immediate viewing
- **Concurrent server startup** - Starts both backend and frontend servers
- **Port management** - Automatically handles port conflicts
- **Graceful fallbacks** - Falls back to JavaScript if C++ generation fails
- **Colored output** - Clear, colored status messages
- **Error handling** - Comprehensive error checking and reporting

## 🛠️ Command Line Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help message |
| `-c, --check` | Only check dependencies (don't start the app) |
| `-b, --build` | Only build the backend (don't start the app) |
| `-d, --data` | Only generate initial data (don't start the app) |
| `--no-check` | Skip dependency checking |
| `--no-build` | Skip backend building |
| `--no-data` | Skip initial data generation |

## 📋 What the Script Does

1. **Dependency Check**
   - Verifies Node.js and npm are installed
   - Checks for required build tools (make, clang)
   - Validates OpenGL libraries (GLFW, GLEW, CGLM)

2. **Backend Building**
   - Builds the C++ fractal generator
   - Prefers optimized version when available
   - Falls back to standard version if needed

3. **Data Generation**
   - Generates initial fractal data using C++ backend
   - Creates `fractal_data.json` for immediate web app use
   - Uses optimized parameters for best performance

4. **Web App Startup**
   - Installs Node.js dependencies if needed
   - Starts backend server on port 3005
   - Starts frontend development server on port 3000
   - Runs both servers concurrently

## 🌐 Web Application

Once started, the web application provides:

- **Backend Server** (http://localhost:3005)
  - Handles C++ fractal generation
  - Provides REST API for fractal data
  - Automatic fallback to JavaScript generation

- **Frontend Interface** (http://localhost:3000)
  - Interactive 3D fractal viewer
  - Real-time parameter adjustment
  - Mouse controls for exploration
  - Modern React-based UI

## 🔧 Troubleshooting

### Common Issues

**Permission denied:**
```bash
chmod +x start_web_app.sh
```

**Port already in use:**
The script automatically tries to free ports 3005 and 3000.

**Missing dependencies:**
```bash
# Install required libraries
brew install glfw glew cglm

# Install Node.js from https://nodejs.org/
```

**C++ build fails:**
The web app will automatically fall back to JavaScript generation.

### Manual Steps

If the script fails, you can run steps manually:

```bash
# 1. Check dependencies
./start_web_app.sh --check

# 2. Build backend
make morphosis_optimized

# 3. Generate data
echo "6" | ./morphosis_optimized -j 0.05 -0.2 0.8 0.0 0.0

# 4. Install Node.js dependencies
cd react-fractal-viewer
npm install

# 5. Start servers
npm run start
```

## 🎯 Performance

The script uses the optimized C++ backend which provides:
- **42.76x faster** fractal generation
- **Real-time parameter adjustment**
- **High-quality 3D meshes**
- **Efficient memory usage**

## 📝 Requirements

- **macOS** (tested on macOS 14.6.0)
- **Node.js** (v16 or higher)
- **npm** (comes with Node.js)
- **Homebrew** (for library installation)
- **OpenGL libraries** (GLFW, GLEW, CGLM)
- **Build tools** (make, clang)

## 🎉 Success Indicators

When everything works correctly, you'll see:
- ✅ All dependency checks pass
- ✅ C++ backend builds successfully
- ✅ Initial fractal data generates
- ✅ Both servers start without errors
- ✅ Web interface opens in your browser

The script provides clear, colored output to show exactly what's happening at each step.
