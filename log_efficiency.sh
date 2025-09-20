#!/bin/bash

# Simple Efficiency Logging Script
# Logs performance measurements to a CSV file for tracking over time

echo "📊 Morphosis Efficiency Logger"
echo "============================="

# Configuration
LOG_FILE="efficiency_log.csv"
GRID_SIZE=20
STEP_SIZE=0.15
ITERATIONS=4

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Function to measure execution time
measure_time() {
    local executable="$1"
    local version="$2"

    echo -e "${BLUE}Testing $version...${NC}"

    # Measure time using date command
    local start_time=$(date +%s.%N)

    # Run the program
    echo "$ITERATIONS" | timeout 60 "$executable" "$STEP_SIZE" -0.2 0.8 0.0 0.0 > /dev/null 2>&1
    local exit_code=$?

    local end_time=$(date +%s.%N)
    local execution_time=$(echo "$end_time - $start_time" | bc -l 2>/dev/null || echo "0")

    if [ $exit_code -eq 0 ] || [ $exit_code -eq 139 ]; then
        echo -e "${GREEN}✓ $version: ${execution_time}s${NC}"
    else
        echo -e "${YELLOW}⚠ $version: Failed (${execution_time}s)${NC}"
    fi

    echo "$execution_time"
}

# Initialize log file if it doesn't exist
if [ ! -f "$LOG_FILE" ]; then
    echo "timestamp,grid_size,step_size,iterations,original_time,optimized_time,speedup,improvement_percent" > "$LOG_FILE"
fi

# Check if executables exist
if [ ! -f "./morphosis" ]; then
    echo "Error: ./morphosis not found!"
    exit 1
fi

if [ ! -f "./morphosis_optimized" ]; then
    echo "Error: ./morphosis_optimized not found!"
    exit 1
fi

echo "Configuration: ${GRID_SIZE}x${GRID_SIZE}x${GRID_SIZE}, step $STEP_SIZE, $ITERATIONS iterations"
echo ""

# Measure both versions
original_time=$(measure_time "./morphosis" "Original")
optimized_time=$(measure_time "./morphosis_optimized" "Optimized")

# Calculate metrics
speedup=$(echo "scale=2; $original_time / $optimized_time" | bc -l 2>/dev/null || echo "1.0")
improvement_percent=$(echo "scale=1; ($speedup - 1) * 100" | bc -l 2>/dev/null || echo "0")

# Log results
timestamp=$(date '+%Y-%m-%d %H:%M:%S')
echo "$timestamp,$GRID_SIZE,$STEP_SIZE,$ITERATIONS,$original_time,$optimized_time,$speedup,$improvement_percent" >> "$LOG_FILE"

# Display results
echo ""
echo -e "${GREEN}📊 EFFICIENCY RESULTS:${NC}"
echo "========================"
echo "Original Time:    ${original_time}s"
echo "Optimized Time:   ${optimized_time}s"
echo "Speedup:          ${speedup}x faster"
echo "Improvement:      ${improvement_percent}%"
echo ""
echo -e "${BLUE}Results logged to: $LOG_FILE${NC}"

# Show recent history
echo ""
echo -e "${YELLOW}Recent Performance History:${NC}"
echo "=============================="
tail -5 "$LOG_FILE" | while IFS=',' read -r timestamp grid step iter orig opt speedup improvement; do
    if [ "$timestamp" != "timestamp" ]; then
        echo "$timestamp: ${speedup}x speedup (${improvement}% improvement)"
    fi
done
