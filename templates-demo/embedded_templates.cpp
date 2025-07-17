#include <iostream>
#include <cstdint>
#include <array>
#include <type_traits>

// ===== EMBEDDED SYSTEMS SPECIFIC TEMPLATES =====

// Fixed-point arithmetic template for embedded systems
template <typename BaseType, int FractionalBits>
class FixedPoint {
private:
    BaseType value;
    static constexpr BaseType SCALE = 1 << FractionalBits;
    
public:
    FixedPoint() : value(0) {}
    
    FixedPoint(float f) : value(static_cast<BaseType>(f * SCALE)) {}
    FixedPoint(double d) : value(static_cast<BaseType>(d * SCALE)) {}
    FixedPoint(int i) : value(i * SCALE) {}
    
    // Convert back to floating point
    float to_float() const {
        return static_cast<float>(value) / SCALE;
    }
    
    double to_double() const {
        return static_cast<double>(value) / SCALE;
    }
    
    // Arithmetic operations
    FixedPoint operator+(const FixedPoint& other) const {
        FixedPoint result;
        result.value = value + other.value;
        return result;
    }
    
    FixedPoint operator-(const FixedPoint& other) const {
        FixedPoint result;
        result.value = value - other.value;
        return result;
    }
    
    FixedPoint operator*(const FixedPoint& other) const {
        FixedPoint result;
        result.value = (static_cast<int64_t>(value) * other.value) >> FractionalBits;
        return result;
    }
    
    FixedPoint operator/(const FixedPoint& other) const {
        FixedPoint result;
        result.value = (static_cast<int64_t>(value) << FractionalBits) / other.value;
        return result;
    }
    
    void display() const {
        std::cout << "FixedPoint(" << FractionalBits << " bits): " 
                  << to_double() << " (raw: " << value << ")" << std::endl;
    }
};

// Ring buffer template for embedded systems
template <typename T, size_t Size>
class RingBuffer {
private:
    T buffer[Size];
    size_t head;
    size_t tail;
    bool full;
    
public:
    RingBuffer() : head(0), tail(0), full(false) {
        static_assert(Size > 0, "Ring buffer size must be greater than 0");
    }
    
    bool push(const T& item) {
        buffer[head] = item;
        
        if (full) {
            tail = (tail + 1) % Size;  // Overwrite oldest
        }
        
        head = (head + 1) % Size;
        
        if (head == tail) {
            full = true;
        }
        
        return true;
    }
    
    bool pop(T& item) {
        if (empty()) {
            return false;
        }
        
        item = buffer[tail];
        full = false;
        tail = (tail + 1) % Size;
        
        return true;
    }
    
    bool peek(T& item) const {
        if (empty()) {
            return false;
        }
        
        item = buffer[tail];
        return true;
    }
    
    bool empty() const {
        return (!full && (head == tail));
    }
    
    bool is_full() const {
        return full;
    }
    
    size_t size() const {
        if (full) return Size;
        if (head >= tail) return head - tail;
        return Size + head - tail;
    }
    
    constexpr size_t capacity() const {
        return Size;
    }
    
    void display() const {
        std::cout << "RingBuffer[" << size() << "/" << Size << "]: ";
        if (empty()) {
            std::cout << "[empty]" << std::endl;
            return;
        }
        
        std::cout << "[";
        size_t current = tail;
        size_t count = 0;
        size_t current_size = size();
        
        while (count < current_size) {
            if (count > 0) std::cout << ", ";
            std::cout << buffer[current];
            current = (current + 1) % Size;
            count++;
        }
        std::cout << "]" << std::endl;
    }
};

// GPIO pin template for type-safe embedded programming
template <uint8_t PinNumber>
class GPIO {
private:
    static constexpr uint8_t pin = PinNumber;
    bool state;
    
public:
    GPIO() : state(false) {
        static_assert(PinNumber < 32, "Pin number must be less than 32");
        std::cout << "GPIO Pin " << static_cast<int>(pin) << " initialized" << std::endl;
    }
    
    void set_high() {
        state = true;
        std::cout << "Pin " << static_cast<int>(pin) << " set HIGH" << std::endl;
    }
    
    void set_low() {
        state = false;
        std::cout << "Pin " << static_cast<int>(pin) << " set LOW" << std::endl;
    }
    
    void toggle() {
        state = !state;
        std::cout << "Pin " << static_cast<int>(pin) << " toggled to " 
                  << (state ? "HIGH" : "LOW") << std::endl;
    }
    
    bool read() const {
        return state;
    }
    
