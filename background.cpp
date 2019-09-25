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

class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image() { delete [] data; }
    Image(const char *fname) {
        if (fname[0] == '\0')
            return;
        //printf("fname **%s**\n", fname);
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        name[slen-4] = '\0';
        //printf("name **%s**\n", name);
        char ppmname[80];
        sprintf(ppmname,"%s.ppm", name);
        //printf("ppmname **%s**\n", ppmname);
        char ts[100];
        //system("convert eball.jpg eball.ppm");
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
        //sprintf(ts, "%s", name);
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
        unlink(ppmname);
    }
};
Image img[1] = {"MountainLayer.png"};

class Texture {
public:
    Image *backImage;
    GLuint backTexture;
    float xc[2];
    float yc[2];
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

class Global {
public:
    int xres, yres;
    Shape player;
    Texture tex;
    Shape box;
    int n;
    GLuint texid;
    int showCredits;
    Shape enemy[5];
    Global() {
        xres=1920, yres=1080;
        showCredits = 0;
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
extern void showCreditScreen();
extern void showPicture(int x, int y, GLuint texid);
void show();
void printAlexisB(Rect r);
void showAlonsoText(Rect r);

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
        render();
        x11.swapBuffers();
    }
    return 0;
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
    g.tex.backImage = &img[0];
    //create opengl texture elements
    glGenTextures(1, &g.tex.backTexture);
    int w = g.tex.backImage->width;
    int h = g.tex.backImage->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, g.tex.backImage->data);
    //Change view area of image
    g.tex.xc[0] = 0.0;
    g.tex.xc[1] = 1.0;
    g.tex.yc[0] = 0.0;
    g.tex.yc[1] = 1.0;
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
            switch(key){
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
                    printf("test");
                    g.showCredits ^= 1;
                    break;
                case XK_Escape:
                    return 1;
            }
        //if (key == XK_Escape) {
        //	return 1;
        //}
    }
    return 0;
}
void show()
{
    g.box.width = 1920;
    g.box.height = 1080;
    g.box.center.x = 960;
    g.box.center.y = 540;
    float h, w;
        glColor3ub(255, 143, 143);
        glPushMatrix();
        glTranslatef(g.box.center.x, g.box.center.y, 0);
        w = g.box.width;
        h = g.box.height;
        glBegin(GL_QUADS);
            glVertex2i(-w, -h);
            glVertex2i(-w, h);
            glVertex2i(w, h);
            glVertex2i(w, -h);
        glEnd();
        glPopMatrix();
}

void physics()
{
    //move the background
    g.tex.xc[0] += 0.02;
    g.tex.xc[1] += 0.02;
//	Shape *p = &g.player;
//	p->velocity.x = 0.5;
//	p->center.x += p->velocity.x;
//	cout << p->center.x << endl;


}

void render()
{
    Rect r;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
    glEnd();

    //creating player
    Shape *p = &g.player;
    if(!p->playerExists){
        spawnPlayer(p);
        p->playerExists = true;
    }
    glColor3ub(190,140,10);
    glPushMatrix();
    float w = p->width;
    float h = p->height;
    glTranslatef(p->center.x, p->center.y, p->center.z);
    glBegin(GL_QUADS);
        glVertex2i(-w,-h);
        glVertex2i(-w, h);
        glVertex2i( w, h);
        glVertex2i( w,-h);
    glEnd();
    glPopMatrix();
    //creating enemies
    //Shape *e = &g.enemy[4];
    
    for (int i = 0; i < 5; i++) {
        g.enemy[i].width = 12;
        g.enemy[i].height = 12;
        g.enemy[i].center.x = ((i+1) * 20) + 200;
        g.enemy[i].center.y = 400 - ((i+1) * 50);
    }
    float we[5];
    float he[5];
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3ub(190,150,10);
        //spawnEnemy(g.n);
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
        //g.n++;
        cout << i << endl;
        cout << g.enemy[i].center.x << endl;
        cout << g.enemy[i].center.y << endl;
    }
    if (g.showCredits) {
        show();
        printAlexisB(r);
        showAlonsoText(r);
    }
    //unsigned int c = 0x00ffff44;
        r.bot = 500;
        r.left = 500;
        r.center = 0;
        ggprint8b(&r, 16, 0x00ffff44, "Press C to go to credits");
}

