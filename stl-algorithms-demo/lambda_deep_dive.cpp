#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>

// ===== LAMBDA EXPRESSIONS DEEP DIVE =====

void basicLambdaSyntax() {
    std::cout << "=== Basic Lambda Syntax ===\n\n";
    
    // 1. Simplest lambda
    auto simple = []() { std::cout << "Hello from lambda!\n"; };
    simple();
    
    // 2. Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;
    
    // 3. Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0.0;
        return a / b;
    };
    std::cout << "10.0 / 3.0 = " << divide(10.0, 3.0) << std::endl;
    
    // 4. Generic lambda (C++14)
    auto generic = [](auto x, auto y) { return x + y; };
    std::cout << "Generic lambda: " << generic(5, 10) << " and " 
              << generic(3.14, 2.86) << std::endl;
    
    std::cout << std::endl;
}

void lambdaCaptureTypes() {
    std::cout << "=== Lambda Capture Types ===\n\n";
    
    int x = 10;
    int y = 20;
    std::string message = "Hello";
    
    // 1. Capture by value
    auto capture_value = [x, y]() {
        return x + y; // x and y are copies
    };
    std::cout << "Capture by value: " << capture_value() << std::endl;
    
    // 2. Capture by reference
    auto capture_ref = [&x, &y]() {
        x += 5; // Modifying original x
        return x + y;
    };
    std::cout << "Before capture by ref: x=" << x << std::endl;
    std::cout << "Capture by reference: " << capture_ref() << std::endl;
    std::cout << "After capture by ref: x=" << x << std::endl;
    
    // 3. Capture all by value
    auto capture_all_value = [=]() {
        return x + y; // All variables captured by value
    };
    std::cout << "Capture all by value: " << capture_all_value() << std::endl;
    
    // 4. Capture all by reference
    auto capture_all_ref = [&]() {
        y += 10; // Modifying original y
        return x + y;
    };
    std::cout << "Before capture all by ref: y=" << y << std::endl;
    std::cout << "Capture all by reference: " << capture_all_ref() << std::endl;
    std::cout << "After capture all by ref: y=" << y << std::endl;
    
    // 5. Mixed capture
    auto mixed_capture = [=, &y](int z) {
        // x captured by value, y by reference, z is parameter
        y *= 2; // Modifying original y
        return x + y + z;
    };
    std::cout << "Mixed capture: " << mixed_capture(5) << std::endl;
    std::cout << "After mixed capture: y=" << y << std::endl;
    
    // 6. Init capture (C++14) - capture by move
    auto init_capture = [str = std::move(message)](const std::string& suffix) {
        return str + " " + suffix;
    };
    std::cout << "Init capture: " << init_capture("World!") << std::endl;
    std::cout << "Original message after move: '" << message << "'" << std::endl;
    
    std::cout << std::endl;
}

void lambdasWithSTLAlgorithms() {
    std::cout << "=== Lambdas with STL Algorithms ===\n\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 1. std::for_each with side effects
    std::cout << "Using for_each to print squares: ";
    std::for_each(numbers.begin(), numbers.end(), 
                  [](int n) { std::cout << n*n << " "; });
    std::cout << std::endl;
    
    // 2. std::transform with stateful lambda
    int multiplier = 3;
    std::vector<int> transformed;
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(transformed),
                   [multiplier](int n) { return n * multiplier; });
    
    std::cout << "Transformed (x3): ";
    for (int n : transformed) std::cout << n << " ";
    std::cout << std::endl;
    
    // 3. std::find_if with complex condition
    auto found_it = std::find_if(numbers.begin(), numbers.end(),
                                 [](int n) { 
                                     return n % 3 == 0 && n > 5; 
                                 });
    if (found_it != numbers.end()) {
        std::cout << "Found number divisible by 3 and > 5: " << *found_it << std::endl;
    }
    
    // 4. std::count_if with lambda
    auto even_count = std::count_if(numbers.begin(), numbers.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers count: " << even_count << std::endl;
    
    // 5. std::accumulate with lambda
    auto sum_of_squares = std::accumulate(numbers.begin(), numbers.end(), 0,
                                         [](int acc, int n) { 
                                             return acc + n * n; 
                                         });
    std::cout << "Sum of squares: " << sum_of_squares << std::endl;
    
    // 6. std::sort with custom comparator
    std::vector<int> to_sort = numbers;
    std::sort(to_sort.begin(), to_sort.end(),
              [](int a, int b) { 
                  return (a % 2) < (b % 2); // Even numbers first
              });
    
    std::cout << "Sorted (even first): ";
    for (int n : to_sort) std::cout << n << " ";
    std::cout << std::endl;
    
    std::cout << std::endl;
}

