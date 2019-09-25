// Name: Alexisis Barcenas
// Date: 9/17/2019
// Game: Ace Fighter 9
// Goals: Have hitboxes work correctly for player and enemy models
//
//
#include <stdio.h>
#include "fonts.h"
#include <string.h>
#include <iostream>
using namespace std;
void printAlexisB(Rect r) 
{
	r.left = 40;
	r.bot = 50;
	r.center = 0;
	ggprint16(&r, 16, 0xfffff, "Alexisis Barcenas - Hitbox Specialist");    
}

void printAceFighter9(Rect r)
{
	string aceLetters[11] = {"A", "C", "E", "F","I","G", "H", "T", "E",
	    "R", "9"};
	r.left = 960;
	r.bot = 1050;
	for (int i = 0; i < 11; ++i) {
		string tmp = aceLetters[i];
		ggprint16(&r, 100, 0xfffff, tmp.c_str());
	}
	int x_scale = 100;
	r.left = 485;
	r.bot = 550;
	for (int i = 0; i < 11; ++i) {
		string tmp = aceLetters[i];
		
		if (i <= 4){
			r.left = 485  + i * x_scale;
			ggprint16(&r, 0, 0xfffff, tmp.c_str());
		}
		else if (i == 5) {
		    r.center = 0;
		}
		else {
		    r.left = 530 + (i-1) * x_scale;
		    ggprint16(&r, 0, 0xfffff, tmp.c_str());
		}
	}
}
