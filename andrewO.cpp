// Name: Andrew Oliveros
// Date: 9/17/2019
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"
/*
 void ajPic(int x, int y, GLuint textid)
{
	//show pic
	static float angle = 0.0f;
	float fx = (float)x;
	float fy = (float)y;
	double g = 0.0;
	g+= sin(angle) * 10.0f;
	fx+=g; 
	fy+=g+ 2.0;
	angle+= 0.5f;
	glColor3ub(255,55,255);
	int width=50;
	glPushMatrix();
	glTranslate(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertext2i(-width, -width);
	glTexCoord2f(0.0f, 1.0f); glVertext2i(-width,  width);
	glTexCoord2f(0.0f, 1.0f); glVertext2i( width,  width);
	glTexCoord2f(0.0f, 1.0f); glVertext2i( width, -width);
	glEnd();
	glPopMatrix();
}
*/

void ajPrint(Rect r) {
	r.bot = 20;
	r.left = 40;
	r.center = 0;
	ggprint16(&r, 16, 0x0084ff, "ANDREW OLIVEROS");

}
/*
void initButtons(void) { 

nbuttons=0;
	//Quit button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 50;
	button[nbuttons].r.left = 10;
	button[nbuttons].r.bot = 10;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx =
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery =
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	
	//
	//Reset button
	//size and position
	button[nbuttons].r.width = 200;
	button[nbuttons].r.height = 100;
	button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 50;
	button[nbuttons].r.right =
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx =
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery =
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Reset Grids");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
}


void mouse_click(int ibutton, int action, int x, int y)
{
	int con;
	if (action == 1) {
		int i,j,k,m=0;
		//center of a grid
		int cent[2];
		//
		//buttons?
		for (i=0; i<nbuttons; i++) {
			if (button[i].over) {
				button[i].down = 1;
				button[i].click = 1;
				if (i==0) {
					//user clicked QUIT
					done = 1;
				}
				if (i==1) {
					//user clicked button 0
					reset_grids();
				}
			}
		}
	}
}
*/
