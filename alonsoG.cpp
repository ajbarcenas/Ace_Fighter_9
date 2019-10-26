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
#include <iostream>
using namespace std;

const int MAX_PARTICLES = 8000;
const float GRAVITY = .05;

void showAlonsoText(Rect r)
{
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
    Particle rain[MAX_PARTICLES];
    int n = 0;
    int u = 0;
    int k = 0;
    int q = 0;
    float xr = 1.0;
    float yr = 1.0;
    float cx = 1.0;
    float cy = 0.0;
    bool increasing = true;
    //AlonsoGlobal();
}ag;

void cubePower()
{
    //Cube rotation
    ag.xr = ag.xr + 6;
    ag.yr = ag.yr + 6;

    //Cube goes up and down
    if (ag.increasing){
        ag.cy = ag.cy + 0.05;
        if (ag.cy >= 1.0)
            ag.increasing = false;
    }
    else {
        ag.cy = ag.cy - 0.05;
        if (ag.cy <= -1.0)
            ag.increasing = true;
    }

    //Cube moves from right to left
    if (ag.cx > -1.0)
        ag.cx = ag.cx - 0.01;
    if (ag.cx <= -1.0)
        ag.cx = 1.0;

    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(ag.cx, ag.cy, 0.1);
    glRotatef(ag.xr, 1.0, 0.0, 0.0);
    glRotatef(ag.yr, 0.0, 1.0, 0.0);

    //Front
    glBegin(GL_QUADS);
        glColor3f(0.54f, 0.00f, 0.86f);  
        glVertex3f(-0.05f, -0.05f, 0.05f);
        glVertex3f(-0.05f,  0.05f, 0.05f);
        glVertex3f( 0.05f,  0.05f, 0.05f);
        glVertex3f( 0.05f, -0.05f, 0.05f);
    //Back
        glColor3f(0.54f, 0.00f, 0.86f); 
        glVertex3f(-0.05f, -0.05f, -0.05f);
        glVertex3f(-0.05f,  0.05f, -0.05f);
        glVertex3f( 0.05f,  0.05f, -0.05f);
        glVertex3f( 0.05f, -0.05f, -0.05f);
    //Right
        glColor3f(0.51f, 0.00f, 0.80f); 
        glVertex3f(0.05f, -0.05f,  0.05f);
        glVertex3f(0.05f,  0.05f,  0.05f);
        glVertex3f(0.05f,  0.05f, -0.05f);
        glVertex3f(0.05f, -0.05f, -0.05f);
    //Left  
        glColor3f(0.51f, 0.00f, 0.80f); 
        glVertex3f(-0.05f, -0.05f, -0.05f);
        glVertex3f(-0.05f,  0.05f, -0.05f);
        glVertex3f(-0.05f,  0.05f,  0.05f);
        glVertex3f(-0.05f, -0.05f,  0.05f);
    //Top
        glColor3f(0.49f, 0.00f, 0.78f); 
        glVertex3f(-0.05f, 0.05f, -0.05f);
        glVertex3f(-0.05f, 0.05f,  0.05f);
        glVertex3f( 0.05f, 0.05f,  0.05f);
        glVertex3f( 0.05f, 0.05f, -0.05f);
    //Bottom
        glColor3f(0.49f, 0.00f, 0.78f); 
        glVertex3f(-0.05f, -0.05f,  0.05f);
        glVertex3f(-0.05f, -0.05f, -0.05f);
        glVertex3f( 0.05f, -0.05f, -0.05f);
        glVertex3f( 0.05f, -0.05f,  0.05f);
    glEnd();

    glFlush();
}

void makeRain()
{
    if (ag.k >= MAX_PARTICLES)
        return;

    Particle *r = &ag.rain[ag.k];

    r->s.center.x = rand() % 1921;
    r->s.center.y = 1080;
    r->velocity.y = -5.0;
    r->velocity.x = -0.2;
    ++ag.k;
}

void rainMovement()
{
    if (ag.k <= 0)
        return;

    for (int i = 0; i < ag.k; i++) {
        Particle *r = &ag.rain[i];
        r->s.center.x += r->velocity.x;
        r->s.center.y += r->velocity.y;
        r->velocity.y = r->velocity.y - GRAVITY;

        //check for off screen
        if (r->s.center.y < 0.0 || r->s.center.y > 1080 ||
            r->s.center.x < 0.0 || r->s.center.y > 1920) {
            ag.rain[i] = ag.rain[ag.k - 1];
            --ag.k;
        }
    }
}

void printRain()
{
    float w, h;
    
    for(int i = 0; i < ag.k; i++) {
        glPushMatrix();
        glColor4ub(20,52,255,150);
        Vec *v = &ag.rain[i].s.center;
        w = h = 2;
        glBegin(GL_QUADS);
            glVertex2i(v->x-w, v->y-h);
            glVertex2i(v->x-w, v->y+h);
            glVertex2i(v->x+w, v->y+h);
            glVertex2i(v->x+w, v->y-h);
        glEnd();
        glPopMatrix();
    }
}

void makeConfetti()
{
    if (ag.u >= MAX_PARTICLES)
        return;

    Particle *c = &ag.confetti[ag.u];

    c->s.center.x = rand() % 1921;
    c->s.center.y = 1080;

    c->velocity.y = -0.8;
    c->velocity.x = 0;
    ++ag.u;
}

void confettiMovement()
{
    if (ag.u <= 0)
        return;

    for (int i = 0; i < ag.u; i++) {
        Particle *c = &ag.confetti[i];
        c->s.center.x += c->velocity.x;
        c->s.center.y += c->velocity.y;
        c->velocity.y = c->velocity.y - GRAVITY;

        //check for off screen
        if (c->s.center.y < 0.0 || c->s.center.y > 1080 ||
            c->s.center.x < 0.0 || c->s.center.y > 1920) {
            ag.confetti[i] = ag.confetti[ag.u - 1];
            --ag.u;
        }
    }
}

void printConfetti()
{
    float w, h;

    for (int i = 0; i < ag.u; i++) {
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
    if (ag.q >= MAX_PARTICLES)
        return;

    Particle *b = &ag.bullet[ag.q];

    b->s.center.x = x;
    b->s.center.y = y;
    b->velocity.y = 0;
    b->velocity.x = ((double)rand() / (double)RAND_MAX) + 20;
    ++ag.q;
}

void bulletMovement()
{
    if (ag.q <= 0)
        return;
    for (int i = 0; i < ag.q; i++) {
        Particle *b = &ag.bullet[i];
        b->s.center.x += b->velocity.x;
        b->s.center.y += b->velocity.y;
        //b->s.velocity.y = b->velocity.y - GRAVITY;

        //check for off screen
        if (b->s.center.y < 0.0 || b->s.center.y > 1080 ||
            b->s.center.x < 0.0 || b->s.center.x > 1920) {
            ag.bullet[i] = ag.bullet[ag.q - 1];
            --ag.q;
        }
    }
}

void printBullet()
{
    float w, h;

    for (int i = 0; i < ag.q; i++) {
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
