//
//program: background.cpp
//author:  Gordon Griesel
//date:    2017 - 2018
//
//The position of the background QUAD does not change.
//Just the texture coordinates change.
//In this example, only the x coordinates change.
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "alexisisB.h"

typedef double Vect[3];

class Image {
	public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;
			data = new unsigned char[n];
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
		printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if(!ppmFlag)
			unlink(ppmname);
	}
};
Image img[8] = { "./Images/MountainLayer.png",
    "./Images/CloudLayer.png",
    "./Images/AceFighter9.png",
"./Images/Alexis.jpg",
"./Images/freeRealEstate.jpg",
"./Images/DiegoPic.jpg",
"./Images/andrew.jpg",
"./Images/PineTreeLayer.png"};

class Texture {
	public:
	Image *backImage;
	float xc[6];
	float yc[6];
};
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

struct Particle {
	Shape s;
	Vec Velocity;
};

class Logo {
public:
	Vect pos;
	Vect vel;
} logo;

class Pictures {
public: 
	Vect pos;
	Vect vel;
} picture;

class Picturetwo {
public:
	Vect pos;
	Vect vel;
} picture2;

class Picturethree {
public:
	Vect pos;
	Vect vel;
} picture3;

class Picturefour {
public:
	Vect pos;
	Vect vel;
} picture4;

class Global {
	public:
	int xres, yres;
	GLuint mountainTexture;
	GLuint cloudTexture;
    GLuint cSilhouetteTexture;
    GLuint pineTreeTexture;
	GLuint pSilhouetteTexture;
	Shape player;
	Texture tex;
	Shape box;
	GLuint logoTexture;
	GLuint alexisTexId;
	GLuint alonsoTexId;
	GLuint diegoTexId;
	GLuint andrewTexId;
	int showLogo;
	int n = 0;
	GLuint texid;
	int showCredits;
	Shape enemy[5];
	Global() {
		//Pictures pic;
		xres=1920, yres=1080;
		picture.pos[0] = 480, picture.pos[1] = 800;
		picture2.pos[0] = 1440, picture2.pos[1] = 800;
		picture3.pos[0] = 480, picture3.pos[1] = 300;
		picture4.pos[0] = 1440, picture4.pos[1] = 300;
		showCredits = 0;
		showLogo = 0;
		logo.pos[0] = 960;
		logo.pos[1] = 540;
	}
} g;


