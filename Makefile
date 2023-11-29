# Makefile for compiling C++ code in src and its subdirectories

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall

# Directories
SRCDIR := src
OBJDIR := target/obj
BINDIR := target

# Source files
SRC := $(wildcard $(SRCDIR)/**/*.cpp $(SRCDIR)/*.cpp)

# Object files
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

# Target binary
TARGET := $(BINDIR)/iris.bin

# Compile rule
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object file rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Phony target to clean the project
.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony target to create directories
.PHONY: directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Main build target
all: directories $(TARGET)

run: all
	@$(TARGET)

# Default target
.DEFAULT_GOAL := all

