CXX = clang++
CXXFLAGS = -std=c++17 -Wall -I/path/to/SDL2/include
LDFLAGS = -L/path/to/SDL2/lib -lSDL2

all: final_mario

final_mario: main.cpp other_source_file.cpp
	$(CXX) $(CXXFLAGS) -o final_mario main.cpp other_source_file.cpp $(LDFLAGS)

clean:
	rm -f final_mario
l
