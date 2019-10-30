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
	r.left = 960;
	r.bot = 650;
	ggprint16(&r, 16, 0xcf13ac, "HIGH SCORES");
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

void ABarGlobal::incrementScore() 
{
	highscore++;
	cout << "Score: " << highscore << endl;
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
	r.left = 960;
        ggprint16(&r, 16, 0xff1919, "TEMPORARY START SCREEN");
	ggprint16(&r, 16, 0xff1919, "Press S to Start the game");
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
	r.left = 850;
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

void Enemy::makeTest(int x, int y)
{
	if (n >= 5)
		return;
	Dot *p = &test[n];
	p->e.center.x = x;
	p->e.center.y = y;

	p->velocity.y = rand() % 15 + (-6); 
	p->velocity.x = rand () % 4 + (-5);
	++n;
}

void Enemy::testMovement()
{
	if (n <= 0)
		return;
	for (int i = 0; i < n; i++) {
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

void Enemy::printTest()
{
	float w, h;
	for (int i = 0; i < n; i++) {
		glPushMatrix();
		glColor3ub(100,100,100);
		Vec1 *c = &test[i].e.center;
		w = h = 30;
		glBegin(GL_QUADS);
		glVertex2i(c->x-w, c->y-h);
		glVertex2i(c->x-w, c->y+h);
		glVertex2i(c->x+w, c->y+h);
		glVertex2i(c->x+w, c->y-h);
		glEnd();
		glPopMatrix();
	}
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
