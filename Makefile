all:
	g++ -Wall -o visualize  graphics.cpp graphics.h grid.cpp grid.h min_heap.cpp -lglut -lGLU -lGL 
clean:
	rm visualize
