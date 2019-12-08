// Author: Alexisis Barcenas
// Game: Ace Fighter 9
// Program: alexisisB.h
// Date Created: 9/26/19
/*=============[Notes]=================
Current class in my header file is ABarGlobal
which currently holds general functions that
print out basic information.
Current functions are being used to create
the features of the credits page.
Credits page is still a WIP and will periodically
be receiving updates.
  */

#ifndef ALEXISIS_B_H
#define ALEXISIS_B_H

#include <iostream>
#include "fonts.h"
#include <string.h>
#include <GL/glx.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
const int MAXRANDOM = 6;
const int MAXENEMIES = 5;
const int CHECKMAXENEM = 20;
const float GF = 0.0;

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
	Color color;
};

struct Triangle {
	float width, height;
	Color color;
	Vec1 center;	
};

struct Model {
	float width;
	float height;
	float radius;
	Vec1 center;
};

struct Dot {
    GLuint texture;
	Color c;
	Model e;
	Vec1 velocity;
};

class Enemy {
	public:
		
		Dot test[MAXRANDOM];
		Dot vEnemy[MAXENEMIES];
		Dot cEnemy[CHECKMAXENEM];
		int getCHECKMAXENEM();
        void deleteEnemy(int i);
        int getMAXENEMIES();
        int getEX(int i);
        int getEY(int i);
		void deleteVEnemy(int i);
		int getVX(int i);
		int getVY(int i);
		int getVNumEnemy();
		int enemyX[MAXENEMIES];
		int enemyY[MAXENEMIES];
		int numEnemy = 0;
		int numVEnemy = 0;
		int numCEnemy = 0;
		int getNumEnemy();
		int m = 0;
		int o = 0;
		int v = 0;
		int r = 0;
		Dot boss[1];
		void makeBoss(int x, int y);
		void bossMovement();
		void printBoss();
		int bossX, bossY;
		void makeEBullet(int x, int y);
		void bulletMovement();
		void printEBullet();
		Dot bullets[20];
		void makeTest();
		void testMovement(int &currentHealth);
		void printTest(float w, float h, GLuint Texture1, GLuint Texture2,
                       GLuint Texutre3, GLuint Texture4, GLuint Texture5,
                       GLuint Texture6);
		void makeVEnem();
		void vEnemMovement(int &currentHealth);
		void printVEnem(float w, float h, GLuint Texture);
		void makeCEnem();
		void cEnemMovement(int &currentHealth);
		void printCEnem(float w, float h, GLuint Texture);
		int getCX(int i);
		int getCY(int i);
		int getCNumEnemy();
		void deleteCEnemy(int i);		
		bool increasing = true;
        bool rDead = true;
        bool vDead = true;
        bool cDead = true;
		float cx = 1.0;
		float cy = 0.0;
};

class ABarGlobal {
	public:
		Rect r;
		int highscore;
		int showHigh;
		int showCreds;
		int showHow;
		int showGameIsOver;
		int showStart = 1;
		void printAceFighter9(Rect r);
		void printPicture(double x, double y, double z, GLuint texturecode);
		void printCredBoxes(int x_pos, int y_pos);
		void printHighScore(Rect r);
		void incrementScore(int points);
		void showHighScores();
		void showCredits();
		void showGameOver(int health);
		void showStartScreen();
		void showHowTo();
		void drawTriangle(int width, int height, int x_pos, int y_pos,
			int rColor, int gColor, int bColor);
		void printTempScores(Rect r);
		void drawCircle();
		void printTempScreen(Rect r);
		void drawButton(int x_pos, int y_pos);
		void colorBlendBorder(int width, int height, int x_pos,
			       	int y_pos, int rColor, int gColor, int bColor,
				int rColor2, int gColor2, int bColor2,
				int rColor3, int gColor3, int bColor3,
				int rColor4, int gColor4, int bColor4);
		void condenseCreds();
		void condenseHigh();
		void condenseStart();
		void condenseHow();
		void printInstructions(Rect r);
		void printCredits(Rect r);
};

void printAlexisB(Rect r);
void showCreditsBorder(int width, int height, int x_pos, int y_pos);
void showCreditsBorder(int width, int height, int x_pos, int y_pos,
	int r_color, int g_color, int b_color);
#endif