    constexpr uint8_t get_pin_number() const {
        return pin;
    }
};

// Memory pool template for deterministic allocation
template <typename T, size_t PoolSize>
class MemoryPool {
private:
    alignas(T) uint8_t pool[PoolSize * sizeof(T)];
    bool used[PoolSize];
    size_t allocated_count;
    
public:
    MemoryPool() : allocated_count(0) {
        for (size_t i = 0; i < PoolSize; ++i) {
            used[i] = false;
        }
        std::cout << "MemoryPool for " << PoolSize << " objects of size " 
                  << sizeof(T) << " bytes created" << std::endl;
    }
    
    T* allocate() {
        for (size_t i = 0; i < PoolSize; ++i) {
            if (!used[i]) {
                used[i] = true;
                allocated_count++;
                T* ptr = reinterpret_cast<T*>(pool + i * sizeof(T));
                std::cout << "Allocated object at index " << i << std::endl;
                return ptr;
            }
        }
        std::cout << "MemoryPool exhausted!" << std::endl;
        return nullptr;
    }
    
    void deallocate(T* ptr) {
        if (ptr == nullptr) return;
        
        uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(ptr);
        if (byte_ptr < pool || byte_ptr >= pool + PoolSize * sizeof(T)) {
            std::cout << "Invalid pointer for deallocation!" << std::endl;
            return;
        }
        
        size_t index = (byte_ptr - pool) / sizeof(T);
        if (used[index]) {
            used[index] = false;
            allocated_count--;
            std::cout << "Deallocated object at index " << index << std::endl;
        }
    }
    
    size_t available() const {
        return PoolSize - allocated_count;
    }
    
    size_t allocated() const {
        return allocated_count;
    }
    
    constexpr size_t capacity() const {
        return PoolSize;
    }
    
    void display_status() const {
        std::cout << "MemoryPool status: " << allocated_count << "/" << PoolSize 
                  << " allocated, " << available() << " available" << std::endl;
    }
};

// State machine template
template <typename StateEnum, StateEnum InitialState>
class StateMachine {
private:
    StateEnum current_state;
    
public:
    StateMachine() : current_state(InitialState) {
        std::cout << "StateMachine initialized to state " << static_cast<int>(current_state) << std::endl;
    }
    
    void transition_to(StateEnum new_state) {
        StateEnum old_state = current_state;
        current_state = new_state;
        std::cout << "State transition: " << static_cast<int>(old_state) 
                  << " -> " << static_cast<int>(current_state) << std::endl;
    }
    
    StateEnum get_state() const {
        return current_state;
    }
    
    bool is_in_state(StateEnum state) const {
        return current_state == state;
    }
};

// ===== EMBEDDED SYSTEM EXAMPLES =====

// Example: Motor control states
enum class MotorState {
    STOPPED = 0,
    ACCELERATING = 1,
    RUNNING = 2,
    DECELERATING = 3,
    ERROR = 4
};

// Example: Sensor data structure for memory pool
struct SensorReading {
    uint32_t timestamp;
    float temperature;
    float humidity;
    uint16_t light_level;
    
    SensorReading() : timestamp(0), temperature(0.0f), humidity(0.0f), light_level(0) {}
    
    SensorReading(uint32_t ts, float temp, float hum, uint16_t light) 
        : timestamp(ts), temperature(temp), humidity(hum), light_level(light) {}
    
    void display() const {
        std::cout << "SensorReading{ts:" << timestamp << ", temp:" << temperature 
                  << "°C, hum:" << humidity << "%, light:" << light_level << "}" << std::endl;
    }
};

// ===== DEMONSTRATION FUNCTIONS =====

void demonstrateFixedPoint() {
    std::cout << "=== Fixed-Point Arithmetic ===\n\n";
    
    // 16-bit fixed point with 8 fractional bits
    using Fixed16_8 = FixedPoint<int16_t, 8>;
    
    Fixed16_8 a(3.14f);
    Fixed16_8 b(2.5f);
    
    std::cout << "Fixed-point arithmetic (16-bit, 8 fractional bits):\n";
    a.display();
    b.display();
    
    auto sum = a + b;
    auto diff = a - b;
    auto product = a * b;
    auto quotient = a / b;
    
    std::cout << "a + b = ";
    sum.display();
    std::cout << "a - b = ";
    diff.display();
    std::cout << "a * b = ";
    product.display();
    std::cout << "a / b = ";
    quotient.display();
    std::cout << "\n";
}