void advancedLambdaFeatures() {
    std::cout << "=== Advanced Lambda Features ===\n\n";
    
    // 1. Mutable lambda
    int counter = 0;
    auto mutable_lambda = [counter](int increment) mutable {
        counter += increment; // Can modify captured value copy
        return counter;
    };
    
    std::cout << "Mutable lambda calls: ";
    std::cout << mutable_lambda(5) << " ";
    std::cout << mutable_lambda(3) << " ";
    std::cout << mutable_lambda(2) << std::endl;
    std::cout << "Original counter unchanged: " << counter << std::endl;
    
    // 2. Lambda returning lambda (higher-order function)
    auto make_multiplier = [](int factor) {
        return [factor](int n) { return n * factor; };
    };
    
    auto times_3 = make_multiplier(3);
    auto times_5 = make_multiplier(5);
    
    std::cout << "Higher-order lambda: 7 * 3 = " << times_3(7) << std::endl;
    std::cout << "Higher-order lambda: 7 * 5 = " << times_5(7) << std::endl;
    
    // 3. Lambda with std::function
    std::function<int(int, int)> operation;
    
    char op = '+';
    switch (op) {
        case '+':
            operation = [](int a, int b) { return a + b; };
            break;
        case '*':
            operation = [](int a, int b) { return a * b; };
            break;
        default:
            operation = [](int a, int b) { return 0; };
    }
    
    std::cout << "Function wrapper: 6 " << op << " 4 = " << operation(6, 4) << std::endl;
    
    // 4. Recursive lambda (C++14 style)
    std::function<int(int)> factorial = [&factorial](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    
    std::cout << "Recursive lambda: 5! = " << factorial(5) << std::endl;
    
    // 5. IIFE (Immediately Invoked Function Expression)
    auto result = [](int x, int y) {
        auto temp = x * x + y * y;
        return temp > 100 ? temp : 0;
    }(7, 8);
    
    std::cout << "IIFE result: " << result << std::endl;
    
    std::cout << std::endl;
}

void embeddedLambdaBestPractices() {
    std::cout << "=== Embedded Systems Lambda Best Practices ===\n\n";
    
    std::vector<int> sensor_readings = {45, 78, 23, 89, 67, 34, 91, 56};
    
    // 1. Prefer capture by value for simple types (avoid dangling references)
    int threshold = 50;
    auto is_high_reading = [threshold](int reading) {
        return reading > threshold;
    };
    
    auto high_count = std::count_if(sensor_readings.begin(), sensor_readings.end(),
                                   is_high_reading);
    std::cout << "High readings (>" << threshold << "): " << high_count << std::endl;
    
    // 2. Use const for read-only captures
    const double conversion_factor = 3.14159;
    auto convert_reading = [conversion_factor](int raw) -> double {
        return raw * conversion_factor;
    };
    
    std::cout << "Converted reading: " << convert_reading(100) << std::endl;
    
    // 3. Avoid unnecessary captures for performance
    auto simple_filter = [](int value) { 
        return value >= 0 && value <= 100; // No captures needed
    };
    
    auto valid_count = std::count_if(sensor_readings.begin(), sensor_readings.end(),
                                    simple_filter);
    std::cout << "Valid readings: " << valid_count << std::endl;
    
    // 4. Use auto parameters for generic operations (C++14)
    auto max_element = [](const auto& container) {
        return *std::max_element(container.begin(), container.end());
    };
    
    std::cout << "Maximum reading: " << max_element(sensor_readings) << std::endl;
    
    // 5. Stateless lambdas can be converted to function pointers
    auto processor = [](int* data, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = data[i] > 100 ? 100 : data[i]; // Clamp to 100
        }
    };
    
    // This can be passed to C-style APIs
    void (*c_function_ptr)(int*, size_t) = processor;
    
    std::cout << "Lambda converted to function pointer successfully" << std::endl;
    
    std::cout << std::endl;
}

