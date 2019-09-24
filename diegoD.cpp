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

/*
void spawnEnemy(int i, Shape *e){
	//Shape *e = &g.enemy[i];
        e->width = 12;
        e->height = 12;
        e->center.x = ((i+1) * 20) + 200;
        e->center.y = 400 - ((i+1) * 50);
}
*/

void spawnPlayer(Shape *p)
{
	//Shape *p = &g.player;
        p->width = 15;
        p->height = 15;
        p->center.x = 200;
        p->center.y = 180;
}


void ShowDiegosPicture(int x, int y, GLuint textid)
{
	
}
