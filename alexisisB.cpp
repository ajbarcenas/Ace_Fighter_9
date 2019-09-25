// Name: Alexisis Barcenas
// Date: 9/17/2019
// Game: Ace Fighter 9
// Goals: Have hitboxes work correctly for player and enemy models
//
//
#include <stdio.h>
#include "fonts.h"

void printAlexisB(Rect r) 
{
	r.left = 40;
	r.bot = 50;
	r.center = 0;
	ggprint16(&r, 16, 0xfffff, "Alexisis Barcenas - Hitbox Specialist");    
}
