all: background

background: background.cpp
	g++ background.cpp -Wall -obackground -lX11 -lGL -lGLU -lm

clean:
	rm -f background
	rm -f *.o


