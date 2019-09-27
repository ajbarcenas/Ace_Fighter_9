// WIP Header File for Alexisis Barcenas
// Goal is to create a few new classes to implement my functions in the main file

#ifndef ALEXISIS_B_H
#define ALEXISIS_B_H

#include <iostream>
#include "fonts.h"
#include <string.h>
#include <GL/glx.h>
#include <stdio.h>


class ABarGlobal {
	public:
		Rect r;
		void printAceFighter(Rect r);
		void printLogo(double x, double y, double z, GLuint texturecode);
};

void printAlexisB(Rect r);

#endif
