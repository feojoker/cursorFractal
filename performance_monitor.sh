#!/bin/bash

# Real-time Performance Monitoring Script for Morphosis Optimizations
# This script measures and logs performance metrics with detailed analysis

echo "🎯 Morphosis Performance Monitor"
echo "==============================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
LOG_FILE="performance_results.log"
CSV_FILE="performance_data.csv"
TEST_GRID_SIZES=(20 25 30 35 40)
TEST_STEP_SIZES=(0.2 0.15 0.1 0.08 0.05)
TEST_ITERATIONS=(3 4 5 6 7)

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

print_result() {
    echo -e "${PURPLE}[RESULT]${NC} $1"
}

# Function to measure execution time
measure_time() {
    local command="$1"
    local output_file="$2"

    # Use time command with format
    /usr/bin/time -f "real %e\nuser %U\nsys %S\nmax_memory %M" -o "$output_file" $command
}

# Function to extract time from time output
extract_time() {
    local time_file="$1"
    local field="$2"  # real, user, sys, max_memory

    grep "^$field" "$time_file" | awk '{print $2}'
}

# Function to run performance test
run_performance_test() {
    local grid_size="$1"
    local step_size="$2"
    local iterations="$3"
    local version="$4"  # original or optimized
    local test_name="${grid_size}x${grid_size}x${grid_size}_${step_size}_${iterations}_${version}"

    print_status "Testing: $test_name"

    # Create temporary files
    local time_file="/tmp/morphosis_time_${test_name}.txt"
    local output_file="/tmp/morphosis_output_${test_name}.txt"

    # Build command
    local executable="./morphosis"
    if [ "$version" = "optimized" ]; then
        executable="./morphosis_optimized"
    fi

    # Check if executable exists
    if [ ! -f "$executable" ]; then
        print_error "Executable $executable not found!"
        return 1
    fi

    # Run the test
    echo "$iterations" | measure_time "$executable $step_size -0.2 0.8 0.0 0.0" "$time_file" > "$output_file" 2>&1

    # Extract metrics
    local real_time=$(extract_time "$time_file" "real")
    local user_time=$(extract_time "$time_file" "user")
    local sys_time=$(extract_time "$time_file" "sys")
    local max_memory=$(extract_time "$time_file" "max_memory")

    # Count triangles from output (if available)
    local triangle_count=$(grep -o "Generated [0-9]* triangles" "$output_file" | grep -o "[0-9]*" | tail -1)
    if [ -z "$triangle_count" ]; then
        triangle_count="0"
    fi

    # Log results
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    echo "$timestamp,$test_name,$real_time,$user_time,$sys_time,$max_memory,$triangle_count" >> "$CSV_FILE"

    # Print results
    print_result "Real Time: ${real_time}s, User Time: ${user_time}s, Memory: ${max_memory}KB, Triangles: ${triangle_count}"

    # Cleanup
    rm -f "$time_file" "$output_file"

    return 0
}

