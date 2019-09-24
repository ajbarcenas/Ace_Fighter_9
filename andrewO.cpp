// Name: Andrew Oliveros
// Date: 9/17/2019
#include <stdio.h>
#include <GL/glx.h>
#include <math.h>
#include <X11/keysym.h>
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