void performanceConsiderations() {
    std::cout << "=== Performance Considerations ===\n\n";
    
    std::vector<int> large_dataset(1000);
    std::iota(large_dataset.begin(), large_dataset.end(), 1);
    
    // 1. Capture by reference vs by value performance
    int expensive_computation_result = 42;
    
    // Good: capture by value for simple types
    auto lambda_by_value = [expensive_computation_result](int x) {
        return x + expensive_computation_result;
    };
    
    // Consider: capture by reference for expensive-to-copy types
    std::string expensive_string = "This could be a very long string...";
    auto lambda_by_ref = [&expensive_string](int x) {
        return expensive_string.length() + x;
    };
    
    std::cout << "Lambda performance patterns demonstrated" << std::endl;
    
    // 2. Template vs std::function performance
    auto template_lambda = [](auto x) { return x * 2; };
    std::function<int(int)> function_wrapper = [](int x) { return x * 2; };
    
    // template_lambda will typically be faster due to no type erasure
    
    // 3. Inline lambda vs stored lambda
    // Inline (fast)
    auto result1 = std::count_if(large_dataset.begin(), large_dataset.end(),
                                [](int x) { return x % 2 == 0; });
    
    // Stored (potentially slower due to indirect call)
    auto stored_lambda = [](int x) { return x % 2 == 0; };
    auto result2 = std::count_if(large_dataset.begin(), large_dataset.end(),
                                stored_lambda);
    
    std::cout << "Even count (inline): " << result1 << std::endl;
    std::cout << "Even count (stored): " << result2 << std::endl;
    
    std::cout << std::endl;
}

void realWorldExamples() {
    std::cout << "=== Real-World Embedded Examples ===\n\n";
    
    // Example 1: Sensor data filtering and processing
    struct SensorReading {
        int id;
        double value;
        bool is_valid;
    };
    
    std::vector<SensorReading> sensors = {
        {1, 23.5, true}, {2, 45.2, true}, {3, -999, false},
        {4, 67.8, true}, {5, 89.1, true}, {6, 12.3, false}
    };
    
    // Filter valid sensors and convert to Celsius
    std::vector<double> celsius_readings;
    std::for_each(sensors.begin(), sensors.end(),
                  [&celsius_readings](const SensorReading& sensor) {
                      if (sensor.is_valid && sensor.value > 0) {
                          // Convert from Fahrenheit to Celsius
                          double celsius = (sensor.value - 32.0) * 5.0 / 9.0;
                          celsius_readings.push_back(celsius);
                      }
                  });
    
    std::cout << "Valid temperature readings in Celsius: ";
    for (double temp : celsius_readings) {
        std::cout << temp << "°C ";
    }
    std::cout << std::endl;
    
    // Example 2: Motor control state machine
    enum class MotorState { STOPPED, STARTING, RUNNING, STOPPING, ERROR };
    
    std::vector<MotorState> state_history = {
        MotorState::STOPPED, MotorState::STARTING, MotorState::RUNNING,
        MotorState::RUNNING, MotorState::STOPPING, MotorState::STOPPED
    };
    
    // Count how long motor was running
    auto running_count = std::count_if(state_history.begin(), state_history.end(),
                                      [](MotorState state) {
                                          return state == MotorState::RUNNING;
                                      });
    
    std::cout << "Motor was running for " << running_count << " time periods" << std::endl;
    
    // Example 3: GPIO pin management
    struct GPIOPin {
        int pin_number;
        bool state;
        double voltage;
    };
    
    std::vector<GPIOPin> pins = {
        {13, true, 3.3}, {14, false, 0.0}, {15, true, 3.3},
        {16, false, 0.1}, {17, true, 3.2}
    };
    
    // Find pins that are logically high but voltage is suspicious
    auto suspicious_pin = std::find_if(pins.begin(), pins.end(),
                                      [](const GPIOPin& pin) {
                                          return pin.state == true && pin.voltage < 2.5;
                                      });
    
    if (suspicious_pin != pins.end()) {
        std::cout << "Suspicious pin found: " << suspicious_pin->pin_number 
                  << " (state: " << suspicious_pin->state 
                  << ", voltage: " << suspicious_pin->voltage << "V)" << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "C++ Lambda Expressions with STL Algorithms\n";
    std::cout << "==========================================\n\n";
    
    basicLambdaSyntax();
    std::cout << std::string(60, '-') << "\n\n";
    
    lambdaCaptureTypes();
    std::cout << std::string(60, '-') << "\n\n";
    
    lambdasWithSTLAlgorithms();
    std::cout << std::string(60, '-') << "\n\n";
    
    advancedLambdaFeatures();
    std::cout << std::string(60, '-') << "\n\n";
    
    embeddedLambdaBestPractices();
    std::cout << std::string(60, '-') << "\n\n";
    
    performanceConsiderations();
    std::cout << std::string(60, '-') << "\n\n";
    
    realWorldExamples();
    
    return 0;
}
