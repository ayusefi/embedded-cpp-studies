#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <chrono>
#include <string>
#include <random>

// ===== DATA STRUCTURES FOR DEMONSTRATIONS =====

struct Employee {
    std::string name;
    int age;
    double salary;
    std::string department;
    
    Employee(const std::string& n, int a, double s, const std::string& d)
        : name(n), age(a), salary(s), department(d) {}
    
    void display() const {
        std::cout << "Employee{" << name << ", age:" << age 
                  << ", salary:$" << salary << ", dept:" << department << "}" << std::endl;
    }
};

struct SensorData {
    int id;
    double value;
    bool is_valid;
    
    SensorData(int i, double v, bool valid = true) : id(i), value(v), is_valid(valid) {}
    
    void display() const {
        std::cout << "Sensor{id:" << id << ", value:" << value 
                  << ", valid:" << (is_valid ? "true" : "false") << "}" << std::endl;
    }
};

// ===== UTILITY FUNCTIONS =====

template<typename Container>
void print_container(const Container& container, const std::string& label) {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& item : container) {
        if (!first) std::cout << ", ";
        std::cout << item;
        first = false;
    }
    std::cout << "]" << std::endl;
}

template<typename Container>
void print_employee_container(const Container& container, const std::string& label) {
    std::cout << label << ":\n";
    for (const auto& emp : container) {
        std::cout << "  ";
        emp.display();
    }
    std::cout << std::endl;
}

// ===== PROBLEM 1: EVEN NUMBERS SQUARED =====

void demonstrateEvenSquaredTraditional() {
    std::cout << "=== Problem 1: Even Numbers Squared (Traditional Loops) ===\n\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    print_container(numbers, "Original numbers");
    
    // Traditional approach with loops
    std::vector<int> even_squared;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int num : numbers) {
        if (num % 2 == 0) {  // Check if even
            even_squared.push_back(num * num);  // Square and add
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    print_container(even_squared, "Even numbers squared (traditional)");
    std::cout << "Traditional approach time: " << duration.count() << " nanoseconds\n\n";
}

void demonstrateEvenSquaredSTL() {
    std::cout << "=== Problem 1: Even Numbers Squared (STL Algorithms) ===\n\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    print_container(numbers, "Original numbers");
    
    // STL approach with algorithms and lambdas
    std::vector<int> even_squared;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Method 1: Using copy_if + transform
    std::vector<int> even_numbers;
    std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(even_numbers),
                 [](int n) { return n % 2 == 0; });
    
    std::transform(even_numbers.begin(), even_numbers.end(), std::back_inserter(even_squared),
                   [](int n) { return n * n; });
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    print_container(even_squared, "Even numbers squared (STL)");
    std::cout << "STL approach time: " << duration.count() << " nanoseconds\n";
    
    // Method 2: More concise with transform + back_inserter
    std::cout << "\nAlternative STL approach (single pass):\n";
    std::vector<int> even_squared_v2;
    
    auto start2 = std::chrono::high_resolution_clock::now();
    
    for (int num : numbers) {
        if (num % 2 == 0) {
            even_squared_v2.push_back(num * num);
        }
    }
    // But using STL for the transformation part:
    even_squared_v2.clear();
    std::for_each(numbers.begin(), numbers.end(), 
                  [&even_squared_v2](int n) {
                      if (n % 2 == 0) {
                          even_squared_v2.push_back(n * n);
                      }
                  });
    
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);
    
    print_container(even_squared_v2, "Even numbers squared (for_each)");
    std::cout << "for_each approach time: " << duration2.count() << " nanoseconds\n\n";
}

// ===== PROBLEM 2: EMPLOYEE DATA PROCESSING =====

