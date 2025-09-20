#!/bin/bash

# 🌌 Morphosis Web App Starter Script
# This script starts the complete web application with all backend calculations

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${PURPLE}================================${NC}"
    echo -e "${PURPLE}🌌 Morphosis Web App Starter${NC}"
    echo -e "${PURPLE}================================${NC}"
}

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if a port is in use
port_in_use() {
    lsof -i :$1 >/dev/null 2>&1
}

# Function to kill processes on specific ports
kill_port() {
    local port=$1
    if port_in_use $port; then
        print_warning "Port $port is in use. Attempting to free it..."
        lsof -ti :$port | xargs kill -9 2>/dev/null || true
        sleep 2
    fi
}

# Function to check dependencies
check_dependencies() {
    print_status "Checking dependencies..."

    local missing_deps=()

    # Check for Node.js
    if ! command_exists node; then
        missing_deps+=("Node.js")
    else
        local node_version=$(node --version)
        print_success "Node.js found: $node_version"
    fi

    # Check for npm
    if ! command_exists npm; then
        missing_deps+=("npm")
    else
        local npm_version=$(npm --version)
        print_success "npm found: $npm_version"
    fi

    # Check for make
    if ! command_exists make; then
        missing_deps+=("make")
    else
        print_success "make found"
    fi

    # Check for clang
    if ! command_exists clang; then
        missing_deps+=("clang")
    else
        print_success "clang found"
    fi

    # Check for required libraries
    if ! command_exists brew; then
        print_warning "Homebrew not found. Some libraries might be missing."
    else
        # Check for OpenGL libraries
        if brew list glfw >/dev/null 2>&1; then
            print_success "GLFW library found"
        else
            missing_deps+=("glfw (brew install glfw)")
        fi

        if brew list glew >/dev/null 2>&1; then
            print_success "GLEW library found"
        else
            missing_deps+=("glew (brew install glew)")
        fi

        if brew list cglm >/dev/null 2>&1; then
            print_success "CGLM library found"
        else
            missing_deps+=("cglm (brew install cglm)")
        fi
    fi

    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "Missing dependencies:"
        for dep in "${missing_deps[@]}"; do
            echo "  - $dep"
        done
        echo ""
        print_status "To install missing dependencies:"
        echo "  brew install glfw glew cglm"
        echo "  # For Node.js, visit: https://nodejs.org/"
        echo ""
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

# Function to build the C++ backend
build_backend() {
    print_status "Building C++ backend..."

    # Check if morphosis executable exists
    if [ -f "./morphosis" ] && [ -f "./morphosis_optimized" ]; then
        print_success "C++ executables already exist"
        return 0
    fi

    # Build the project
    print_status "Running make to build the project..."
    if make; then
        print_success "C++ backend built successfully"
    else
        print_error "Failed to build C++ backend"
        print_status "Trying to build optimized version..."
        if make morphosis_optimized; then
            print_success "Optimized C++ backend built successfully"
        else
            print_error "Failed to build C++ backend. Please check dependencies."
            exit 1
        fi
    fi
}

# Function to generate initial fractal data
generate_initial_data() {
    print_status "Generating initial fractal data..."

    # Check if fractal_data.json exists
    if [ -f "./fractal_data.json" ]; then
        print_success "Initial fractal data already exists"
        return 0
    fi

    # Generate fractal data using the standard version (optimized version doesn't support -j flag)
    if [ -f "./morphosis" ]; then
        print_status "Using standard version to generate fractal data..."
        ./morphosis -j 0.05 -0.2 0.8 0.0 0.0 6
    else
        print_warning "No C++ executable found. Web app will use fallback generation."
        return 0
    fi

    if [ -f "./fractal_data.json" ]; then
        print_success "Initial fractal data generated successfully"
    else
        print_warning "Failed to generate initial fractal data. Web app will use fallback generation."
    fi
}

# Function to install Node.js dependencies
install_node_deps() {
    print_status "Installing Node.js dependencies..."

    cd react-fractal-viewer

    # Check if node_modules exists
    if [ -d "node_modules" ]; then
        print_success "Node.js dependencies already installed"
    else
        print_status "Running npm install..."
        if npm install; then
            print_success "Node.js dependencies installed successfully"
        else
            print_error "Failed to install Node.js dependencies"
            exit 1
        fi
    fi

    cd ..
}

# Function to start the web application
start_web_app() {
    print_status "Starting web application..."

    # Kill any existing processes on the ports we need
    kill_port 3005  # Backend server port
    kill_port 3000  # Vite dev server port

    cd react-fractal-viewer

    print_status "Starting backend server and frontend development server..."
    print_status "Backend will run on: http://localhost:3005"
    print_status "Frontend will run on: http://localhost:3000"
    print_status ""
    print_status "Press Ctrl+C to stop both servers"
    print_status ""

    # Start both servers concurrently
    npm run start
}

# Function to show usage information
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help     Show this help message"
    echo "  -c, --check    Only check dependencies (don't start the app)"
    echo "  -b, --build    Only build the backend (don't start the app)"
    echo "  -d, --data     Only generate initial data (don't start the app)"
    echo "  --no-check     Skip dependency checking"
    echo "  --no-build     Skip backend building"
    echo "  --no-data      Skip initial data generation"
    echo ""
    echo "Examples:"
    echo "  $0                    # Start the complete web app"
    echo "  $0 --check            # Only check dependencies"
    echo "  $0 --no-build         # Skip building, just start the app"
    echo ""
}

# Main function
main() {
    local check_deps=true
    local build_backend=true
    local generate_data=true
    local start_app=true

    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_usage
                exit 0
                ;;
            -c|--check)
                check_deps=true
                build_backend=false
                generate_data=false
                start_app=false
                shift
                ;;
            -b|--build)
                check_deps=true
                build_backend=true
                generate_data=false
                start_app=false
                shift
                ;;
            -d|--data)
                check_deps=true
                build_backend=true
                generate_data=true
                start_app=false
                shift
                ;;
            --no-check)
                check_deps=false
                shift
                ;;
            --no-build)
                build_backend=false
                shift
                ;;
            --no-data)
                generate_data=false
                shift
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done

    print_header

    # Check if we're in the right directory
    if [ ! -f "Makefile" ] || [ ! -d "react-fractal-viewer" ]; then
        print_error "Please run this script from the project root directory"
        print_status "Expected to find: Makefile and react-fractal-viewer/ directory"
        exit 1
    fi

    # Execute steps based on flags
    if [ "$check_deps" = true ]; then
        check_dependencies
    fi

    if [ "$build_backend" = true ]; then
        build_backend
    fi

    if [ "$generate_data" = true ]; then
        generate_initial_data
    fi

    if [ "$start_app" = true ]; then
        install_node_deps
        start_web_app
    else
        print_success "Setup completed successfully!"
        print_status "Run '$0' to start the web application"
    fi
}

# Run main function with all arguments
main "$@"
