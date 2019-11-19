// Author: Alexisis Barcenas
// Date: 9/17/2019
// Game: Ace Fighter 9
// Program: alexisisB.cpp
// Goals: Have hitboxes work correctly for player and enemy models
/*=============[Notes]=================
Current class in my source file is ABarGlobal
which currently holds general functions that
print out basic information. Current functions are being used to create
the features of the credits page.
Credits page is still a WIP and will continually
be receiving updates.
===============[End]===================
*/
//The four steps to debugging are to identify it, isolate it, fix it and then to
//review it.

#ifndef ALEXISIS_B_CPP
#define ALEXISIS_B_CPP
#include "alexisisB.h"
using namespace std;
ABarGlobal abG;
Enemy eLex;
//Prints out my name on the credits Screen
void printAlexisB(Rect r)
{
	glColor3f(1.0, 1.0, 1.0);
	r.left = 340;
	r.bot = 645;
	r.center = 0;
	ggprint16(&r, 16, 0xcf13ac, "Alexisis Barcenas - Hitbox Specialist");
}

void ABarGlobal::printHighScore(Rect r)
{
	glColor3f(1.0, 1.0, 1.0);
	r.left = 835;
	r.bot = 650;
	ggprint16(&r, 16, 0xcf13ac, "         HIGH SCORES");
	ggprint16(&r, 16, 0xcf13ac, " Press T to increment scores");
}
/*Prints out "Ace Fighter 9" in a cross format across the screen
Not sure whether I like it, so might be commented out for progress report
*/
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
		
		if (i <= 4) {
			glColor3f(1.0, 1.0, 1.0);
			r.left = 485 + i * x_scale;
			ggprint16(&r, 0, 0xfffff, tmp.c_str());
		} else if (i == 5) {
			r.center = 0;
		} else {
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
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid * image_scale,
		-wid * image_scale);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid * image_scale,
		wid * image_scale);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(wid * image_scale, wid * image_scale);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(wid * image_scale, -wid * image_scale);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();
	glPopMatrix();
}

/* This functions prints out the box that the pictures will be placed on.
I used vertex-color blending to give the box that gradient look to it.
 */
void ABarGlobal::printCredBoxes(int x_pos, int y_pos)
{
	Box box;
	float h, w;
		glColor3f(1.0, 1.0, 1.0);
		box.width = 1920 * .48;
		box.height = 1080 * .44;
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
		glVertex2i(-w, h);
		glColor3f(0.35f, 0.80f, 1.0f);
		glVertex2i(w, h);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(w, -h);
		glEnd();
		glPopMatrix();
}


