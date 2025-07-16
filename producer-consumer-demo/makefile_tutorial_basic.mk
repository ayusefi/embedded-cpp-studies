# 🎓 MAKEFILE LEARNING TUTORIAL
# ============================
# This is a step-by-step tutorial on Makefile concepts

# STEP 1: BASIC VARIABLES
# Variables in Makefiles store values that can be reused
CC = gcc                    # C compiler
CXX = g++                  # C++ compiler  
CFLAGS = -Wall -g          # C compiler flags
CXXFLAGS = -Wall -g        # C++ compiler flags

# 🔍 EXPLANATION:
# Variables are defined with = 
# Use $(VARIABLE_NAME) to reference them
# This makes it easy to change compiler or flags in one place

# STEP 2: BASIC TARGET SYNTAX
# target: dependencies
#	command
# Note: Commands MUST start with a TAB, not spaces!

hello: hello.cpp
	$(CXX) $(CXXFLAGS) -o hello hello.cpp

# 🔍 EXPLANATION:
# - 'hello' is the target (what we want to build)
# - 'hello.cpp' is the dependency (what it needs)
# - The command tells make HOW to build it
# - If hello.cpp is newer than hello, make will rebuild

# STEP 3: MULTIPLE TARGETS
target1: source1.cpp
	$(CXX) -o target1 source1.cpp

target2: source2.cpp
	$(CXX) -o target2 source2.cpp

# STEP 4: PHONY TARGETS
# .PHONY tells make these aren't files, they're commands
.PHONY: clean help test

clean:
	rm -f hello target1 target2

help:
	@echo "Available targets:"
	@echo "  hello  - Build hello program"
	@echo "  clean  - Remove built files"
	@echo "  help   - Show this help"

# 🔍 EXPLANATION:
# @ suppresses the command from being printed
# .PHONY prevents conflicts if files named 'clean' or 'help' exist

# STEP 5: DEFAULT TARGET
# The first target is the default when you just run 'make'
all: hello target1 target2

# 🔍 EXPLANATION:
# 'all' depends on other targets
# make will build all dependencies first
