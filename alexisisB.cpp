// Name: Alexisis Barcenas
// Date: 9/17/2019
#include <stdio.h>
#include "fonts.h"

void printAlexisB(Rect r) 
{
	r.left = 40;
	r.bot = 50;
	r.center = 0;
	ggprint8b(&r,16, 0xfffff, "Alexisis Barcenas");    
}
