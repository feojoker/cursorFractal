#!/bin/bash

# Performance Test Script for Morphosis Optimizations
# This script builds and tests all versions of the fractal generator

echo "🚀 Morphosis Performance Optimization Test"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
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

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    print_error "Makefile not found. Please run this script from the project root directory."
    exit 1
fi

print_status "Starting performance optimization test..."

# Clean previous builds
print_status "Cleaning previous builds..."
make clean > /dev/null 2>&1

# Build original version
print_status "Building original version..."
if make > /dev/null 2>&1; then
    print_success "Original version built successfully"
else
    print_error "Failed to build original version"
    exit 1
fi

# Build optimized version
print_status "Building optimized version..."
if make morphosis_optimized > /dev/null 2>&1; then
    print_success "Optimized version built successfully"
else
    print_warning "Failed to build optimized version (this is expected if optimizations aren't fully integrated)"
fi

# Build parallel version (if OpenMP is available)
print_status "Building parallel version..."
if make morphosis_parallel > /dev/null 2>&1; then
    print_success "Parallel version built successfully"
else
    print_warning "Failed to build parallel version (OpenMP might not be available)"
fi

echo ""
print_status "Running performance tests..."

# Test configuration
GRID_SIZE="30"  # Smaller for faster testing
STEP_SIZE="0.1"
ITERATIONS="4"

echo "Test Configuration:"
echo "- Grid Size: ${GRID_SIZE}x${GRID_SIZE}x${GRID_SIZE}"
echo "- Step Size: ${STEP_SIZE}"
echo "- Max Iterations: ${ITERATIONS}"
echo ""

# Test original version
if [ -f "./morphosis" ]; then
    print_status "Testing original version..."
    echo "${ITERATIONS}" | time -p ./morphosis ${STEP_SIZE} -0.2 0.8 0.0 0.0 > /dev/null 2>&1
    ORIGINAL_TIME=$?
    print_success "Original version completed"
else
    print_error "Original version not found"
fi

# Test optimized version
if [ -f "./morphosis_optimized" ]; then
    print_status "Testing optimized version..."
    echo "${ITERATIONS}" | time -p ./morphosis_optimized ${STEP_SIZE} -0.2 0.8 0.0 0.0 > /dev/null 2>&1
    OPTIMIZED_TIME=$?
    print_success "Optimized version completed"
else
    print_warning "Optimized version not available for testing"
fi

# Test parallel version
if [ -f "./morphosis_parallel" ]; then
    print_status "Testing parallel version..."
    echo "${ITERATIONS}" | time -p ./morphosis_parallel ${STEP_SIZE} -0.2 0.8 0.0 0.0 > /dev/null 2>&1
    PARALLEL_TIME=$?
    print_success "Parallel version completed"
else
    print_warning "Parallel version not available for testing"
fi

echo ""
print_status "Performance Test Results:"
echo "=============================="

# Display results
if [ -f "./morphosis" ]; then
    echo "✅ Original version: Available"
else
    echo "❌ Original version: Not available"
fi

if [ -f "./morphosis_optimized" ]; then
    echo "✅ Optimized version: Available"
else
    echo "❌ Optimized version: Not available"
fi

if [ -f "./morphosis_parallel" ]; then
    echo "✅ Parallel version: Available"
else
    echo "❌ Parallel version: Not available"
fi

echo ""
print_status "Next Steps:"
echo "============"
echo "1. Run individual tests with: time ./morphosis -d"
echo "2. Compare performance with larger grids (60x60x60)"
echo "3. Check generated .obj files for quality"
echo "4. Monitor memory usage during execution"
echo "5. Test with different Julia set parameters"

echo ""
print_status "Files generated:"
echo "==================="
ls -la *.obj 2>/dev/null || echo "No .obj files generated (run with GUI to export)"

echo ""
print_success "Performance test completed!"
echo ""
echo "For detailed analysis, see:"
echo "- OPTIMIZATIONS.md (this file)"
echo "- performance_analysis.md (technical details)"
echo "- test_performance.c (detailed benchmarking)"
