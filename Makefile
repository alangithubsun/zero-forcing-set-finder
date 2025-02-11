# Compiler
CXX ?= g++

# Default Flags
CXXFLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic

EXECUTABLE = zero

# list of sources used in project
SOURCES     = $(wildcard *.cpp)

# list of objects used in project
OBJECTS     = $(SOURCES:%.cpp=%.o)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o zero

# make debug - will compile sources with $(CXXFLAGS) -g3 and -fsanitize
#              flags also defines DEBUG and _GLIBCXX_DEBUG
debug: CXXFLAGS += -g3 -DDEBUG -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG
debug: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o zero_debug
.PHONY: debug

# make clean - remove .o files, executables, tarball
clean:
	rm -Rf *.dSYM
	rm -f $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE)_debug
.PHONY: clean