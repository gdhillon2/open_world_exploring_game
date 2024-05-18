# Compiler and flags
CXX = g++
CXXFLAGS = -I/opt/homebrew/Cellar/sdl2/2.30.3/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2 -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf

# Project details
TARGET = my_game
SRCS = $(wildcard *.cpp lib/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