void demonstrateRingBuffer() {
    std::cout << "=== Ring Buffer ===\n\n";
    
    RingBuffer<int, 5> ring;
    
    std::cout << "Pushing elements to ring buffer:\n";
    for (int i = 1; i <= 7; ++i) {
        ring.push(i * 10);
        ring.display();
    }
    
    std::cout << "\nPopping elements:\n";
    int value;
    while (ring.pop(value)) {
        std::cout << "Popped: " << value << std::endl;
        ring.display();
    }
    std::cout << "\n";
}

void demonstrateGPIO() {
    std::cout << "=== GPIO Template ===\n\n";
    
    GPIO<13> led;      // LED on pin 13
    GPIO<2> button;    // Button on pin 2
    
    std::cout << "GPIO operations:\n";
    led.set_high();
    led.set_low();
    led.toggle();
    
    button.set_high();  // Simulating button press
    std::cout << "Button state: " << (button.read() ? "PRESSED" : "RELEASED") << std::endl;
    std::cout << "\n";
}

void demonstrateMemoryPool() {
    std::cout << "=== Memory Pool ===\n\n";
    
    MemoryPool<SensorReading, 4> sensor_pool;
    
    std::cout << "Allocating sensor readings:\n";
    sensor_pool.display_status();
    
    // Allocate and construct objects
    SensorReading* readings[5];
    for (int i = 0; i < 5; ++i) {
        readings[i] = sensor_pool.allocate();
        if (readings[i]) {
            new(readings[i]) SensorReading(i * 1000, 20.0f + i, 50.0f + i * 5, 100 + i * 50);
            readings[i]->display();
            sensor_pool.display_status();
        }
    }
    
    std::cout << "\nDeallocating some objects:\n";
    if (readings[1]) {
        readings[1]->~SensorReading();
        sensor_pool.deallocate(readings[1]);
        sensor_pool.display_status();
    }
    
    if (readings[3]) {
        readings[3]->~SensorReading();
        sensor_pool.deallocate(readings[3]);
        sensor_pool.display_status();
    }
    std::cout << "\n";
}

void demonstrateStateMachine() {
    std::cout << "=== State Machine ===\n\n";
    
    StateMachine<MotorState, MotorState::STOPPED> motor;
    
    std::cout << "Motor control state machine:\n";
    std::cout << "Current state: " << static_cast<int>(motor.get_state()) << std::endl;
    
    motor.transition_to(MotorState::ACCELERATING);
    motor.transition_to(MotorState::RUNNING);
    motor.transition_to(MotorState::DECELERATING);
    motor.transition_to(MotorState::STOPPED);
    
    std::cout << "Is motor stopped? " << (motor.is_in_state(MotorState::STOPPED) ? "Yes" : "No") << std::endl;
    std::cout << "\n";
}

void demonstrateEmbeddedBenefits() {
    std::cout << "=== Embedded Systems Benefits ===\n\n";
    
    std::cout << "Template benefits for embedded systems:\n\n";
    
    std::cout << "1. Zero Runtime Overhead:\n";
    std::cout << "   - All template instantiation happens at compile time\n";
    std::cout << "   - No virtual function calls or dynamic dispatch\n";
    std::cout << "   - Direct, optimized machine code generation\n\n";
    
    std::cout << "2. Type Safety:\n";
    std::cout << "   - GPIO<13> vs GPIO<14> are different types\n";
    std::cout << "   - Compile-time error if you mix them up\n";
    std::cout << "   - Prevents runtime errors in critical systems\n\n";
    
    std::cout << "3. Memory Efficiency:\n";
    std::cout << "   - Fixed-size containers with compile-time bounds checking\n";
    std::cout << "   - Memory pools eliminate dynamic allocation\n";
    std::cout << "   - Ring buffers provide predictable memory usage\n\n";
    
    std::cout << "4. Performance Predictability:\n";
    std::cout << "   - All operations have known time complexity\n";
    std::cout << "   - No garbage collection or hidden allocations\n";
    std::cout << "   - Suitable for real-time systems\n\n";
    
    std::cout << "5. Code Reuse:\n";
    std::cout << "   - Same template works for different data types\n";
    std::cout << "   - Reduces code duplication and maintenance\n";
    std::cout << "   - Easier to test and validate\n\n";
}

int main() {
    std::cout << "Embedded Systems C++ Templates Demonstration\n";
    std::cout << "============================================\n\n";
    
    demonstrateFixedPoint();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateRingBuffer();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateGPIO();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateMemoryPool();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateStateMachine();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateEmbeddedBenefits();
    
    return 0;
}
