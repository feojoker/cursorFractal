#!/usr/bin/env python3
"""
Morphosis Performance Analysis Tool
Analyzes performance data and generates detailed efficiency reports
"""

import csv
import json
import statistics
import matplotlib.pyplot as plt
import pandas as pd
from datetime import datetime
import os
import sys

class PerformanceAnalyzer:
    def __init__(self, csv_file="performance_data.csv"):
        self.csv_file = csv_file
        self.data = []
        self.load_data()

    def load_data(self):
        """Load performance data from CSV file"""
        if not os.path.exists(self.csv_file):
            print(f"Error: {self.csv_file} not found!")
            return

        with open(self.csv_file, 'r') as file:
            reader = csv.DictReader(file)
            self.data = list(reader)

        print(f"Loaded {len(self.data)} performance records")

    def parse_test_name(self, test_name):
        """Parse test name to extract parameters"""
        parts = test_name.split('_')
        if len(parts) >= 4:
            grid_size = parts[0]
            step_size = float(parts[1])
            iterations = int(parts[2])
            version = parts[3]
            return grid_size, step_size, iterations, version
        return None, None, None, None

    def calculate_efficiency_metrics(self):
        """Calculate comprehensive efficiency metrics"""
        metrics = {
            'speedup_by_grid_size': {},
            'speedup_by_step_size': {},
            'speedup_by_iterations': {},
            'memory_efficiency': {},
            'overall_stats': {}
        }

        # Group data by test configuration
        test_groups = {}
        for record in self.data:
            grid_size, step_size, iterations, version = self.parse_test_name(record['test_name'])
            if grid_size:
                key = f"{grid_size}_{step_size}_{iterations}"
                if key not in test_groups:
                    test_groups[key] = {}
                test_groups[key][version] = record

        # Calculate speedups
        speedups = []
        memory_improvements = []

        for key, versions in test_groups.items():
            if 'original' in versions and 'optimized' in versions:
                orig = versions['original']
                opt = versions['optimized']

                # Calculate speedup
                orig_time = float(orig['real_time'])
                opt_time = float(opt['real_time'])
                speedup = orig_time / opt_time if opt_time > 0 else 1.0
                speedups.append(speedup)

                # Calculate memory improvement
                orig_memory = float(orig['max_memory_kb'])
                opt_memory = float(opt['max_memory_kb'])
                memory_improvement = orig_memory / opt_memory if opt_memory > 0 else 1.0
                memory_improvements.append(memory_improvement)

                # Parse parameters
                grid_size, step_size, iterations, _ = self.parse_test_name(orig['test_name'])

                # Group by parameters
                if grid_size not in metrics['speedup_by_grid_size']:
                    metrics['speedup_by_grid_size'][grid_size] = []
                metrics['speedup_by_grid_size'][grid_size].append(speedup)

                if step_size not in metrics['speedup_by_step_size']:
                    metrics['speedup_by_step_size'][step_size] = []
                metrics['speedup_by_step_size'][step_size].append(speedup)

                if iterations not in metrics['speedup_by_iterations']:
                    metrics['speedup_by_iterations'][iterations] = []
                metrics['speedup_by_iterations'][iterations].append(speedup)

        # Calculate overall statistics
        if speedups:
            metrics['overall_stats'] = {
                'average_speedup': statistics.mean(speedups),
                'median_speedup': statistics.median(speedups),
                'min_speedup': min(speedups),
                'max_speedup': max(speedups),
                'std_speedup': statistics.stdev(speedups) if len(speedups) > 1 else 0,
                'average_memory_improvement': statistics.mean(memory_improvements),
                'total_tests': len(speedups)
            }

        return metrics

    def generate_report(self):
        """Generate comprehensive performance report"""
        metrics = self.calculate_efficiency_metrics()

        report = f"""
🚀 MORPHOSIS OPTIMIZATION PERFORMANCE REPORT
==========================================
Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

📊 OVERALL PERFORMANCE METRICS
==============================
Total Tests Analyzed: {metrics['overall_stats'].get('total_tests', 0)}
Average Speedup: {metrics['overall_stats'].get('average_speedup', 0):.2f}x
Median Speedup: {metrics['overall_stats'].get('median_speedup', 0):.2f}x
Minimum Speedup: {metrics['overall_stats'].get('min_speedup', 0):.2f}x
Maximum Speedup: {metrics['overall_stats'].get('max_speedup', 0):.2f}x
Standard Deviation: {metrics['overall_stats'].get('std_speedup', 0):.2f}x
Average Memory Improvement: {metrics['overall_stats'].get('average_memory_improvement', 0):.2f}x

🎯 EFFICIENCY ANALYSIS
=====================
Overall Efficiency Gain: {(metrics['overall_stats'].get('average_speedup', 1) - 1) * 100:.1f}%
Performance Consistency: {'High' if metrics['overall_stats'].get('std_speedup', 0) < 0.5 else 'Medium' if metrics['overall_stats'].get('std_speedup', 0) < 1.0 else 'Low'}

📈 SPEEDUP BY GRID SIZE
======================
"""

        for grid_size in sorted(metrics['speedup_by_grid_size'].keys()):
            speedups = metrics['speedup_by_grid_size'][grid_size]
            avg_speedup = statistics.mean(speedups)
            report += f"{grid_size}: {avg_speedup:.2f}x average speedup ({len(speedups)} tests)\n"

        report += f"""
📈 SPEEDUP BY STEP SIZE
=====================
"""

        for step_size in sorted(metrics['speedup_by_step_size'].keys()):
            speedups = metrics['speedup_by_step_size'][step_size]
            avg_speedup = statistics.mean(speedups)
            report += f"{step_size}: {avg_speedup:.2f}x average speedup ({len(speedups)} tests)\n"

        report += f"""
📈 SPEEDUP BY ITERATIONS
======================
"""

        for iterations in sorted(metrics['speedup_by_iterations'].keys()):
            speedups = metrics['speedup_by_iterations'][iterations]
            avg_speedup = statistics.mean(speedups)
            report += f"{iterations}: {avg_speedup:.2f}x average speedup ({len(speedups)} tests)\n"

        report += f"""
💡 OPTIMIZATION INSIGHTS
======================
"""

        # Add insights based on data
        avg_speedup = metrics['overall_stats'].get('average_speedup', 1)
        if avg_speedup > 5:
            report += "🔥 EXCELLENT: Optimizations provide significant performance improvements!\n"
        elif avg_speedup > 2:
            report += "✅ GOOD: Optimizations provide noticeable performance improvements.\n"
        elif avg_speedup > 1.5:
            report += "👍 MODERATE: Optimizations provide modest performance improvements.\n"
        else:
            report += "⚠️  LIMITED: Optimizations provide minimal performance improvements.\n"

        # Memory efficiency insights
        memory_improvement = metrics['overall_stats'].get('average_memory_improvement', 1)
        if memory_improvement > 1.5:
            report += "💾 MEMORY: Significant memory efficiency improvements achieved.\n"
        elif memory_improvement > 1.1:
            report += "💾 MEMORY: Modest memory efficiency improvements achieved.\n"
        else:
            report += "💾 MEMORY: Limited memory efficiency improvements.\n"

        return report

    def save_report(self, filename="performance_analysis_report.txt"):
        """Save performance report to file"""
        report = self.generate_report()
        with open(filename, 'w') as file:
            file.write(report)
        print(f"Performance report saved to: {filename}")
        return report

    def create_visualizations(self):
        """Create performance visualization charts"""
        if not self.data:
            print("No data available for visualization")
            return

        # Create DataFrame for easier analysis
        df = pd.DataFrame(self.data)
        df['real_time'] = pd.to_numeric(df['real_time'])
        df['max_memory_kb'] = pd.to_numeric(df['max_memory_kb'])

        # Parse test parameters
        test_params = df['test_name'].str.split('_', expand=True)
        df['grid_size'] = test_params[0]
        df['step_size'] = pd.to_numeric(test_params[1])
        df['iterations'] = pd.to_numeric(test_params[2])
        df['version'] = test_params[3]

        # Create subplots
        fig, axes = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle('Morphosis Performance Analysis', fontsize=16)

        # 1. Execution Time Comparison
        ax1 = axes[0, 0]
        orig_times = df[df['version'] == 'original']['real_time']
        opt_times = df[df['version'] == 'optimized']['real_time']
        ax1.hist([orig_times, opt_times], bins=20, alpha=0.7, label=['Original', 'Optimized'])
        ax1.set_xlabel('Execution Time (seconds)')
        ax1.set_ylabel('Frequency')
        ax1.set_title('Execution Time Distribution')
        ax1.legend()

        # 2. Speedup by Grid Size
        ax2 = axes[0, 1]
        speedup_data = []
        grid_sizes = []
        for grid_size in df['grid_size'].unique():
            orig = df[(df['grid_size'] == grid_size) & (df['version'] == 'original')]['real_time'].mean()
            opt = df[(df['grid_size'] == grid_size) & (df['version'] == 'optimized')]['real_time'].mean()
            if orig > 0 and opt > 0:
                speedup_data.append(orig / opt)
                grid_sizes.append(grid_size)
        ax2.bar(grid_sizes, speedup_data)
        ax2.set_xlabel('Grid Size')
        ax2.set_ylabel('Speedup (x)')
        ax2.set_title('Speedup by Grid Size')
        ax2.tick_params(axis='x', rotation=45)

        # 3. Memory Usage Comparison
        ax3 = axes[1, 0]
        orig_memory = df[df['version'] == 'original']['max_memory_kb']
        opt_memory = df[df['version'] == 'optimized']['max_memory_kb']
        ax3.hist([orig_memory, opt_memory], bins=20, alpha=0.7, label=['Original', 'Optimized'])
        ax3.set_xlabel('Memory Usage (KB)')
        ax3.set_ylabel('Frequency')
        ax3.set_title('Memory Usage Distribution')
        ax3.legend()

        # 4. Performance Improvement Percentage
        ax4 = axes[1, 1]
        improvements = []
        for i in range(len(speedup_data)):
            improvement = (speedup_data[i] - 1) * 100
            improvements.append(improvement)
        ax4.bar(grid_sizes, improvements, color='green', alpha=0.7)
        ax4.set_xlabel('Grid Size')
        ax4.set_ylabel('Performance Improvement (%)')
        ax4.set_title('Performance Improvement by Grid Size')
        ax4.tick_params(axis='x', rotation=45)

        plt.tight_layout()
        plt.savefig('performance_analysis.png', dpi=300, bbox_inches='tight')
        print("Performance visualization saved to: performance_analysis.png")
        plt.show()

def main():
    if len(sys.argv) > 1:
        csv_file = sys.argv[1]
    else:
        csv_file = "performance_data.csv"

    print("🎯 Morphosis Performance Analyzer")
    print("=================================")

    analyzer = PerformanceAnalyzer(csv_file)

    if not analyzer.data:
        print("No performance data found. Please run performance tests first.")
        return

    # Generate and display report
    report = analyzer.save_report()
    print("\n" + report)

    # Create visualizations
    try:
        analyzer.create_visualizations()
    except ImportError:
        print("Matplotlib not available. Skipping visualizations.")
    except Exception as e:
        print(f"Error creating visualizations: {e}")

if __name__ == "__main__":
    main()
