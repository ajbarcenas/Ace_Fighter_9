// Author: Alexisis Barcenas
// Date: 9/17/2019
// Game: Ace Fighter 9
// Program: alexisisB.cpp
// Goals: Have hitboxes work correctly for player and enemy models
//
#ifndef ALEXISIS_B_CPP
#define ALEXISIS_B_CPP
#include "alexisisB.h"
using namespace std;
ABarGlobal abG;

//Prints out my name on the credits Screen
void printAlexisB(Rect r) 
{
	glColor3f(1.0, 1.0, 1.0);
	r.left = 340;
	r.bot = 645;
	r.center = 0;
	ggprint16(&r, 16, 0xcf13ac, "Alexisis Barcenas - Hitbox Specialist");    
}

void ABarGlobal::printAceFighter9(Rect r)
{
	glColor3f(1.0, 1.0, 1.0);
	string aceLetters[11] = {"A", "C", "E", "F","I","G", "H", "T", "E",
		"R", "9"};
	r.left = 960;
	r.bot = 1050;
	for (int i = 0; i < 11; ++i) {
		string tmp = aceLetters[i];
		ggprint16(&r, 100, 0xfffff, tmp.c_str());
	}
	int x_scale = 100;
	r.left = 485;
	r.bot = 550;
	for (int i = 0; i < 11; ++i) {
		string tmp = aceLetters[i];
		
		if (i <= 4){
			glColor3f(1.0, 1.0, 1.0);
			r.left = 485  + i * x_scale;
			ggprint16(&r, 0, 0xfffff, tmp.c_str());
		}
		else if (i == 5) {
			r.center = 0;
		}
		else {
			glColor3f(1.0, 1.0, 1.0);
			r.left = 530 + (i-1) * x_scale;
			ggprint16(&r, 0, 0xfffff, tmp.c_str());
		}
	}
}

// This code prints out the logo on our credits screen
void ABarGlobal::printPicture(double x, double y, double z, GLuint texturecode) 
{
	glColor3f(1.0f, 1.0f, 1.0f);
	float wid = 120.0f;
	float image_scale = 1.00;
	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texturecode);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid * image_scale, -wid * image_scale);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid * image_scale, wid * image_scale);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(wid * image_scale, wid * image_scale);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(wid * image_scale, -wid * image_scale);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();
	glPopMatrix();
}


void ABarGlobal::printCredBoxes(int x_pos, int y_pos)
{
	Box box;
	float h, w;
		glColor3f(1.0, 1.0, 1.0);
		box.width = 1920*.48;
		box.height = 1080*.44;
		box.center.x = x_pos;
		box.center.y = y_pos;
		glPushMatrix();
		glTranslatef(box.center.x, box.center.y, 0);
		w = box.width;
		h = box.height;
		glBegin(GL_QUADS);
		glColor3f(0.79f, 1.0f, 0.90f);
		glVertex2i(-w, -h);
		glColor3f(0.31f, 0.26f, 1.0f);
		glVertex2i(-w, h );
		glColor3f(0.35f, 0.80f, 1.0f);
		glVertex2i(w, h );
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(w, -h );
		glEnd();
		glPopMatrix();
}


// Prints out the grey background for the credits screen
void showCreditsBorder()
{
	glColor3f(1.0, 1.0, 1.0);
	Box box;
	box.width = 1920;
	box.height = 1080;
	box.center.x = 960;
	box.center.y = 540;
	float h, w;
	glColor3ub(148, 139, 139);
	glPushMatrix();
	glTranslatef(box.center.x, box.center.y, 0);
	w = box.width;
	h = box.height;
	glBegin(GL_QUADS);
	glVertex2i(-w, -h);
	glVertex2i(-w, h);
	glVertex2i(w, h);
	glVertex2i(w, -h);
	glEnd();
	glPopMatrix();
}
#endif
