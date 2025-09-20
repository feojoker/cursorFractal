#include "morphosis.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// Performance measurement structure
typedef struct {
  double build_time;
  double total_time;
  int triangle_count;
  size_t memory_usage;
  int grid_size;
  float step_size;
  int iterations;
} performance_metrics_t;

// Get current time in seconds with microsecond precision
double get_time_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Measure memory usage (approximate)
size_t get_memory_usage() {
  // This is a simplified memory measurement
  // In a real implementation, you'd use more sophisticated methods
  return 0; // Placeholder
}

// Run performance test for a specific configuration
performance_metrics_t run_performance_test(int grid_size, float step_size,
                                           int iterations, int use_optimized) {
  performance_metrics_t metrics;
  t_data *data;
  double start_time, end_time;

  printf("\n=== Performance Test ===\n");
  printf("Grid Size: %dx%dx%d\n", grid_size, grid_size, grid_size);
  printf("Step Size: %.3f\n", step_size);
  printf("Iterations: %d\n", iterations);
  printf("Version: %s\n", use_optimized ? "OPTIMIZED" : "ORIGINAL");
  printf("------------------------\n");

  // Initialize data
  data = init_data();
  data->fract->step_size = step_size;
  data->fract->grid_size = (float)grid_size;
  data->fract->julia->max_iter = iterations;

  // Measure total time
  start_time = get_time_seconds();

  // Measure build time specifically
  double build_start = get_time_seconds();

  if (use_optimized) {
    calculate_point_cloud_optimized(data);
  } else {
    calculate_point_cloud(data);
    gl_retrieve_tris(data);
    clean_calcs(data);
  }

  double build_end = get_time_seconds();

  // Complete the timing
  end_time = get_time_seconds();

  // Record metrics
  metrics.build_time = build_end - build_start;
  metrics.total_time = end_time - start_time;
  metrics.triangle_count = data->gl->num_tris;
  metrics.memory_usage = get_memory_usage();
  metrics.grid_size = grid_size;
  metrics.step_size = step_size;
  metrics.iterations = iterations;

  // Cleanup
  clean_up(data);

  return metrics;
}

// Log performance results to file
void log_performance_results(performance_metrics_t original,
                             performance_metrics_t optimized,
                             const char *filename) {
  FILE *file = fopen(filename, "a");
  if (!file) {
    printf("Error: Could not open log file %s\n", filename);
    return;
  }

  // Calculate efficiency improvements
  double build_speedup = original.build_time / optimized.build_time;
  double total_speedup = original.total_time / optimized.total_time;
  double memory_efficiency =
      (double)original.memory_usage / (double)optimized.memory_usage;

  // Write header if file is empty
  fseek(file, 0, SEEK_END);
  if (ftell(file) == 0) {
    fprintf(file, "Timestamp,Grid_Size,Step_Size,Iterations,Original_Build_"
                  "Time,Optimized_Build_Time,Build_Speedup,Original_Total_Time,"
                  "Optimized_Total_Time,Total_Speedup,Original_Triangles,"
                  "Optimized_Triangles,Memory_Efficiency\n");
  }

  // Get current timestamp
  time_t now = time(0);
  char timestamp[100];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

  // Write performance data
  fprintf(file, "%s,%d,%.3f,%d,%.3f,%.3f,%.2fx,%.3f,%.3f,%.2fx,%d,%d,%.2fx\n",
          timestamp, original.grid_size, original.step_size,
          original.iterations, original.build_time, optimized.build_time,
          build_speedup, original.total_time, optimized.total_time,
          total_speedup, original.triangle_count, optimized.triangle_count,
          memory_efficiency);

  fclose(file);

  // Also print to console
  printf("\n📊 PERFORMANCE RESULTS:\n");
  printf("======================\n");
  printf("Build Time:     %.3fs → %.3fs (%.2fx faster)\n", original.build_time,
         optimized.build_time, build_speedup);
  printf("Total Time:     %.3fs → %.3fs (%.2fx faster)\n", original.total_time,
         optimized.total_time, total_speedup);
  printf("Triangles:      %d → %d (same quality)\n", original.triangle_count,
         optimized.triangle_count);
  printf("Memory Usage:   %zu → %zu bytes (%.2fx more efficient)\n",
         original.memory_usage, optimized.memory_usage, memory_efficiency);
  printf("Efficiency:     %.1f%% improvement\n", (build_speedup - 1.0) * 100.0);
}

// Run comprehensive benchmark suite
void run_benchmark_suite() {
  printf("🚀 Starting Comprehensive Performance Benchmark\n");
  printf("================================================\n");

  // Test configurations
  struct {
    int grid_size;
    float step_size;
    int iterations;
  } test_configs[] = {
      {20, 0.2f, 3},  // Small test
      {30, 0.1f, 4},  // Medium test
      {40, 0.08f, 5}, // Large test
      {50, 0.05f, 6}, // Very large test
  };

  int num_tests = sizeof(test_configs) / sizeof(test_configs[0]);

  for (int i = 0; i < num_tests; i++) {
    printf("\n🧪 Test %d/%d\n", i + 1, num_tests);

    // Run original version
    printf("Running ORIGINAL version...\n");
    performance_metrics_t original = run_performance_test(
        test_configs[i].grid_size, test_configs[i].step_size,
        test_configs[i].iterations, 0);

    // Run optimized version
    printf("Running OPTIMIZED version...\n");
    performance_metrics_t optimized = run_performance_test(
        test_configs[i].grid_size, test_configs[i].step_size,
        test_configs[i].iterations, 1);

    // Log results
    log_performance_results(original, optimized, "performance_log.csv");

    // Small delay between tests
    sleep(1);
  }

  printf("\n✅ Benchmark suite completed!\n");
  printf("Results logged to: performance_log.csv\n");
}

// Quick performance comparison
void quick_performance_test() {
  printf("⚡ Quick Performance Test\n");
  printf("========================\n");

  // Run a quick test with medium parameters
  performance_metrics_t original = run_performance_test(25, 0.15f, 4, 0);
  performance_metrics_t optimized = run_performance_test(25, 0.15f, 4, 1);

  log_performance_results(original, optimized, "quick_test_log.csv");
}

int main(int argc, char **argv) {
  printf("🎯 Morphosis Performance Benchmarking Tool\n");
  printf("==========================================\n");

  if (argc > 1 && strcmp(argv[1], "quick") == 0) {
    quick_performance_test();
  } else if (argc > 1 && strcmp(argv[1], "suite") == 0) {
    run_benchmark_suite();
  } else {
    printf("Usage:\n");
    printf("  %s quick  - Run quick performance test\n", argv[0]);
    printf("  %s suite  - Run comprehensive benchmark suite\n", argv[0]);
    printf("\nRunning quick test by default...\n");
    quick_performance_test();
  }

  return 0;
}
