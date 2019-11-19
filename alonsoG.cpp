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
3.Rain and Confetti paricles
4.3D cube powerup
*/

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <math.h>
#include "fonts.h"
#include <iostream>
#include <iomanip>
#include "alexisisB.h"

using namespace std;

extern ABarGlobal abG;
extern Enemy eLex;
extern void incrementScore(int points);

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
    int damage;
};

class AlonsoGlobal {
public:
    int xres, yres;
    Particle smoke[MAX_PARTICLES];
    Particle bullet[MAX_PARTICLES];
    Particle confetti[MAX_PARTICLES];
    Particle rain[MAX_PARTICLES];
    Particle missile[MAX_PARTICLES];
    int n = 0;
    int u = 0;
    int k = 0;
    int q = 0;
    int p = 0;
    int power = 1;
    int pointsX;
    int pointsY;
    float xr = 1.0;
    float yr = 1.0;
    float cx = 1.0;
    float cy = 0.0;
    float cz = 0.1;
    double winX;
    double winY;
    double winZ;
    bool printPoints = false;
    bool increasing = true;
}ag;

//=============================================================================
// Cube Functions
//=============================================================================
void cubePower(GLuint topTexture, GLuint sideTexture, GLuint bottomTexture)
{
    //Cube rotation
    ag.xr = ag.xr + 6;
    ag.yr = ag.yr + 6;

    //Cube goes up and down
    if (ag.increasing) {
        ag.cy = ag.cy + 0.005;
        if (ag.cy >= 1.0)
            ag.increasing = false;
    } else {
        ag.cy = ag.cy - 0.005;
        if (ag.cy <= -1.0)
            ag.increasing = true;
    }

    //Cube moves from right to left
    if (ag.cx > -1.0)
        ag.cx = ag.cx - 0.002;
    if (ag.cx <= -1.0)
        ag.cx = 1.0;

    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(ag.cx, ag.cy, ag.cz);
    
    //convert 3D coordinates to 2D coordinates
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble modelview[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    gluProject(ag.cx, ag.cy, ag.cz,
               modelview, projection, viewport,
               &ag.winX, &ag.winY, &ag.winZ);

    ag.winX = (ag.winX/2) + (956/2) + 2;
    ag.winY = (ag.winY/2) + (545/2);

    glRotatef(ag.xr, 1.0, 0.0, 0.0);
    glRotatef(ag.yr, 0.0, 1.0, 0.0);

    //Front
    glBindTexture(GL_TEXTURE_2D, sideTexture);
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, -0.05f, 0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f,  0.05f, 0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f,  0.05f, 0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.05f, -0.05f, 0.05f);
    glEnd();
    //Back
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, -0.05f, -0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f,  0.05f, -0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f,  0.05f, -0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.05f, -0.05f, -0.05f);
    glEnd();
    //Right
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.05f, -0.05f,  0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.05f,  0.05f,  0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.05f,  0.05f, -0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.05f, -0.05f, -0.05f);
    glEnd();
    //Left
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, -0.05f, -0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f,  0.05f, -0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.05f,  0.05f,  0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.05f, -0.05f,  0.05f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //Top
    glBindTexture(GL_TEXTURE_2D, topTexture);
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, 0.05f, -0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f, 0.05f,  0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f, 0.05f,  0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.05f, 0.05f, -0.05f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //Bottom
    glBindTexture(GL_TEXTURE_2D, bottomTexture);
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, -0.05f,  0.05f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f, -0.05f, -0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f, -0.05f, -0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.05f, -0.05f,  0.05f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
}

//=============================================================================
// Rain Functions
//=============================================================================
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
    
    for (int i = 0; i < ag.k; i++) {
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

//=============================================================================
// Confetti Functions
//=============================================================================
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

//=============================================================================
// Smoke Functions
//=============================================================================
void makeSmoke(int x, int y)
{
    if (ag.n >= MAX_PARTICLES)
        return;
    Particle *p = &ag.smoke[ag.n];

    p->s.center.x = x - (rand() % 25);
    
    if (rand() % 16 > 8)
        p->s.center.y = y + (rand() % 10);
    else
        p->s.center.y = y - (rand() % 10);

    p->velocity.y = 0;
    p->velocity.x = - ((double)rand() / (double)RAND_MAX) - 22;
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
        w = h = 5;
        glBegin(GL_QUADS);
            glVertex2i(c->x-w, c->y-h);
            glVertex2i(c->x-w, c->y+h);
            glVertex2i(c->x+w, c->y+h);
            glVertex2i(c->x+w, c->y-h);
        glEnd();
        glPopMatrix();
    }
}

