//Alonso Gomez
//10/09/19
//CMPS 3350
//Quiz
//Step 1: Reproduce
//Step 2: Find
//Step 3: Fix
//Step 4: Test



/*
Name: Alonso Gomez
Game: Ace Fighter 9
Goals: 
1.Have multiple background layers moving at different speeds
  to give a more depth visual to our game.
2.Add smoke particles to follow the player

*/

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"

const int MAX_PARTICLES = 8000;
const float GRAVITY = .05;

void showAlonsoText(Rect r) {
    r.left = 1300;
    r.bot = 645;
    r.center = 0;
    ggprint16(&r,16,0x069e85,"Alonso Gomez - Background design and movement");
}

struct Vec {
    float x, y, z;
};

struct Shape {
    float width;
    float height;
    float radius;
    Vec center;
};

struct Particle {
    int r, g, b;
    Shape s;
    Vec velocity;
};

class AlonsoGlobal {
public:
    int xres, yres;
    Particle smoke[MAX_PARTICLES];
    Particle bullet[MAX_PARTICLES];
    Particle confetti[MAX_PARTICLES];
    int n = 0;
    //AlonsoGlobal();
}ag;

void makeConfetti()
{
    if (ag.n >= MAX_PARTICLES)
        return;

    Particle *c = &ag.confetti[ag.n];

    c->s.center.x = rand() % 1921;
    c->s.center.y = 1080;

    c->velocity.y = -0.8;
    c->velocity.x = 0;
    ++ag.n;

}

void confettiMovement()
{
    if (ag.n <= 0)
        return;

    for (int i = 0; i < ag.n; i++) {
        Particle *c = &ag.confetti[i];
        c->s.center.x += c->velocity.x;
        c->s.center.y += c->velocity.y;
        c->velocity.y = c->velocity.y - GRAVITY;

        //check for off screen
        if (c->s.center.y < 0.0 || c->s.center.y > 1080 ||
            c->s.center.x < 0.0 || c->s.center.y > 1920) {
            ag.confetti[i] = ag.confetti[ag.n - 1];
            --ag.n;
        }
    }
}

void printConfetti()
{
    float w, h;

    for (int i = 0; i < ag.n; i++) {
        if (i % 7 == 0) {
            ag.confetti[i].r = 0;
            ag.confetti[i].g = 128;
            ag.confetti[i].b = 255;
        }
        if (i % 7 == 1) {
            ag.confetti[i].r = 0;
            ag.confetti[i].g = 255;
            ag.confetti[i].b = 128;
        }
        if (i % 7 == 2) {
            ag.confetti[i].r = 255;
            ag.confetti[i].g = 247;
            ag.confetti[i].b = 0;
        }
        if (i % 7 == 3) {
            ag.confetti[i].r = 255;
            ag.confetti[i].g = 0;
            ag.confetti[i].b = 0;
        }
        if (i % 7 == 4) {
            ag.confetti[i].r = 255;
            ag.confetti[i].g = 0;
            ag.confetti[i].b = 255;
        }
        if (i % 7 == 5) {
            ag.confetti[i].r = 0;
            ag.confetti[i].g = 0;
            ag.confetti[i].b = 255;
        }
        if (i % 7 == 6) {
            ag.confetti[i].r = 0;
            ag.confetti[i].g = 255;
            ag.confetti[i].b = 0;
        }

        glPushMatrix();
        glColor3ub(ag.confetti[i].r, ag.confetti[i].g, ag.confetti[i].b);
        Vec *v = &ag.confetti[i].s.center;
        w = h = 3;
        glBegin(GL_QUADS);
            glVertex2i(v->x-w, v->y-h);
            glVertex2i(v->x-w, v->y+h);
            glVertex2i(v->x+w, v->y+h);
            glVertex2i(v->x+w, v->y-h);
        glEnd();
        glPopMatrix();
    }

}

void makeSmoke(int x, int y)
{
    if (ag.n >= MAX_PARTICLES)
        return;
    Particle *p = &ag.smoke[ag.n];

    p->s.center.x = x;
    
    if (rand() % 16 > 8)
        p->s.center.y = y + (rand() % 8);
    else
        p->s.center.y = y - (rand() % 8);

    p->velocity.y = 0;
    p->velocity.x = - ((double)rand() / (double)RAND_MAX) - 5;
    ++ag.n;
}

void smokeMovement()
{
    if (ag.n <= 0)
        return;
    
    for (int i = 0; i < ag.n; i++) {
        Particle *p = &ag.smoke[i];
        p->s.center.x += p->velocity.x;
        p->s.center.y += p->velocity.y;
       // p->velocity.y = p->velocity.y - GRAVITY;

        //check for off screen
        if (p->s.center.y < 0.0 || p->s.center.y > 1080 ||
            p->s.center.x < 0.0 || p->s.center.x > 1920) {
            ag.smoke[i] = ag.smoke[ag.n - 1];
            --ag.n;
        }
    }    
}

void printSmoke()
{
    float w, h;

    for (int i = 0; i < ag.n; i++) {
        if (i % 2 == 0)
            ag.smoke[i].r = ag.smoke[i].g = ag.smoke[i].b = 115;
        else
            ag.smoke[i].r = ag.smoke[i].g = ag.smoke[i].b = 220;
        if (i % 8 == 2)
            ag.smoke[i].r = ag.smoke[i].g = ag.smoke[i].b = 175;

        glPushMatrix();
        glColor3ub(ag.smoke[i].r, ag.smoke[i].g, ag.smoke[i].b);
        Vec *c = &ag.smoke[i].s.center;
        w = h = 3;
        glBegin(GL_QUADS);
            glVertex2i(c->x-w, c->y-h);
            glVertex2i(c->x-w, c->y+h);
            glVertex2i(c->x+w, c->y+h);
            glVertex2i(c->x+w, c->y-h);
        glEnd();
        glPopMatrix();
    }
}

void makeBullet(int x, int y)
{
    if (ag.n >= MAX_PARTICLES)
        return;

    Particle *b = &ag.bullet[ag.n];

    b->s.center.x = x;
    b->s.center.y = y;
    b->velocity.y = 0;
    b->velocity.x = ((double)rand() / (double)RAND_MAX) + 20;
    ++ag.n;
}

void bulletMovement()
{
    if (ag.n <= 0)
        return;

    for (int i = 0; i < ag.n; i++) {
        Particle *b = &ag.bullet[i];
        b->s.center.x += b->velocity.x;
        b->s.center.y += b->velocity.y;
        //b->s.velocity.y = b->velocity.y - GRAVITY;

        //check for off screen
        if (b->s.center.y < 0.0 || b->s.center.y > 1080 ||
            b->s.center.x < 0.0 || b->s.center.x > 1920) {
            ag.bullet[i] = ag.bullet[ag.n - 1];
            --ag.n;
        }
    }
}

void printBullet()
{
    float w, h;

    for (int i = 0; i < ag.n; i++) {
        ag.bullet[i].r = 255;
        ag.bullet[i].g = 0;
        ag.bullet[i].b = 0;

        glPushMatrix();
        glColor3ub(ag.bullet[i].r, ag.bullet[i].g, ag.bullet[i].b);
        Vec *c = &ag.bullet[i].s.center;
        w = h = 2;
        glBegin(GL_QUADS);
            glVertex2i(c->x-w, c->y-h);
            glVertex2i(c->x-w, c->y+h);
            glVertex2i(c->x+w, c->y+h);
            glVertex2i(c->x+w, c->y-h);
        glEnd();
        glPopMatrix();
    }
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