# Function to calculate and display efficiency metrics
calculate_efficiency() {
    local csv_file="$1"

    print_status "Calculating efficiency metrics..."

    # Create efficiency report
    local report_file="efficiency_report.txt"

    echo "📊 MORPHOSIS OPTIMIZATION EFFICIENCY REPORT" > "$report_file"
    echo "===========================================" >> "$report_file"
    echo "Generated: $(date)" >> "$report_file"
    echo "" >> "$report_file"

    # Process CSV data to calculate improvements
    local total_tests=0
    local total_speedup=0
    local total_memory_improvement=0

    # Group tests by configuration
    for grid_size in "${TEST_GRID_SIZES[@]}"; do
        for step_size in "${TEST_STEP_SIZES[@]}"; do
            for iterations in "${TEST_ITERATIONS[@]}"; do
                local test_base="${grid_size}x${grid_size}x${grid_size}_${step_size}_${iterations}"

                # Get original and optimized results
                local original_line=$(grep "${test_base}_original" "$csv_file" | tail -1)
                local optimized_line=$(grep "${test_base}_optimized" "$csv_file" | tail -1)

                if [ -n "$original_line" ] && [ -n "$optimized_line" ]; then
                    # Extract times
                    local orig_time=$(echo "$original_line" | cut -d',' -f3)
                    local opt_time=$(echo "$optimized_line" | cut -d',' -f3)
                    local orig_memory=$(echo "$original_line" | cut -d',' -f6)
                    local opt_memory=$(echo "$optimized_line" | cut -d',' -f6)

                    # Calculate improvements
                    local speedup=$(echo "scale=2; $orig_time / $opt_time" | bc -l 2>/dev/null || echo "1.0")
                    local memory_improvement=$(echo "scale=2; $orig_memory / $opt_memory" | bc -l 2>/dev/null || echo "1.0")

                    # Add to totals
                    total_tests=$((total_tests + 1))
                    total_speedup=$(echo "scale=2; $total_speedup + $speedup" | bc -l 2>/dev/null || echo "$total_speedup")
                    total_memory_improvement=$(echo "scale=2; $total_memory_improvement + $memory_improvement" | bc -l 2>/dev/null || echo "$total_memory_improvement")

                    # Add to report
                    echo "Test: $test_base" >> "$report_file"
                    echo "  Original Time: ${orig_time}s" >> "$report_file"
                    echo "  Optimized Time: ${opt_time}s" >> "$report_file"
                    echo "  Speedup: ${speedup}x" >> "$report_file"
                    echo "  Memory Improvement: ${memory_improvement}x" >> "$report_file"
                    echo "" >> "$report_file"
                fi
            done
        done
    done

    # Calculate averages
    if [ $total_tests -gt 0 ]; then
        local avg_speedup=$(echo "scale=2; $total_speedup / $total_tests" | bc -l 2>/dev/null || echo "1.0")
        local avg_memory=$(echo "scale=2; $total_memory_improvement / $total_tests" | bc -l 2>/dev/null || echo "1.0")

        echo "SUMMARY STATISTICS:" >> "$report_file"
        echo "===================" >> "$report_file"
        echo "Total Tests: $total_tests" >> "$report_file"
        echo "Average Speedup: ${avg_speedup}x" >> "$report_file"
        echo "Average Memory Improvement: ${avg_memory}x" >> "$report_file"
        echo "Overall Efficiency Gain: $(echo "scale=1; ($avg_speedup - 1) * 100" | bc -l 2>/dev/null || echo "0")%" >> "$report_file"
    fi

    print_success "Efficiency report generated: $report_file"
}

# Function to run comprehensive benchmark
run_comprehensive_benchmark() {
    print_status "Starting comprehensive performance benchmark..."

    # Initialize CSV file
    echo "timestamp,test_name,real_time,user_time,sys_time,max_memory_kb,triangle_count" > "$CSV_FILE"

    local test_count=0
    local total_tests=$((${#TEST_GRID_SIZES[@]} * ${#TEST_STEP_SIZES[@]} * ${#TEST_ITERATIONS[@]} * 2))

    for grid_size in "${TEST_GRID_SIZES[@]}"; do
        for step_size in "${TEST_STEP_SIZES[@]}"; do
            for iterations in "${TEST_ITERATIONS[@]}"; do
                # Test original version
                run_performance_test "$grid_size" "$step_size" "$iterations" "original"
                test_count=$((test_count + 1))
                print_status "Progress: $test_count/$total_tests tests completed"

                # Test optimized version
                run_performance_test "$grid_size" "$step_size" "$iterations" "optimized"
                test_count=$((test_count + 1))
                print_status "Progress: $test_count/$total_tests tests completed"

                # Small delay between tests
                sleep 1
            done
        done
    done

    print_success "Comprehensive benchmark completed!"
    calculate_efficiency "$CSV_FILE"
}

# Function to run quick benchmark
run_quick_benchmark() {
    print_status "Running quick performance benchmark..."

    # Initialize CSV file
    echo "timestamp,test_name,real_time,user_time,sys_time,max_memory_kb,triangle_count" > "$CSV_FILE"

    # Test with medium parameters
    local grid_size=25
    local step_size=0.15
    local iterations=4

    print_status "Testing with grid size: ${grid_size}x${grid_size}x${grid_size}, step: $step_size, iterations: $iterations"

    # Test original version
    run_performance_test "$grid_size" "$step_size" "$iterations" "original"

    # Test optimized version
    run_performance_test "$grid_size" "$step_size" "$iterations" "optimized"

    print_success "Quick benchmark completed!"
    calculate_efficiency "$CSV_FILE"
}

# Main execution
case "${1:-quick}" in
    "quick")
        run_quick_benchmark
        ;;
    "comprehensive")
        run_comprehensive_benchmark
        ;;
    "help"|"-h"|"--help")
        echo "Usage: $0 [quick|comprehensive|help]"
        echo "  quick         - Run quick benchmark (default)"
        echo "  comprehensive - Run full benchmark suite"
        echo "  help          - Show this help"
        ;;
    *)
        print_error "Unknown option: $1"
        echo "Use '$0 help' for usage information"
        exit 1
        ;;
esac

print_success "Performance monitoring completed!"
echo "Results saved to:"
echo "  - $CSV_FILE (raw data)"
echo "  - efficiency_report.txt (analysis)"