void demonstrateEmployeeProcessingTraditional() {
    std::cout << "=== Problem 2: Employee Processing (Traditional Loops) ===\n\n";
    
    std::vector<Employee> employees = {
        {"Alice Johnson", 28, 75000, "Engineering"},
        {"Bob Smith", 35, 82000, "Engineering"},
        {"Carol Davis", 42, 95000, "Management"},
        {"David Wilson", 29, 68000, "Sales"},
        {"Eve Brown", 31, 78000, "Engineering"},
        {"Frank Miller", 38, 89000, "Management"},
        {"Grace Lee", 26, 65000, "Sales"}
    };
    
    print_employee_container(employees, "All employees");
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Find all engineering employees over 30 with salary > 70000
    std::vector<Employee> filtered_employees;
    for (const auto& emp : employees) {
        if (emp.department == "Engineering" && emp.age > 30 && emp.salary > 70000) {
            filtered_employees.push_back(emp);
        }
    }
    
    // Count total employees meeting criteria
    int count = 0;
    for (const auto& emp : employees) {
        if (emp.department == "Engineering" && emp.age > 30 && emp.salary > 70000) {
            count++;
        }
    }
    
    // Calculate average salary of all employees
    double total_salary = 0;
    for (const auto& emp : employees) {
        total_salary += emp.salary;
    }
    double avg_salary = total_salary / employees.size();
    
    // Transform names to uppercase
    std::vector<std::string> uppercase_names;
    for (const auto& emp : employees) {
        std::string upper_name = emp.name;
        for (char& c : upper_name) {
            c = std::toupper(c);
        }
        uppercase_names.push_back(upper_name);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    print_employee_container(filtered_employees, "Filtered employees (traditional)");
    std::cout << "Count of matching employees: " << count << std::endl;
    std::cout << "Average salary: $" << avg_salary << std::endl;
    print_container(uppercase_names, "Uppercase names");
    std::cout << "Traditional processing time: " << duration.count() << " microseconds\n\n";
}

void demonstrateEmployeeProcessingSTL() {
    std::cout << "=== Problem 2: Employee Processing (STL Algorithms) ===\n\n";
    
    std::vector<Employee> employees = {
        {"Alice Johnson", 28, 75000, "Engineering"},
        {"Bob Smith", 35, 82000, "Engineering"},
        {"Carol Davis", 42, 95000, "Management"},
        {"David Wilson", 29, 68000, "Sales"},
        {"Eve Brown", 31, 78000, "Engineering"},
        {"Frank Miller", 38, 89000, "Management"},
        {"Grace Lee", 26, 65000, "Sales"}
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Define the filter criteria as a lambda
    auto engineering_criteria = [](const Employee& emp) {
        return emp.department == "Engineering" && emp.age > 30 && emp.salary > 70000;
    };
    
    // Filter employees using copy_if
    std::vector<Employee> filtered_employees;
    std::copy_if(employees.begin(), employees.end(), std::back_inserter(filtered_employees),
                 engineering_criteria);
    
    // Count employees using count_if
    auto count = std::count_if(employees.begin(), employees.end(), engineering_criteria);
    
    // Calculate average salary using accumulate
    auto total_salary = std::accumulate(employees.begin(), employees.end(), 0.0,
                                       [](double sum, const Employee& emp) {
                                           return sum + emp.salary;
                                       });
    double avg_salary = total_salary / employees.size();
    
    // Transform names to uppercase using transform
    std::vector<std::string> uppercase_names;
    std::transform(employees.begin(), employees.end(), std::back_inserter(uppercase_names),
                   [](const Employee& emp) {
                       std::string upper_name = emp.name;
                       std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
                                    [](char c) { return std::toupper(c); });
                       return upper_name;
                   });
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    print_employee_container(filtered_employees, "Filtered employees (STL)");
    std::cout << "Count of matching employees: " << count << std::endl;
    std::cout << "Average salary: $" << avg_salary << std::endl;
    print_container(uppercase_names, "Uppercase names");
    std::cout << "STL processing time: " << duration.count() << " microseconds\n\n";
}

// ===== PROBLEM 3: SENSOR DATA ANALYSIS =====

void demonstrateSensorDataAnalysis() {
    std::cout << "=== Problem 3: Sensor Data Analysis (STL Focus) ===\n\n";
    
    // Generate sample sensor data
    std::vector<SensorData> sensors;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(10.0, 100.0);
    std::uniform_int_distribution<> valid_dis(0, 10);
    
    for (int i = 1; i <= 20; ++i) {
        bool is_valid = valid_dis(gen) > 1; // 90% chance of being valid
        sensors.emplace_back(i, dis(gen), is_valid);
    }
    
    std::cout << "Generated sensor data:\n";
    std::for_each(sensors.begin(), sensors.end(), 
                  [](const SensorData& s) { s.display(); });
    std::cout << std::endl;
    
    // STL Algorithm demonstrations
    
    // 1. std::for_each - Apply function to every element
    std::cout << "1. Using std::for_each to display sensor IDs:\n";
    std::for_each(sensors.begin(), sensors.end(),
                  [](const SensorData& s) {
                      std::cout << "Sensor ID: " << s.id << " ";
                  });
    std::cout << "\n\n";
    
    // 2. std::find_if - Find first element satisfying condition
    std::cout << "2. Using std::find_if to find first high-value sensor (>80):\n";
    auto high_value_it = std::find_if(sensors.begin(), sensors.end(),
                                     [](const SensorData& s) {
                                         return s.is_valid && s.value > 80.0;
                                     });
    
    if (high_value_it != sensors.end()) {
        std::cout << "Found high-value sensor: ";
        high_value_it->display();
    } else {
        std::cout << "No high-value sensor found.\n";
    }
    std::cout << std::endl;
    
    // 3. std::count_if - Count elements satisfying condition
    std::cout << "3. Using std::count_if to count valid sensors:\n";
    auto valid_count = std::count_if(sensors.begin(), sensors.end(),
                                    [](const SensorData& s) {
                                        return s.is_valid;
                                    });
    std::cout << "Valid sensors: " << valid_count << " out of " << sensors.size() << std::endl;
    
    auto low_value_count = std::count_if(sensors.begin(), sensors.end(),
                                        [](const SensorData& s) {
                                            return s.is_valid && s.value < 30.0;
                                        });
    std::cout << "Low-value sensors (<30): " << low_value_count << std::endl << std::endl;
    
    // 4. std::transform - Create new sequence by transforming elements
    std::cout << "4. Using std::transform to normalize sensor values (0-1 range):\n";
    
    // First find min and max values
    auto minmax_it = std::minmax_element(sensors.begin(), sensors.end(),
                                        [](const SensorData& a, const SensorData& b) {
                                            if (!a.is_valid) return false;
                                            if (!b.is_valid) return true;
                                            return a.value < b.value;
                                        });
    
    double min_val = minmax_it.first->value;
    double max_val = minmax_it.second->value;
    std::cout << "Value range: " << min_val << " to " << max_val << std::endl;
    
    std::vector<double> normalized_values;
    std::transform(sensors.begin(), sensors.end(), std::back_inserter(normalized_values),
                   [min_val, max_val](const SensorData& s) -> double {
                       if (!s.is_valid) return -1.0; // Invalid marker
                       return (s.value - min_val) / (max_val - min_val);
                   });
    
    print_container(normalized_values, "Normalized values");
    std::cout << std::endl;
    
    // 5. Complex chaining example
    std::cout << "5. Complex chaining - Valid sensors with values > 50, transformed to percentages:\n";
    
    // Better approach: use for_each with lambda capture to filter and transform
    std::vector<double> final_percentages;
    std::for_each(sensors.begin(), sensors.end(),
                  [&final_percentages](const SensorData& s) {
                      if (s.is_valid && s.value > 50.0) {
                          final_percentages.push_back(s.value); // Keep as raw value for now
                      }
                  });
    
    print_container(final_percentages, "High-value sensor readings");
    std::cout << std::endl;
}

// ===== ADVANCED STL ALGORITHMS DEMONSTRATION =====

void demonstrateAdvancedAlgorithms() {
    std::cout << "=== Advanced STL Algorithms ===\n\n";
    
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    print_container(numbers, "Original numbers");
    
    // 1. std::sort with custom comparator
    std::vector<int> sorted_desc = numbers;
    std::sort(sorted_desc.begin(), sorted_desc.end(), 
              [](int a, int b) { return a > b; });
    print_container(sorted_desc, "Sorted descending");
    
    // 2. std::partition - separate elements based on predicate
    std::vector<int> to_partition = numbers;
    auto partition_it = std::partition(to_partition.begin(), to_partition.end(),
                                      [](int n) { return n % 2 == 0; });
    
    std::cout << "Partitioned (even first): [";
    for (auto it = to_partition.begin(); it != to_partition.end(); ++it) {
        if (it != to_partition.begin()) std::cout << ", ";
        std::cout << *it;
        if (it == partition_it - 1) std::cout << " | ";
    }
    std::cout << "]" << std::endl;
    
    // 3. std::accumulate with custom operation
    auto product = std::accumulate(numbers.begin(), numbers.end(), 1,
                                  [](int acc, int n) { return acc * n; });
    std::cout << "Product of all numbers: " << product << std::endl;
    
    // 4. std::any_of, std::all_of, std::none_of
    bool has_even = std::any_of(numbers.begin(), numbers.end(),
                               [](int n) { return n % 2 == 0; });
    bool all_positive = std::all_of(numbers.begin(), numbers.end(),
                                   [](int n) { return n > 0; });
    bool none_negative = std::none_of(numbers.begin(), numbers.end(),
                                     [](int n) { return n < 0; });
    
    std::cout << "Has even numbers: " << (has_even ? "Yes" : "No") << std::endl;
    std::cout << "All positive: " << (all_positive ? "Yes" : "No") << std::endl;
    std::cout << "None negative: " << (none_negative ? "Yes" : "No") << std::endl;
    
    // 5. std::remove_if (doesn't actually remove, moves to end)
    std::vector<int> to_remove = numbers;
    auto new_end = std::remove_if(to_remove.begin(), to_remove.end(),
                                 [](int n) { return n % 3 == 0; });
    to_remove.erase(new_end, to_remove.end()); // Actually remove
    print_container(to_remove, "After removing multiples of 3");
    
    std::cout << std::endl;
}

// ===== PERFORMANCE COMPARISON =====

void performanceComparison() {
    std::cout << "=== Performance Comparison ===\n\n";
    
    // Generate large dataset
    const size_t SIZE = 1000000;
    std::vector<int> large_data(SIZE);
    std::iota(large_data.begin(), large_data.end(), 1); // Fill with 1, 2, 3, ...
    
    std::cout << "Testing with " << SIZE << " elements...\n\n";
    
    // Traditional loop approach
    auto start1 = std::chrono::high_resolution_clock::now();
    
    std::vector<int> result1;
    for (int num : large_data) {
        if (num % 2 == 0 && num > 100) {
            result1.push_back(num * num);
        }
    }
    
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    
    // STL algorithm approach
    auto start2 = std::chrono::high_resolution_clock::now();
    
    std::vector<int> result2;
    std::for_each(large_data.begin(), large_data.end(),
                  [&result2](int num) {
                      if (num % 2 == 0 && num > 100) {
                          result2.push_back(num * num);
                      }
                  });
    
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
    
    std::cout << "Traditional loop time: " << duration1.count() << " ms" << std::endl;
    std::cout << "STL for_each time: " << duration2.count() << " ms" << std::endl;
    std::cout << "Results size (both): " << result1.size() << std::endl;
    std::cout << "Results match: " << (result1 == result2 ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
}

// ===== EMBEDDED SYSTEMS CONSIDERATIONS =====

void embeddedConsiderations() {
    std::cout << "=== Embedded Systems Considerations ===\n\n";
    
    std::cout << "STL Algorithms in Embedded Systems:\n\n";
    
    std::cout << "PROS:\n";
    std::cout << "✓ Often compile to optimal assembly code\n";
    std::cout << "✓ Less prone to off-by-one errors\n";
    std::cout << "✓ More readable and maintainable\n";
    std::cout << "✓ Standardized and well-tested\n";
    std::cout << "✓ Can be optimized by compiler better than manual loops\n\n";
    
    std::cout << "CONS:\n";
    std::cout << "✗ May use more stack space for lambda captures\n";
    std::cout << "✗ Some algorithms may allocate memory (be careful!)\n";
    std::cout << "✗ Can be harder to debug in some embedded debuggers\n";
    std::cout << "✗ Template instantiation can increase code size\n\n";
    
    std::cout << "BEST PRACTICES:\n";
    std::cout << "• Use algorithms that work with existing containers\n";
    std::cout << "• Avoid algorithms that allocate memory dynamically\n";
    std::cout << "• Use lambdas with minimal captures\n";
    std::cout << "• Profile code to ensure performance meets requirements\n";
    std::cout << "• Consider using constexpr where applicable\n";
    std::cout << "• Test thoroughly on target hardware\n\n";
}

// ===== MAIN DEMONSTRATION =====

int main() {
    std::cout << "C++ STL Algorithms Mastery Demonstration\n";
    std::cout << "=======================================\n\n";
    
    demonstrateEvenSquaredTraditional();
    std::cout << std::string(70, '-') << "\n\n";
    
    demonstrateEvenSquaredSTL();
    std::cout << std::string(70, '-') << "\n\n";
    
    demonstrateEmployeeProcessingTraditional();
    std::cout << std::string(70, '-') << "\n\n";
    
    demonstrateEmployeeProcessingSTL();
    std::cout << std::string(70, '-') << "\n\n";
    
    demonstrateSensorDataAnalysis();
    std::cout << std::string(70, '-') << "\n\n";
    
    demonstrateAdvancedAlgorithms();
    std::cout << std::string(70, '-') << "\n\n";
    
    performanceComparison();
    std::cout << std::string(70, '-') << "\n\n";
    
    embeddedConsiderations();
    
    return 0;
}