/* Prints out the grey background for the credits screen and picture frames
still working on the outermost border because the window size is too large to
view from my local machine. Will need to continue working on this at school
machines because of their higher resolution/dimensions.
*/
void showCreditsBorder(int width, int height, int x_pos, int y_pos)
{
	glColor3f(1.0, 1.0, 1.0);
	Box box;
	box.width = width;
	box.height = height;
	box.center.x = x_pos;
	box.center.y = y_pos;
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
// Overloading the function so different colors can be assigned to the border
void showCreditsBorder(int width, int height, int x_pos, int y_pos,
	int r_color, int g_color, int b_color) 
{
	glColor3f(1.0, 1.0, 1.0);
	Box box;
	box.width = width;
	box.height = height;
	box.center.x = x_pos;
	box.center.y = y_pos;
	float h, w;
	box.color.r = r_color;
	box.color.g = g_color;
	box.color.b = b_color;
	glColor3ub(box.color.r, box.color.g, box.color.b);
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

void ABarGlobal::incrementScore(int points) 
{
	highscore += points;
}

void ABarGlobal::showHighScores()
{
	showHigh ^= 1;
	if (showCreds == 1) {
		showCreds = 0;
	}
}

void ABarGlobal::showCredits()
{
	showCreds ^= 1;
	if (showHigh == 1) {
		showHigh = 0;
	}
}
void ABarGlobal::showStartScreen()
{
	showStart ^= 1;
}

void ABarGlobal::printTempScreen(Rect r)
{
        glColor3f(1.0, 1.0, 1.0);
	r.bot = 800;
	r.left = 825;
        ggprint16(&r, 16, 0xff1919, "TEMPORARY START SCREEN");
	ggprint16(&r, 16, 0xff1919, "   Press S to Start the game");
}
void ABarGlobal::drawButton(int x_pos, int y_pos)
{
	glColor3f(1.0, 1.0, 1.0);
	Box box;
	box.width = 150;
	box.height = 50;
	box.center.x  = x_pos;
	box.center.y = y_pos;
	float h, w; 
	glColor3ub(255, 255, 255);
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

void ABarGlobal::drawTriangle(int width, int height, int x_pos, int y_pos,
	int rColor, int gColor, int bColor)
{
	glColor3f(1.0, 1.0, 1.0);
	Triangle triangle;
	triangle.width = width;
	triangle.height = height;
	triangle.center.x = x_pos;
	triangle.center.y = y_pos;
	float h, w;
	triangle.color.r = rColor;
	triangle.color.g = gColor;
	triangle.color.b = bColor;
	glColor3ub(triangle.color.r, triangle.color.g, triangle.color.b);
	glPushMatrix();
	glTranslatef(triangle.center.x, triangle.center.y, 0);
	w = triangle.width;
	h = triangle.height;
	glBegin(GL_TRIANGLES);
		glVertex2i(-w, -h);
		glVertex2i(-w/10, h);
		glVertex2i(w,-h);
	glEnd();
	glPopMatrix();
}

void ABarGlobal::printTempScores(Rect r)
{
	glColor3f(1.0, 1.0, 1.0);
	r.left = 800;
	r.bot = 575;
	ggprint16(&r, 50, 0xcf13ac, "Test Person");
	ggprint16(&r, 50, 0xcf13ac, "Test Person 2");
	ggprint16(&r, 50, 0xcf13ac, "Test Person 3");
	ggprint16(&r, 50, 0xcf13ac, "Test Person 4");
}
void ABarGlobal::colorBlendBorder(int height, int width, int x_pos, int y_pos,
		int rColor, int gColor, int bColor, int rColor2, int gColor2,
		int bColor2, int rColor3, int gColor3, int bColor3,
		int rColor4, int gColor4, int bColor4)
{
	Box box;
	float h, w;
	glColor3f(1.0, 1.0, 1.0);
	box.width = width;
	box.height = height;
	box.center.x= x_pos;
	box.center.y = y_pos;
	glPushMatrix();
	glTranslatef(box.center.x, box.center.y, 0);
	w = box.width;
	h = box.height;
	glBegin(GL_QUADS);
	box.color.r = rColor;
	box.color.g = gColor;
	box.color.b = bColor;
	glColor3ub(box.color.r, box.color.g, box.color.b);
	glVertex2i(-w, -h);
	box.color.r = rColor2;
	box.color.g = gColor2;
	box.color.b = bColor2;
	glColor3ub(box.color.r, box.color.g, box.color.b);
	glVertex2i(-w, h);
	box.color.r = rColor3;
	box.color.g = gColor3;
	box.color.b = bColor3;
	glColor3ub(box.color.r, box.color.g, box.color.b);
	glVertex2i(w, h);
	box.color.r = rColor4;
	box.color.g = gColor4;
	box.color.b = bColor4;
	glColor3ub(box.color.r, box.color.g, box.color.b);
	glVertex2i(w, -h);
	glEnd();
	glPopMatrix();
			
}

void ABarGlobal::condenseCreds()
{
	showCreditsBorder(1920, 1080, 960, 540);
	printCredBoxes(960, 540);
	showCreditsBorder(180, 180, 960, 540, 14, 14, 138 );
	showCreditsBorder(170, 170, 960, 540, 21, 21, 237);
	showCreditsBorder(160, 160, 960, 540, 47, 47, 237);
	showCreditsBorder(150, 150, 960, 540, 69, 80, 237);
	showCreditsBorder(140, 140, 960, 540, 39, 123, 232);
	showCreditsBorder(130, 130, 960, 540);
	showCreditsBorder(130, 130, 480, 800);
	showCreditsBorder(130, 130, 1440, 800);
	showCreditsBorder(130, 130, 480, 300);
	showCreditsBorder(130, 130, 1440, 300);
	printAlexisB(r);
	glColor3f(1.0, 1.0, 1.0);
	r.bot = 145, r.left = 340;
	ggprint16(&r, 16, 0xcf13ac, "Diego Diaz- Player and Enemy Movement");
	r.bot = 145, r.left = 1300;
	ggprint16(&r, 16, 0xcf13ac,
		"Andrew Oliveros- HUD Creation/Sprites/Menu");
}

void ABarGlobal::condenseHigh()
{
    	showCreditsBorder(1920, 1080, 960, 540);
	printCredBoxes(960, 540);
	showCreditsBorder(210, 210, 960, 540, 1, 1, 1);
	colorBlendBorder(200, 200, 960,540,0,0,0,
			255,255,255,
			0,0,0,
			255,255,255);
	printHighScore(r);
	printTempScores(r);
}

void ABarGlobal::condenseStart()
{
	colorBlendBorder(980, 1920, 960,540,230,0,0,
			25,255,255,
			34,204,0,
			204,230,255);
	drawButton(960, 800);
	drawButton(960, 600);
	drawButton(960, 400);
	drawButton(960, 200);
	printTempScreen(r);	
}
void Enemy::makeTest()
{
	if (numEnemy >= 5)
		return;
	Dot *p = &test[numEnemy];
	p->e.center.x = 1920;
	p->e.center.y = rand() % 1081;
	
	if (abG.highscore >= 15000) {
		p->velocity.y = rand() % 15 + (-6); 
		p->velocity.x = rand () % 4 + (-8);
	} if (abG.highscore >= 50000) {	
		p->velocity.y = rand() % 15 + (-6); 
		p->velocity.x = rand () % 4 + (-17);
	} else {
		p->velocity.y = rand() % 15 + (-6); 
		p->velocity.x = rand () % 4 + (-5);
	}
	++numEnemy;
}

void Enemy::makeCEnem()
{
	if (numCEnemy >= 20)
		return;
	Dot *f = &cEnemy[numCEnemy];

	if (numCEnemy == 0) {	
		f->e.center.x = 1920;
		f->e.center.y = 1000;
	} else if (numCEnemy == 1) {
		f->e.center.x = 2120;
		f->e.center.y = 1000;
	} else if (numCEnemy == 2) {
		f->e.center.x = 2320;
		f->e.center.y = 1000;
	} else if (numCEnemy == 3) {
		f->e.center.x = 2520;
		f->e.center.y = 1000;
	
	} else if (numCEnemy == 4) {
		f->e.center.x = 2000;
		f->e.center.y = 800;
	} else if (numCEnemy == 5) {
		f->e.center.x = 2200;
		f->e.center.y = 800;
	} else if (numCEnemy == 6) {
		f->e.center.x = 2400;
		f->e.center.y = 800;
	} else if (numCEnemy == 7) {
		f->e.center.x = 2600;
		f->e.center.y = 800;
	
	} else if (numCEnemy == 8) {
		f->e.center.x = 1920;
		f->e.center.y = 600;
	} else if (numCEnemy == 9) {
		f->e.center.x = 2120;
		f->e.center.y = 600;
	} else if (numCEnemy == 10) {
		f->e.center.x = 2320;
		f->e.center.y = 600;
	} else if (numCEnemy == 11) {
		f->e.center.x = 2520;
		f->e.center.y = 600;
	
	} else if (numCEnemy == 12) {
		f->e.center.x = 2000;
		f->e.center.y = 400;
	} else if (numCEnemy == 13) {
		f->e.center.x = 2200;
		f->e.center.y = 400;
	} else if (numCEnemy == 14) {
		f->e.center.x = 2400;
		f->e.center.y = 400;
	} else if (numCEnemy == 15) {
		f->e.center.x = 2600;
		f->e.center.y = 400;
	
	} else if (numCEnemy == 16) {
		f->e.center.x = 1920;
		f->e.center.y = 200;
	} else if (numCEnemy == 17) {
		f->e.center.x = 2120;
		f->e.center.y = 200;
	} else if (numCEnemy == 18) {
		f->e.center.x = 2320;
		f->e.center.y = 200;
	} else if (numCEnemy == 19) {
		f->e.center.x = 2520;
		f->e.center.y = 200;
	}
	
	// This is my Friday code
	// As the score increases, the game's enemies gradually increase in
	// speed.
	if (abG.highscore >= 15000) {
		f->velocity.y = 0; 
		f->velocity.x = -6;
	} if (abG.highscore >= 50000) {	
		f->velocity.y = 0; 
		f->velocity.x = -8;
	} if (abG.highscore >= 100000) {
		f->velocity.y = 0;
		f->velocity.x = -10;
	} else {
		f->velocity.y = 0; 
		f->velocity.x = -4;
	}
	++numCEnemy;
}

void Enemy::makeVEnem()
{
	if (numVEnemy >= 5)
		return;
	Dot *v = &vEnemy[numVEnemy];
	if (numVEnemy == 0) {	
		v->e.center.x = 2320;
		v->e.center.y = 1000;
	} else if (numVEnemy == 1) {
		v->e.center.x = 2120;
		v->e.center.y = 800;
	} else if (numVEnemy == 2) {
		v->e.center.x = 1920;
		v->e.center.y = 600;
	} else if (numVEnemy == 3) {
		v->e.center.x = 2120;
		v->e.center.y = 400;
	} else if (numVEnemy == 4) {
		v->e.center.x = 2320;
		v->e.center.y = 200;
	}
		
	// This is my Friday code
	// As the score increases, the game's enemies gradually increase in
	// speed.
	if (abG.highscore >= 15000) {
		v->velocity.y = 0; 
		v->velocity.x = -6;
	} if (abG.highscore >= 50000) {	
		v->velocity.y = 0; 
		v->velocity.x = -8;
	} if (abG.highscore >= 100000) {
		v->velocity.y = 0;
		v->velocity.x = -10;
	} else {
		v->velocity.y = 0; 
		v->velocity.x = -4;
	}
	++numVEnemy;
}

void Enemy::testMovement()
{
	if (numEnemy <= 0)
		return;
	for (int i = 0; i < numEnemy; i++) {
		Dot *p = &test[i];
		p->e.center.x += p->velocity.x;
		p->e.center.y += p->velocity.y;

		if (p->e.center.y < 0.0 || p->e.center.y > 1080) {
			p->velocity.y = -(p->velocity.y); 
		}

		if (p->e.center.x < 0.0)
			p->e.center.x = 1920;

		enemyX[i] = p->e.center.x;
		enemyY[i] = p->e.center.y;
	}
}

void Enemy::cEnemMovement()
{
	if (numCEnemy <= 0)
		return;
	for (int i = 0; i < numCEnemy; i++) {
		Dot *f = &cEnemy[i];

		f->e.center.x += f->velocity.x;
		f->e.center.y += f->velocity.y;
	if (f->e.center.y < 0.0 || f->e.center.y > 1080) {
		f->velocity.y = -(f->velocity.y);
	}
		if (f->e.center.x < 0.0)
			f->e.center.x = 1920;
	}
}

void Enemy::vEnemMovement()
{
	if (numVEnemy <= 0)
		return;
	for (int i = 0; i < numVEnemy; i++) {
		Dot *v = &vEnemy[i];
		v->e.center.x += v->velocity.x;
		v->e.center.y += v->velocity.y;
		if (v->e.center.y < 0.0 || v->e.center.y > 1080) {
			v->velocity.y = -(v->velocity.y);
		}
		if (v->e.center.x < 0.0)
			eLex.deleteVEnemy(i);
	}
}

void Enemy::printTest(float w, float h, GLuint Texture)
{
	for (int i = 0; i < numEnemy; i++) {
		glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
		glColor3ub(255,255,255);
		Vec1 *c = &test[i].e.center;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(c->x-w*.5, c->y-h*.5);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(c->x-w*.5, c->y+h*.5);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(c->x+w*.5, c->y+h*.5);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(c->x+w*.5, c->y-h*.5);
		glEnd();
		glPopMatrix();
	}
}

void Enemy::printCEnem(float w, float h, GLuint Texture)
{
	for (int i = 0; i < numCEnemy; i++) {
		glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
		glColor3ub(255,255,255);
		Vec1 *g = &cEnemy[i].e.center;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(g->x-w*.1, g->y-h*.1);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(g->x-w*.1, g->y+h*.1);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g->x+w*.1, g->y+h*.1);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g->x+w*.1, g->y-h*.1);
		glEnd();
		glPopMatrix();
	}
}
void Enemy::printVEnem(float w, float h, GLuint Texture)
{
	for (int i = 0; i < numVEnemy; i++) {
		glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
		glColor3ub(255,255,255);
		Vec1 *q = &vEnemy[i].e.center;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(q->x-w*.5, q->y-h*.5);	
			glTexCoord2f(0.0f, 0.0f); glVertex2i(q->x-w*.5, q->y+h*.5);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(q->x+w*.5, q->y+h*.5);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(q->x+w*.5, q->y-h*.5);
		glEnd();
		glPopMatrix();
	}
}
void Enemy::deleteEnemy(int i)
{
	test[i] = test[numEnemy - 1];
	--numEnemy;
}

int Enemy::getMAXENEMIES()
{
	return MAXENEMIES;
}
// This is my Friday code.
// Finishing adding my get functions for Alonso's deletion.
int Enemy::getCHECKMAXENEM()
{
	return CHECKMAXENEM;
}
int Enemy::getEX(int i)
{
	Dot *p = &test[i];
	return p->e.center.x;
}

int Enemy::getEY(int i)
{
	Dot *p = &test[i];
	return p->e.center.y;
}

int Enemy::getNumEnemy()
{
	return numEnemy;
}

void Enemy::deleteVEnemy(int i)
{
	vEnemy[i] = vEnemy[numVEnemy - 1];
    --numVEnemy;
}

int Enemy::getVX(int i) 
{
	Dot *v = &vEnemy[i];
	return v->e.center.x;
}
int Enemy::getVY(int i)
{
	Dot *v = &vEnemy[i];
	return v->e.center.y;
}

int Enemy::getVNumEnemy()
{
	return numVEnemy;
}

void Enemy::deleteCEnemy(int i)
{
	cEnemy[i] = cEnemy[numCEnemy - 1];
    --numCEnemy;
}

int Enemy::getCX(int i) 
{
	Dot *f = &cEnemy[i];
	return f->e.center.x;
}
int Enemy::getCY(int i)
{
	Dot *f = &cEnemy[i];
	return f->e.center.y;
}

int Enemy::getCNumEnemy()
{
	return numCEnemy;
}

void Enemy::makeBoss(int x, int y)
{
	if (m >= 1)
		return;
	Dot *b = &boss[m];
	b->e.center.x = x;
	b->e.center.y = y;

	b->velocity.y = 2;
	b->velocity.x = 0;
	++m;
}

void Enemy::bossMovement()
{
	if (m <= 0)
		return;
	for (int i = 0; i < m; i++) {
		Dot *b = &boss[i];
		b->e.center.x += b->velocity.x;
		b->e.center.y += b->velocity.y;

		if (b->e.center.y < 0.0 || b->e.center.y > 1080) {
			b->velocity.y = -(b->velocity.y);
		}
		bossX = b->e.center.x;
		bossY = b->e.center.y;
	}
}

void Enemy::printBoss()
{
	float w, h;
	for (int i = 0; i < m; i++) {
		glPushMatrix();
		glColor3ub(0, 255, 255);
		Vec1 *d = &boss[i].e.center;
		w = 200;
		h = 150;
		glBegin(GL_QUADS);
			glVertex2i(d->x-w, d->y-h);
			glVertex2i(d->x-w, d->y+h);
			glVertex2i(d->x+w, d->y+h);
			glVertex2i(d->x+w, d->y-h);
		glEnd();
		glPopMatrix();
	
	}
}


void Enemy::makeEBullet(int x, int y)
{
	if (o >= 20)
		return;
	Dot *a = &bullets[o];
	a->e.center.x = x;
	a->e.center.y = y;

	a->velocity.y = rand() % 4 + (-2);
	a->velocity.x = -2;
	++o;
}

void Enemy::bulletMovement()
{
	if (o <= 0)
		return;
	for (int i = 0; i < o; i++) {
		Dot *a = &bullets[i];
		a->e.center.x += a->velocity.x;
		a->e.center.y += a->velocity.y;

		if (a->e.center.x < 0.0) {
			bullets[i] = bullets[o - 1];
			--o;
		}
	}
}

void Enemy::printEBullet()
{
	float w, h;
	for (int i = 0; i < o; i++) {
		glPushMatrix();
		glColor3ub(0, 0, 255);
		Vec1 *g = &bullets[i].e.center;
		w = h = 4;
		glBegin(GL_QUADS);
			glVertex2i(g->x-w, g->y-h);
			glVertex2i(g->x-w, g->y+h);
			glVertex2i(g->x+w, g->y+h);
			glVertex2i(g->x+w, g->y-h);
		glEnd();
		glPopMatrix();
	}
}
#endif
