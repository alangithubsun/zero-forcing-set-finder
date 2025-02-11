# Compiler
CXX ?= g++

# Default Flags
CXXFLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic

# Files
FILES = main.cpp Graph.hpp Graph.cpp

zero: $(FILES)
	$(CXX) $(CXXFLAGS) $(FILES) -o zero

# make debug - will compile sources with $(CXXFLAGS) -g3 and -fsanitize
#              flags also defines DEBUG and _GLIBCXX_DEBUG
debug: CXXFLAGS += -g3 -DDEBUG -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG
debug: $(FILES)
	$(CXX) $(CXXFLAGS) $(FILES) -o zero_debug
.PHONY: debug