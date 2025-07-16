#!/bin/bash

# 🎓 Interactive Makefile Learning Script
# =======================================

echo "🎓 WELCOME TO INTERACTIVE MAKEFILE LEARNING!"
echo "============================================="
echo ""
echo "This script will teach you our Makefile step by step"
echo "by showing you what each command does."
echo ""

# Function to pause and wait for user
pause() {
    echo ""
    read -p "📚 Press ENTER to continue to the next lesson..."
    echo ""
}

# Function to run a command and explain it
demo_command() {
    local cmd="$1"
    local explanation="$2"
    
    echo "🔍 EXPLANATION: $explanation"
    echo "💻 COMMAND: $cmd"
    echo "📤 OUTPUT:"
    echo "-------------------"
    eval "$cmd"
    echo "-------------------"
    pause
}

echo "🎯 LESSON 1: Understanding Makefile Variables"
echo "=============================================="
echo ""
echo "Our Makefile starts with variable definitions:"
echo "CXX = g++                    # The compiler"
echo "CXXFLAGS = -std=c++11 -pthread  # Compiler flags"
echo "TARGET = producer_consumer   # Main program name"
echo ""
demo_command "make -f Makefile -p | grep '^CXX'" "Show how make sees the CXX variable"

echo "🎯 LESSON 2: Basic Target Dependencies"
echo "======================================"
echo ""
echo "Targets depend on source files. Let's see what happens"
echo "when we check if our programs need rebuilding:"
echo ""
demo_command "make -n step1" "Show what make WOULD do (dry run) for step1"

echo "🎯 LESSON 3: Phony Targets"  
echo "=========================="
echo ""
echo "Some targets don't create files, they just run commands."
echo "These are called 'phony' targets:"
echo ""
demo_command "make help" "Run the help target (phony - no file created)"

echo "🎯 LESSON 4: Conditional Rebuilding"
echo "==================================="
echo ""
echo "Make only rebuilds when source files are newer than targets."
echo "Let's test this:"
echo ""

# Check if step1_broken exists
if [ -f "step1_broken" ]; then
    demo_command "make step1_broken" "Try to rebuild step1_broken (should say 'up to date')"
    
    echo "Now let's 'touch' the source file to make it newer:"
    demo_command "touch step1_broken.cpp" "Make the source file newer"
    demo_command "make step1_broken" "Now make will rebuild it!"
else
    demo_command "make step1_broken" "Build step1_broken for the first time"
fi

echo "🎯 LESSON 5: Pattern Rules"
echo "=========================="
echo ""
echo "Our Makefile uses pattern rules for efficiency."
echo "One rule can build multiple similar targets:"
echo ""
echo "Pattern rule: step%: step%.cpp"
echo "This matches: step1_broken, step2_mutex, step3_efficient"
echo ""
demo_command "make step2_mutex" "Build using pattern rule"

echo "🎯 LESSON 6: Group Targets"
echo "=========================="
echo ""
echo "Some targets build multiple things at once:"
echo ""
demo_command "make -n learning" "Show what 'make learning' would build (dry run)"

echo "🎯 LESSON 7: Action Targets"
echo "==========================="
echo ""
echo "Some targets not only build but also RUN programs:"
echo ""
demo_command "make -n step3" "Show what 'make step3' does (build AND run)"

echo "🎯 LESSON 8: Variable Override"
echo "=============================="
echo ""
echo "You can override Makefile variables from command line:"
echo ""
demo_command "make CXXFLAGS='-std=c++11 -pthread -Wall' -n step1" "Override compiler flags"

echo "🎯 LESSON 9: Parallel Builds"
echo "============================"
echo ""
echo "Make can build multiple targets in parallel:"
echo ""
demo_command "make -j2 -n learning" "Build with 2 parallel jobs"

echo "🎯 LESSON 10: Debugging Makefiles"
echo "================================="
echo ""
echo "Make has built-in debugging features:"
echo ""
demo_command "make --debug=basic step1 2>&1 | head -10" "Show make's decision process"

echo ""
echo "🎉 CONGRATULATIONS!"
echo "==================="
echo ""
echo "You've completed the interactive Makefile tutorial!"
echo ""
echo "🎯 Key concepts you learned:"
echo "• Variables and how to override them"
echo "• Target dependencies and rebuilding logic"
echo "• Phony targets vs file targets"
echo "• Pattern rules for multiple similar targets"
echo "• Group targets and action targets"
echo "• Parallel builds"
echo "• Debugging techniques"
echo ""
echo "🚀 Try these advanced experiments:"
echo "• make clean && time make -j4 learning"
echo "• make CXXFLAGS='-O3' step3"
echo "• make --debug=verbose step1"
echo ""
echo "📚 Read MAKEFILE_TUTORIAL.md for more advanced concepts!"
