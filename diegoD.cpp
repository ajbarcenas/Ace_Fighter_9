// Name: Diego Diaz
// Date: 9/17/2019
#include <stdio.h>
#include <GL/glx.h>

struct Vec {
        float x,y,z;
};

struct Shape {
        float width, height;
        float radius;
        Vec center;
        Vec velocity;
        bool playerExists = false;
};


void spawnEnemy(int i, Shape *e){
        e->width = 12;
        e->height = 12;
        e->center.x = ((i+1) * 50) + 1300;
        e->center.y = 800 - ((i+1) * 80);
}

void spawnPlayer(Shape *p)
{
        p->width = 15;
        p->height = 15;
        p->center.x = 200;
        p->center.y = 570;
}

void moveEnemy(Shape *e){
       	e->velocity.x = -0.5;
        e->center.x += e->velocity.x;
}


void ShowDiegosPicture(int x, int y, GLuint textid)
{
	
}
