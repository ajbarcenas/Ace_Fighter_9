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
    r.left = 1300;
    r.bot = 645;
    r.center = 0;
    ggprint16(&r, 16, 0x069e85, "Alonso Gomez - Background design and movement");
}
/*
void showBackground()
{
    //Background Layers
    //--------------------------------------------------------------------------
    //Mountain Layer
    //

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.mountainTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //Cloud Layer
    //
    glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);
    glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc[2], g.tex.yc[3]); glVertex2i(0, 0);
        glTexCoord2f(g.tex.xc[2], g.tex.yc[2]); glVertex2i(0, g.yres);
        glTexCoord2f(g.tex.xc[3], g.tex.yc[2]); glVertex2i(g.xres, g.yres);
        glTexCoord2f(g.tex.xc[3], g.tex.yc[3]); glVertex2i(g.xres, 0);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

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
