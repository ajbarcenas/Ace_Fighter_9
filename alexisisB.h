// Game: Ace Fighter 9
// Program: alexisisB.h
// Author: Alexisis Barcenas
// Date Created: 9/26/19
/*=============[Notes]=================
Current class in my header file is ABarGlobal
which currently holds general functions that
print out basic information.
Current functions are being used to create
the features of the credits page.
Credits page is still a WIP and will continually
be receiving updates.
  */

#ifndef ALEXISIS_B_H
#define ALEXISIS_B_H

#include <iostream>
#include "fonts.h"
#include <string.h>
#include <GL/glx.h>
#include <stdio.h>

//New vector for hitboxes
struct Vec1 {
	float x, y, z;
};
// Struct that would be able to assign colors to our objects.
struct Color {
	int r, g, b;
};
// Values that I will create the hitboxes from
struct Box {
	float width, height;
	Vec1 center;
};

class ABarGlobal {
	public:
		Rect r;
		void printAceFighter9(Rect r);
		void printPicture(double x, double y, double z, GLuint texturecode);
		void printCredBoxes(int x_pos, int y_pos);
};

void printAlexisB(Rect r);
void showCreditsBorder();
#endif
