all: background

background: background.cpp alexisisB.cpp andrewO.cpp alonsoG.cpp diegoD.cpp 
	g++ background.cpp alexisisB.cpp andrewO.cpp alonsoG.cpp diegoD.cpp -Wall -obackground -lX11 -lGL -lGLU -lm

clean:
	rm -f background
	rm -f *.o


