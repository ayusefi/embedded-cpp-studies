# Build and Run Instructions

## Quick Start

### Option 1: Simple Demo (Recommended for beginners)
```bash
# Build and run the simple demo
make run-simple
```

### Option 2: Full Demonstration (Shows all three versions)
```bash
# Build and run the complete demonstration
make run
```

## Build Commands

### Build All Programs
```bash
make all
# or simply
make
```

### Build Individual Programs
```bash
# Build the simple demo only
make simple_demo

# Build the full demonstration only
make producer_consumer
```

### Manual Compilation
```bash
# Simple demo
g++ -std=c++11 -pthread -o simple_demo simple_demo.cpp

# Full demonstration
g++ -std=c++11 -pthread -o producer_consumer producer_consumer.cpp
```

## Running the Programs

### Simple Demo
The simple demo shows a clean implementation of Producer-Consumer using condition variables:
```bash
./simple_demo
```

**Expected output**: Shows one producer and two consumers working together efficiently.

### Full Demonstration
The full demonstration shows three different implementations:
```bash
./producer_consumer
```

**Expected output**: 
1. Buggy version (race conditions)
2. Mutex-only version (busy-waiting)
3. Efficient version (condition variables)

**Note**: The full demonstration takes longer to run as it shows timing differences between approaches.

## Development Commands

### Debug Build
```bash
make debug
```

### Clean Build Artifacts
```bash
make clean
```

### View Available Commands
```bash
make help
```

## Requirements

### System Requirements
- Linux/Unix system with POSIX threads
- C++11 compatible compiler (g++ 4.9+ or clang++ 3.5+)
- Make utility

### Compiler Flags Explained
- `-std=c++11`: Use C++11 standard (required for std::thread, std::mutex, etc.)
- `-pthread`: Link with POSIX threads library
- `-Wall -Wextra`: Enable comprehensive warnings
- `-O2`: Optimization level 2 for better performance

## Testing Different Configurations

### Modify Program Parameters
Edit these constants in the source files to experiment:

**In `simple_demo.cpp`:**
```cpp
const int NUM_ITEMS = 10;        // Number of items to produce
const int NUM_CONSUMERS = 2;     // Number of consumer threads
```

**In `producer_consumer.cpp`:**
```cpp
const int NUM_ITEMS = 50;        // Number of items to produce
const int BUFFER_SIZE = 10;      // Maximum buffer capacity
```

### Performance Testing
To measure performance differences:
```bash
# Time the simple demo
time ./simple_demo

# Time the full demonstration
time ./producer_consumer
```

## Troubleshooting

### Common Build Issues

**Error: pthread library not found**
```bash
# Solution: Ensure -pthread flag is used
g++ -pthread your_file.cpp
```

**Error: C++11 features not recognized**
```bash
# Solution: Add -std=c++11 flag
g++ -std=c++11 your_file.cpp
```

**Error: mutex/condition_variable not found**
```bash
# Solution: Ensure you have a C++11 compatible compiler
g++ --version  # Check compiler version
```

### Runtime Issues

**Program hangs or deadlocks**
- Check that condition variables are properly notified
- Ensure mutexes are properly released
- Verify that termination conditions are correct

**Race condition effects (in buggy version)**
- Inconsistent output order
- Missing or duplicated items
- Buffer size inconsistencies

**High CPU usage (in mutex-only version)**
- This is expected due to busy-waiting
- Compare with efficient version using condition variables

## Advanced Usage

### Multiple Producers and Consumers
Modify the simple demo to use multiple producers:
```cpp
const int NUM_PRODUCERS = 3;
const int NUM_CONSUMERS = 2;

// Create multiple producer threads
std::vector<std::thread> producer_threads;
for (int i = 0; i < NUM_PRODUCERS; ++i) {
    producer_threads.emplace_back(producer, std::ref(queue), NUM_ITEMS / NUM_PRODUCERS);
}
```

### Performance Profiling
Use profiling tools to analyze performance:
```bash
# Using time command
time ./simple_demo

# Using perf (if available)
perf stat ./simple_demo

# Using valgrind for memory analysis
valgrind --tool=helgrind ./simple_demo
```

### Integration with ROS 2
This foundation prepares you for ROS 2 development patterns:
```cpp
// Similar patterns in ROS 2 nodes
class ProducerNode : public rclcpp::Node {
    // Publisher acts like producer
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
};

class ConsumerNode : public rclcpp::Node {
    // Subscription callback acts like consumer
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
};
```

## Learning Objectives Checklist

After running these demos, you should understand:

- [ ] How race conditions occur and their effects
- [ ] Why mutexes alone aren't sufficient for efficiency
- [ ] How condition variables improve performance
- [ ] Thread-safe queue implementation patterns
- [ ] RAII principles with lock guards
- [ ] Producer-consumer coordination strategies

## Next Steps

1. **Experiment with parameters**: Change buffer sizes, timing, thread counts
2. **Add logging**: Implement detailed timing and performance measurements
3. **Extend functionality**: Add priority queues, different data types
4. **ROS 2 integration**: Apply these patterns in ROS 2 node development
5. **Real-time constraints**: Explore real-time scheduling considerations