//=============================================================================
// Bullet Functions
//=============================================================================
void makeBullet(int x, int y)
{
    if (ag.q >= MAX_PARTICLES)
        return;

    Particle *b = &ag.bullet[ag.q];
    b->damage = 10;
    b->s.center.x = x;
    b->s.center.y = y;
    b->velocity.y = 0;
    b->velocity.x = ((double)rand() / (double)RAND_MAX) + 20;
    ++ag.q;
}

void bulletMovement()
{
    Rect r;

    if (ag.q <= 0)
        return;

    for (int i = 0; i < ag.q; i++) {
        Particle *b = &ag.bullet[i];
        b->s.center.x += b->velocity.x;
        b->s.center.y += b->velocity.y;

        //check for off screen
        if (b->s.center.y < 0.0 || b->s.center.y > 1080 ||
            b->s.center.x < 0.0 || b->s.center.x > 1920) {
            ag.bullet[i] = ag.bullet[ag.q - 1];
            --ag.q;
        }
        
        //check collision of bullet with cube powerup
        if (b->s.center.y < ag.winY + 36 && b->s.center.y > ag.winY - 36 &&
            b->s.center.x < ag.winX + 36 && b->s.center.x > ag.winX - 36) {
            ag.cy = ((float)rand() / (float)RAND_MAX);
            ag.cx = ((float)rand() / (float)RAND_MAX);
            ag.bullet[i] = ag.bullet[ag.q - 1];
            ag.power++;
            --ag.q;
            abG.incrementScore(100);
        }

        //check collision of bullet with Alexis enemies
        for (int j = 0; j < eLex.getNumEnemy(); j++) {
            if (b->s.center.y < eLex.getEY(j) + 30 &&
                b->s.center.y > eLex.getEY(j) - 30 &&
                b->s.center.x < eLex.getEX(j) + 30 &&
                b->s.center.x > eLex.getEX(j) - 30) {
                ag.bullet[i] = ag.bullet[ag.q - 1];
                --ag.q;
                ag.pointsY = eLex.getEY(j) - 30;
                ag.pointsX = eLex.getEX(j) - 30;
                ag.printPoints = true;
                eLex.deleteEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
        //check collision of bullet with Alexis Venemies
        for (int j = 0; j < eLex.getVNumEnemy(); j++) {
            if (b->s.center.y < eLex.getVY(j) + 30 &&
                b->s.center.y > eLex.getVY(j) - 30 &&
                b->s.center.x < eLex.getVX(j) + 30 &&
                b->s.center.x > eLex.getVX(j) - 30) {
                ag.bullet[i] = ag.bullet[ag.q - 1];
                --ag.q;
                ag.pointsY = eLex.getVY(j) - 30;
                ag.pointsX = eLex.getVX(j) - 30;
                ag.printPoints = true;
                eLex.deleteVEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
        //check collision of bullet with Alexis Cenemies
        for (int j = 0; j < eLex.getCNumEnemy(); j++) {
            if (b->s.center.y < eLex.getCY(j) + 30 &&
                b->s.center.y > eLex.getCY(j) - 30 &&
                b->s.center.x < eLex.getCX(j) + 30 &&
                b->s.center.x > eLex.getCX(j) - 30) {
                ag.bullet[i] = ag.bullet[ag.q - 1];
                --ag.q;
                ag.pointsY = eLex.getCY(j) - 30;
                ag.pointsX = eLex.getCX(j) - 30;
                ag.printPoints = true;
                eLex.deleteCEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
    }
}

void printBullet(float w, float h, GLuint Texture)
{
    for (int i = 0; i < ag.q; i++) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor3ub(255,255,255);
        Vec *c = &ag.bullet[i].s.center;
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(c->x-w*.05, c->y-h*.05);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(c->x-w*.05, c->y+h*.05);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(c->x+w*.05, c->y+h*.05);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(c->x+w*.05, c->y-h*.05);
        glEnd();
        glPopMatrix();
    }
}

//=============================================================================
// Missile Functions
//=============================================================================
void makeMissile(int x, int y)
{
    if (ag.p >= MAX_PARTICLES)
        return;

    Particle *m = &ag.missile[ag.p];
    m->damage = 50;
    m->s.center.x = x;
    m->s.center.y = y;
    m->velocity.y = 0;
    m->velocity.x = ((double)rand() / (double)RAND_MAX) + 20;
    ++ag.p;
}

void missileMovement()
{
    Rect r;

    if (ag.p <= 0)
        return;

    for (int i = 0; i < ag.p; i++) {
        Particle *m = &ag.missile[i];
        m->s.center.x += m->velocity.x;
        m->s.center.y += m->velocity.y;

        //check for off screen
        if (m->s.center.y < 0.0 || m->s.center.y > 1080 ||
            m->s.center.x < 0.0 || m->s.center.x > 1920) {
            ag.missile[i] = ag.missile[ag.p - 1];
            --ag.p;
        }
        
        //check collision of missile with cube powerup
        if (m->s.center.y < ag.winY + 36 && m->s.center.y > ag.winY - 36 &&
            m->s.center.x < ag.winX + 36 && m->s.center.x > ag.winX - 36) {
            ag.cy = ((float)rand() / (float)RAND_MAX);
            ag.cx = ((float)rand() / (float)RAND_MAX);
            ag.missile[i] = ag.missile[ag.p - 1];
            ag.power++;
            --ag.p;
            abG.incrementScore(100);
        }

        //check collision of missile with Alexis enemies
        for (int j = 0; j < eLex.getNumEnemy(); j++) {
            if (m->s.center.y < eLex.getEY(j) + 30 &&
                m->s.center.y > eLex.getEY(j) - 30 &&
                m->s.center.x < eLex.getEX(j) + 30 &&
                m->s.center.x > eLex.getEX(j) - 30) {
                ag.missile[i] = ag.missile[ag.p - 1];
                --ag.p;
                ag.pointsY = eLex.getEY(j) - 30;
                ag.pointsX = eLex.getEX(j) - 30;
                ag.printPoints = true;
                eLex.deleteEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
        //check collision of missile with Alexis Venemies
        for (int j = 0; j < eLex.getVNumEnemy(); j++) {
            if (m->s.center.y < eLex.getVY(j) + 30 &&
                m->s.center.y > eLex.getVY(j) - 30 &&
                m->s.center.x < eLex.getVX(j) + 30 &&
                m->s.center.x > eLex.getVX(j) - 30) {
                ag.missile[i] = ag.missile[ag.p - 1];
                --ag.p;
                ag.pointsY = eLex.getVY(j) - 30;
                ag.pointsX = eLex.getVX(j) - 30;
                ag.printPoints = true;
                eLex.deleteVEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
        //check collision of missile with Alexis Cenemies
        for (int j = 0; j < eLex.getCNumEnemy(); j++) {
            if (m->s.center.y < eLex.getCY(j) + 30 &&
                m->s.center.y > eLex.getCY(j) - 30 &&
                m->s.center.x < eLex.getCX(j) + 30 &&
                m->s.center.x > eLex.getCX(j) - 30) {
                ag.missile[i] = ag.missile[ag.p - 1];
                --ag.p;
                ag.pointsY = eLex.getCY(j) - 30;
                ag.pointsX = eLex.getCX(j) - 30;
                ag.printPoints = true;
                eLex.deleteCEnemy(j);
                ggprint16(&r, 16, 0x00ffff44, "+1000");
                abG.incrementScore(1000);
            }
        }
    }
}

void printMissile(float w, float h, GLuint Texture)
{
    for (int i = 0; i < ag.p; i++) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor3ub(255,255,255);
        Vec *c = &ag.missile[i].s.center;
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(c->x-w*.2, c->y-h*.2);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(c->x-w*.2, c->y+h*.2);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(c->x+w*.2, c->y+h*.2);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(c->x+w*.2, c->y-h*.2);
        glEnd();
        glPopMatrix();
    }
}

//=============================================================================
// Get Functions
//=============================================================================
bool getPrintPoints()
{
    return ag.printPoints;
}

int getPointsX()
{
    return ag.pointsX;
}

int getPointsY()
{
    return ag.pointsY;
}

int getPower()
{
    return ag.power;
}

//=============================================================================
//This is my friday code
int getBulletDamage()
{
    Particle *b = &ag.bullet[ag.q];
    return b->damage;
}

int getMissileDamage()
{
    Particle *m = &ag.missile[ag.q];
    return m->damage;
}

void deleteBullet(int i)
{
    ag.bullet[i] = ag.bullet[ag.q - 1];
    --ag.q;
}

void deleteMissile(int i)
{
    ag.missile[i] = ag.missile[ag.p - 1];
    --ag.p;
}
//Weapons have damage and delete after collision
//=============================================================================

void getTotalBullets(int &tot)
{
	tot = ag.q;
}

void getBulletXY(int &bulletX, int &bulletY, int i)
{
	Particle *c = &ag.bullet[i];
	bulletX = c->s.center.x;
	bulletY = c->s.center.y;
}

void getTotalMissiles(int &tot)
{
    tot = ag.p;
}

void getMissileXY(int &missileX, int &missileY, int i)
{
    Particle *c = &ag.missile[i];
    missileX = c->s.center.x;
    missileY = c->s.center.y;
}

//=============================================================================
// Miscellaneous
//=============================================================================
void makeCubeCoordsNull()
{
    ag.winX = ag.winY = ag.winZ = 9999;
}

double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double)(end->tv_sec - start->tv_sec) +
           (double)(end->tv_nsec - start->tv_nsec) * (1.0/1e9);
}


