CXX=g++
CXXFLAGS=-std=c++17
OUTPUT=exec

all:
	$(CXX) $(CXXFLAGS) source/main.cpp -o $(OUTPUT)
