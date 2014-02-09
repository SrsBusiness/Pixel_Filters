all:
	g++ test.cpp filter.cpp -lX11 -lpng -o edges -g
clean:
	rm edges edges.png
