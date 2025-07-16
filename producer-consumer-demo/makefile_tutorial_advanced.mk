# 🎓 ADVANCED MAKEFILE TUTORIAL
# =============================
# Analyzing our Producer-Consumer Makefile

# LESSON 1: ADVANCED VARIABLES
# ============================

# Using := for immediate assignment (more efficient)
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -pthread

# 🔍 EXPLANATION OF FLAGS:
# -std=c++11  : Use C++11 standard (needed for std::thread, std::mutex)
# -Wall       : Enable all common warnings
# -Wextra     : Enable extra warnings  
# -O2         : Optimization level 2 (faster code)
# -pthread    : Link with POSIX threads library

# Multiple related variables for organization
TARGET := producer_consumer
SIMPLE_TARGET := simple_demo
STEP1_TARGET := step1_broken
STEP2_TARGET := step2_mutex  
STEP3_TARGET := step3_efficient
SUMMARY_TARGET := summary

# Lists of source files
SOURCES := producer_consumer.cpp simple_demo.cpp step1_broken.cpp step2_mutex.cpp step3_efficient.cpp summary.cpp

# LESSON 2: PATTERN RULES AND DEPENDENCIES
# =========================================

# Explicit rules for each target
$(TARGET): producer_consumer.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) producer_consumer.cpp

$(SIMPLE_TARGET): simple_demo.cpp
	$(CXX) $(CXXFLAGS) -o $(SIMPLE_TARGET) simple_demo.cpp

# 🔍 EXPLANATION:
# $(TARGET) expands to "producer_consumer"
# The dependency ensures rebuild if source file changes
# $< means "first dependency" (producer_consumer.cpp)
# $@ means "target name" (producer_consumer)

# LESSON 3: ADVANCED PATTERN RULES
# =================================

# Pattern rule for step files (more elegant)
step%: step%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# 🔍 EXPLANATION:
# % is a wildcard that matches any string
# step% matches step1_broken, step2_mutex, etc.
# $@ = target name (step1_broken)
# $< = first prerequisite (step1_broken.cpp)

# LESSON 4: PHONY TARGETS WITH DEPENDENCIES
# ==========================================

.PHONY: all learning clean run test help step1 step2 step3

# Group targets - build multiple things at once
all: $(TARGET) $(SIMPLE_TARGET)
learning: $(STEP1_TARGET) $(STEP2_TARGET) $(STEP3_TARGET) $(SUMMARY_TARGET)

# LESSON 5: ACTION TARGETS (DO SOMETHING)
# =======================================

# Targets that run programs (not just build them)
run: $(TARGET)
	./$(TARGET)

run-simple: $(SIMPLE_TARGET)  
	./$(SIMPLE_TARGET)

# Educational targets with messages
step1: $(STEP1_TARGET)
	@echo "🚨 Running BROKEN version (expect chaos!)..."
	./$(STEP1_TARGET)

step2: $(STEP2_TARGET)
	@echo "🔒 Running MUTEX version (safe but inefficient)..."
	./$(STEP2_TARGET)

step3: $(STEP3_TARGET)
	@echo "✨ Running EFFICIENT version (perfect!)..."
	./$(STEP3_TARGET)

# LESSON 6: CONDITIONAL COMPILATION
# ==================================

# Debug vs Release builds
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET) $(SIMPLE_TARGET)

# 🔍 EXPLANATION:
# CXXFLAGS += adds to existing flags
# -g adds debug symbols
# -DDEBUG defines DEBUG macro
# This target changes flags, then builds

# LESSON 7: CLEANUP AND UTILITY TARGETS
# ======================================

clean:
	rm -f $(TARGET) $(SIMPLE_TARGET) $(STEP1_TARGET) $(STEP2_TARGET) $(STEP3_TARGET) $(SUMMARY_TARGET) hello

# Help target with detailed formatting
help:
	@echo "🎓 MAKEFILE LEARNING COMMANDS:"
	@echo "================================"
	@echo ""
	@echo "📚 Basic builds:"
	@echo "  all         - Build main programs"
	@echo "  learning    - Build all learning steps"
	@echo "  clean       - Remove all built files"
	@echo ""
	@echo "🏃 Run programs:"
	@echo "  run         - Build and run main demo"
	@echo "  run-simple  - Build and run simple demo"
	@echo ""
	@echo "🎯 Educational sequence:"
	@echo "  step1       - Run broken version (race conditions)"
	@echo "  step2       - Run mutex version (busy-waiting)"
	@echo "  step3       - Run efficient version (condition variables)"
	@echo ""
	@echo "🔧 Development:"
	@echo "  debug       - Build with debug symbols"
	@echo "  help        - Show this help"

# LESSON 8: ADVANCED MAKE FEATURES
# =================================

# Check if files exist before trying to build
ifneq ($(wildcard *.cpp),)
    FOUND_CPP := yes
else
    FOUND_CPP := no
endif

info:
	@echo "C++ files found: $(FOUND_CPP)"
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Targets: $(TARGET) $(SIMPLE_TARGET)"

# LESSON 9: AUTOMATIC DEPENDENCY GENERATION
# ==========================================

# Generate dependency files for automatic header dependency tracking
%.d: %.cpp
	@$(CXX) -MM $(CXXFLAGS) $< > $@

# Include dependency files if they exist
-include $(SOURCES:.cpp=.d)

# 🔍 EXPLANATION:
# -MM generates dependency information
# .d files contain header dependencies
# -include ignores missing .d files
# This ensures rebuilds when headers change

.PHONY: info
