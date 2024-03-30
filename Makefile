# Compiler settings - Change according to your needs
CXX = clang++
CXXFLAGS = -Wall -std=c++17

# Project files
SOURCES = tests/ctb_decoder_test.cpp components/standing_desk_height/ctb_decoder.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = test_ctb_decoder

# Main target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# To obtain object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean
