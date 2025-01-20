# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -fPIC -Ofast $(shell pkg-config --cflags --libs Qt6Widgets)

# Source files
SRC = gui.cpp

# Output executable
OUT = TicTacToe

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Clean target
clean:
	rm -f $(OUT)