class X11_wrapper {
	private:
	Display *dpy;
	Window win;
	GLXContext glc;
	public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		setup_screen_res(1920, 1080);
		dpy = XOpenDisplay(NULL);
		if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);

		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
		}
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = cmap;
		swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
		ButtonPressMask | ButtonReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	void cleanupXWindows() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "scrolling background (seamless)");
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
		return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
		}
	}
} x11;

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);
extern void spawnPlayer(Shape *p);
extern void spawnEnemy(int i, Shape *e);
extern void checkPlayerLocation(Shape *p);
extern void moveEnemy(Shape *e);
extern void checkEnemyLocation(Shape *e, int *i);
extern void removeEnemy(Shape *e, int *i);
extern void showCreditScreen();
extern void showPicture(int x, int y, GLuint texid);
void showAlonsoText(Rect r);
extern ABarGlobal abG;
extern void makeSmoke(int x, int y);
extern void printSmoke();
extern void smokeMovement();
extern void makeBullet(int x, int y);
extern void printBullet();
extern void bulletMovement();
//===========================================================================
//===========================================================================
int main()
{
	init_opengl();
	int done=0;
	g.n = 0;
	cout << g.n << endl;
	while (!done) {
	while (x11.getXPending()) {
		XEvent e = x11.getXNextEvent();
		x11.check_resize(&e);
		check_mouse(&e);
		done = check_keys(&e);
	}
	physics();
    smokeMovement();
	bulletMovement();
    render();
	x11.swapBuffers();
	}
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to the RGB stream
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i = 0; i < img->width * img->height * 3; i += 3) {
		a = *(data + 0);
		b = *(data + 1);
		c = *(data + 2);
		*(ptr + 0) = a;
		*(ptr + 1) = b;
		*(ptr + 2) = c;

		*(ptr + 3) = (a|b|c);

		ptr += 4;
		data += 3;
	}
	return newdata;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow texture maps
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	//g.tex.backImage = &img[0];
	//create opengl texture elements
	glGenTextures(1, &g.mountainTexture);
	glGenTextures(1, &g.cloudTexture);
    glGenTextures(1, &g.cSilhouetteTexture);
    glGenTextures(1, &g.pineTreeTexture);
	glGenTextures(1, &g.pSilhouetteTexture);
	glGenTextures(1, &g.logoTexture);
	glGenTextures(1, &g.alexisTexId);
	glGenTextures(1, &g.alonsoTexId);
	glGenTextures(1, &g.diegoTexId);
	glGenTextures(1, &g.andrewTexId);
	//-------------------------------------------------------------------------
	//MountainLayer
	//
	int w = img[0].width;
	int h = img[0].height;

	glBindTexture(GL_TEXTURE_2D, g.mountainTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	//-------------------------------------------------------------------------
	//CloudLayer
	//
	int wc = img[1].width;
	int hc = img[1].height;

	glBindTexture(GL_TEXTURE_2D, g.cloudTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wc, hc, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

	//Silhouette

	glBindTexture(GL_TEXTURE_2D, g.cSilhouetteTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *cSilhouetteData = buildAlphaData(&img[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wc, hc, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, cSilhouetteData);
	free(cSilhouetteData);
    //--------------------------------------------------------------------------
    //PineTreeLayer
    //
    int wp = img[7].width;
    int hp = img[7].height;

    glBindTexture(GL_TEXTURE_2D, g.pineTreeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, wp, hp, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[7].data);

    //Silhouette

    glBindTexture(GL_TEXTURE_2D, g.pSilhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *pSilhouetteData = buildAlphaData(&img[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wp, hp, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, pSilhouetteData);
    free(pSilhouetteData);

	//Change view area of image
	//mountain imgae
    g.tex.xc[0] = 0.0;
	g.tex.xc[1] = 1.0;
	g.tex.yc[0] = 0.0;
	g.tex.yc[1] = 1.0;
	//cloud image
	g.tex.xc[2] = 0.0;
	g.tex.xc[3] = 1.0;
	g.tex.yc[2] = 0.0;
	g.tex.yc[3] = 1.0;
    //pine tree image
    g.tex.xc[4] = 0.0;
    g.tex.xc[5] = 1.0;
    g.tex.yc[4] = 0.0;
    g.tex.yc[5] = 1.0;

	// Logo Picture
	w = img[2].width;
	h = img[2].height;
	glBindTexture(GL_TEXTURE_2D, g.logoTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

	// Alexis picture
	w = img[3].width;
	h = img[3].height;
	glBindTexture(GL_TEXTURE_2D, g.alexisTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

	// Alonso picture
	w = img[4].width;
	h = img[4].height;
	glBindTexture(GL_TEXTURE_2D, g.alonsoTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

	// Diego picture
	w = img[5].width;
	h = img[5].height;
	glBindTexture(GL_TEXTURE_2D, g.diegoTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
	
	// Diego picture
	w = img[6].width;
	h = img[6].height;
	glBindTexture(GL_TEXTURE_2D, g.andrewTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
	cout << e->xbutton.button << endl;
	if (e->xbutton.button== 1) {
	}
	if (e->xbutton.button== 3) {
	}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	Shape *p = &g.player;
	if (e->type == KeyPress) {
	int key = XLookupKeysym(&e->xkey, 0);
	switch(key) {
		case XK_Left:
			p->velocity.x = -15;
			p->center.x += p->velocity.x;
			break;
		case XK_Right:
			p->velocity.x = 15;
			p->center.x += p->velocity.x;
			break;
		case XK_Up:
			p->velocity.y = 15;
			p->center.y += p->velocity.y;
			break;
		case XK_Down:
			p->velocity.y = -15;
			p->center.y += p->velocity.y;
			break;
		case XK_c:
			g.showCredits ^= 1;
			g.showLogo ^= 1;
			break;
        case XK_space:
            makeBullet(p->center.x, p->center.y);
            break;
		case XK_Escape:
			return 1;
	}
	if (key == XK_Escape) {
		return 1;
	}
}
	return 0;
}

void physics()
{
	//move the background
    //mountain layer
	g.tex.xc[0] += 0.001;
	g.tex.xc[1] += 0.001;
    //cloud layer
	g.tex.xc[2] += 0.005;
	g.tex.xc[3] += 0.005;
    //pine tree layer
   	 g.tex.xc[4] += 0.008;
    	g.tex.xc[5] += 0.008;
    	int *n = &g.n;
	for(int i = 0; i < 5; i++) {
		checkEnemyLocation(&g.enemy[i], n);	
		moveEnemy(&g.enemy[i]);
	}
}

void render()
{
	extern ABarGlobal abG;
	Rect r;
	//All of these scrolling background layers were done by Alonso Gomez
	// 
	//Background Layers
	//-------------------------------------------------------------------------
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
	// Done by Alonso Gomez

	glBindTexture(GL_TEXTURE_2D, g.cSilhouetteTexture);
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
	
    //Pine Tree Layer
    //Done by Alonso Gomez

    glBindTexture(GL_TEXTURE_2D, g.pSilhouetteTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc[4], g.tex.yc[5]); glVertex2i(0, 0);
        glTexCoord2f(g.tex.xc[4], g.tex.yc[4]); glVertex2i(0, g.yres);
        glTexCoord2f(g.tex.xc[5], g.tex.yc[4]); glVertex2i(g.xres, g.yres);
        glTexCoord2f(g.tex.xc[5], g.tex.yc[5]); glVertex2i(g.xres, 0);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    //-------------------------------------------------------------------------

	//creating player
	Shape *p = &g.player;
	if(!p->playerExists) {
		spawnPlayer(p);
		p->playerExists = true;
	}

	checkPlayerLocation(p);
	glColor3ub(190,140,10);


	glPushMatrix();
	float w = p->width;
	float h = p->height;
	makeSmoke(p->center.x, p->center.y);
    makeSmoke(p->center.x, p->center.y);
    printSmoke();
    glColor3ub(190, 140, 10);
    glTranslatef(p->center.x, p->center.y, p->center.z);
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
  printBullet();

	//creating enemies

	//creating enemies
	while( g.n < 5) {
		spawnEnemy(g.n, &g.enemy[g.n]);
		g.n++;
	}
	float we[5];
	float he[5];
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glColor3ub(190,150,10);
		we[i] = g.enemy[i].width;
		he[i] = g.enemy[i].height;
		glTranslatef(g.enemy[i].center.x, 
		g.enemy[i].center.y, g.enemy[i].center.z);
		glBegin(GL_QUADS);
			glVertex2i(-we[i],-he[i]);
			glVertex2i(-we[i], he[i]);
			glVertex2i( we[i], he[i]);
			glVertex2i( we[i],-he[i]);
		glEnd();
		glPopMatrix();
	}
    
    //--------------------------------------------------------------------------
    //Smoke Particles
    //

    //printSmoke();

    //--------------------------------------------------------------------------
    //Credits Screen
    //

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	if (g.showCredits) {
		showCreditsBorder(1920, 1080, 960, 540);
		abG.printCredBoxes(960, 540);
		showCreditsBorder(180, 180, 960, 540, 14, 14, 138 );
		showCreditsBorder(170, 170, 960, 540, 21, 21, 237);
		showCreditsBorder(160, 160, 960, 540, 47, 47, 237);
		showCreditsBorder(150, 150, 960, 540, 69, 80, 237);
		showCreditsBorder(140, 140, 960, 540, 39, 123, 232);
		showCreditsBorder(130, 130, 960, 540);
		showCreditsBorder(130, 130, 480, 800);
		showCreditsBorder(130, 130, 1440, 800);
		showCreditsBorder(130, 130, 480, 300);
		showCreditsBorder(130, 130, 1440, 300);
		abG.printPicture(logo.pos[0], logo.pos[1], logo.pos[2], g.logoTexture);
		abG.printPicture(picture.pos[0], picture.pos[1], 0, g.alexisTexId);
		abG.printPicture(picture2.pos[0], picture2.pos[1], 0, g.alonsoTexId);
		abG.printPicture(picture3.pos[0], picture3.pos[1], 0, g.diegoTexId);
		abG.printPicture(picture4.pos[0], picture4.pos[1], 0, g.andrewTexId);
		printAlexisB(r);
		glColor3f(1.0, 1.0, 1.0);
		showAlonsoText(r);
		glColor3f(1.0, 1.0, 1.0);
		//abG.printAceFighter9(r);
		r.bot = 145, r.left = 520;
		ggprint16(&r, 16, 0xcf13ac, "Diego Diaz- Player and Enemy Movement");
		r.bot = 145, r.left = 1500;
		ggprint16(&r, 16, 0xcf13ac,
			"Andrew Oliveros- HUD Creation/Sprites/Menu");
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	//unsigned int c = 0x00ffff44;
	r.bot = 100;
	r.left = 40;
	r.center = 0;
	ggprint16(&r, 16, 0x00ffff44, "Press C to go to credits");
}

