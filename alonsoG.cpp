/*
Name: Alonso Gomez
Game: Ace Fighter 9
Goals: Have multiple background layers moving at different speeds
       to give a more depth visual to our game.
*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"

void showAlonsoText(Rect r) {
    r.left = 40;
    r.bot = 80;
    r.center = 0;
    ggprint16(&r, 16, 0x069e85, "Alonso Gomez - Background design and movement");
}
/*
void showAlonsoPicture(int x, int y, GLuint texid)
{
    static float angle = 0.0f;
    float fx = (float)x;
    float fy = (float)y;
    double g = 0.0;

    g += sin(angle) * 10.0f;
    fx += g;
    fy += g + 2.0;
    glColor3ub(250, 250, 250);
    int wid = 50;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, -wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();
    glPopMatrix();
}
*/
