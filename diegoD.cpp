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

void spawnPlayer(Shape *p)
{
        p->width = 25;
        p->height = 25;
        p->center.x = 200;
        p->center.y = 570;
}

void checkPlayerLocation(Shape *p)
{
	 if(p->center.x - p->width < -25 || p->center.x + p->width > 1945 ||
	    p->center.y - p->height < -25 || p->center.y + p->height > 1105) {
		spawnPlayer(p);
	 }
}

// =========================Enemy Functions =================================
/*
void removeEnemy(Shape *e, int *i)
{
        *e = *(e--);
        --i;
}

void checkEnemyLocation(Shape *e, int *i)
{
	if(e->center.x < 0.0){
	    removeEnemy(e, i);
	}
}

void spawnEnemy(int i, Shape *e)
{
        e->width = 18;
        e->height = 18;
        e->center.x = ((i+1) * 50) + 2000;
        e->center.y = 900 - ((i+1) * 90);
}

void moveEnemy(Shape *e)
{
       	e->velocity.x = -3.0;
        e->center.x += e->velocity.x;
}
*/

void ShowDiegosPicture(int x, int y, GLuint textid)
{
	
}
