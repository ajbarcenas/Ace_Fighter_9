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
#include <cstring>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "fonts.h"
#include "alexisisB.h"
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctime>

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
        if (ppmFlag)
            strcpy(ppmname, name);
        else {
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

Image img[20] = {"./Images/MountainLayer.png",
                 "./Images/CloudLayer.png",
                 "./Images/AceFighter9.png",
                 "./Images/Alexis.jpg",
                 "./Images/freeRealEstate.jpg",
                 "./Images/DiegoPic.jpg",
                 "./Images/GameOver.png",
                 "./Images/PineTreeLayer.png",
                 "./Images/BulletGray.png",
                 "./Images/MissileRed.png",
                 "./Images/PlayerJet.png",
                 "./Images/EnemyJet1.png",
                 "./Images/EnemyJet2.png",
                 "./Images/EnemyJet3.png",
                 "./Images/MinecraftCubeTop.png",
                 "./Images/MinecraftCubeSide.png",
                 "./Images/MinecraftCubeBottom.png",
                 "./Images/yugiohFront.png",
                 "./Images/yugiohBack.png",
                 "./Images/yugiohBottom.png"};

class Texture {
public:
    Image *backImage;
    float xc[12];
    float yc[12];
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
    int damage;
};

struct Enemy1 {
    int maxHealth;
    int currentHealth;
    int damage;
    Shape s;
    bool removeEnemy = false;
};

struct Player {
    int currentHealth;
    int maxHealth; 
    int damage;
    Shape s;
};

struct Node {
    Enemy1 data;
    struct Node* next;
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
    GLuint pineTexture;
    GLuint bulletTexture;
    GLuint missileTexture;
    GLuint playerTexture;
    GLuint enemy1Texture;
    GLuint enemy2Texture;
    GLuint enemy3Texture;
    GLuint minecraftTop;
    GLuint minecraftSide;
    GLuint minecraftBottom;
    GLuint yugiohFront;
    GLuint yugiohBack;
    GLuint yugiohBottom;
    Player player;
    Texture tex;
    Shape box;
    GLuint logoTexture;
    GLuint alexisTexId;
    GLuint alonsoTexId;
    GLuint diegoTexId;
    GLuint andrewTexId;
    int showLogo;
    char keys[65536];
    int n = 0;
    int wave = 1;
    int maxEnemy1 = 5;
    bool mouseMovement = true;
    bool enemies1Dead = true;
    Enemy1 enemy;
    struct Node* head = NULL;
    GLuint texid;
    int showCredits, showHighScores;
    int HighScore;
    bool getCTime = true;
    bool getPTime = true;
    int getBTime = 1;
    bool isPaused = true;
    struct timespec cTimeStart;
    struct timespec cTimeCurr;
    struct timespec bTimeStart;
    struct timespec bTimeCurr;
    struct timespec pTimeStart;
    struct timespec pTimeCurr;
    double cTime;
    double bTime;
    double pTime;
    double billion = 1.0/1e9;
    Global() {
        //Pictures pic;
        xres=1920, yres=1080;
        picture.pos[0] = 480,   picture.pos[1] = 800;
        picture2.pos[0] = 1440, picture2.pos[1] = 800;
        picture3.pos[0] = 960,  picture3.pos[1] = 200;
        picture4.pos[0] = 960, picture4.pos[1] = 540;
        showCredits = 0;
        showLogo = 0;
        showHighScores = 0;
        logo.pos[0] = 960;
        logo.pos[1] = 540;
        HighScore = 0;
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
        XStoreName(dpy, win, "ACE FIGHTER 9 - BY GROUP #9");
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
extern void spawnPlayer(Player *p);
extern void printPlayer(Player *p, float w, float h, GLuint Texture);
extern void checkPlayerLocation(Player *p);
extern void setPlayerHealth(Player *p, int maxHealth);
extern void subtractPlayerHealth(int &currentHealth, int damage);
extern void movePlayerUp(Player *p);
extern void movePlayerDown(Player *p);
extern void movePlayerLeft(Player *p);
extern void movePlayerRight(Player *p);
extern void spawnEnemy(struct Node** head_ref, Enemy1 enemy);
extern void setEnemySize(struct Node* head_ref, int i);
extern void setEnemyHealth(struct Node* head_ref, int maxHealth);
extern void printEnemy(struct Node* temp, int n, float w, float h, GLuint Texture);
extern void moveEnemy(struct Node* enemy);
extern void checkEnemyLocation(struct Node* enemy, int &currentHealth);
extern void removeEnemy(struct Node** head, struct Node* enemy, int &n, bool &enemies1Dead, int &wave);
extern void checkEnemyCollision(struct Node* enemy);
extern void subtractEnemyHealth(struct Node* enemy, int damage);
extern void showCreditScreen();
extern void showPicture(int x, int y, GLuint texid);
void showAlonsoText(Rect r);
extern ABarGlobal abG;
extern Enemy eLex;
int rainDrops = 0;
int cube = 0;
extern int getPower();
extern void makeSmoke(int x, int y);
extern void printSmoke();
extern void smokeMovement();
extern void makeBullet(int x, int y);
extern void printBullet(float w, float h, GLuint Texture);
extern void bulletMovement(int &currentHealth);
extern void getBulletXY(int &bulletX, int &bulletY, int i);
extern void getTotalBullets(int &tot);
extern void makeMissile(int x, int y);
extern void printMissile(float w, float h, GLuint Texture);
extern void missileMovement(int &currentHealth);
extern void makeConfetti();
extern void printConfetti();
extern void confettiMovement();
extern void makeRain();
extern void printRain();
extern void rainMovement();
extern void cubePower(GLuint topTexture, GLuint sideTexture, GLuint bottomTexture);
extern void makeCubeCoordsNull();
extern bool getCubeCollision(int i);
extern bool getPyramidCollision(int i);
extern void makePyramidCoordsNull();
extern void pyramidPower(GLuint frontTexture, GLuint backTexture, GLuint bottomTexture);
extern int getPointsX();
extern int getPointsY();
extern bool getPrintPoints();
extern double timeDiff(struct timespec *, struct timespec *);
extern int authScores();
//===========================================================================
//===========================================================================
int main()
{
    init_opengl();
    int done=0;
    g.n = 0;
    g.enemies1Dead = true;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            //if(!g.isPaused)
		check_mouse(&e);
            done = check_keys(&e);
        }
    if(!g.isPaused)
	    physics();
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
    glClear(GL_DEPTH_BUFFER_BIT);
    //Do this to allow texture maps
    glEnable(GL_TEXTURE_2D);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    initialize_fonts();
	
    //=========================================================================
    // Create opengl texture elements
    //=========================================================================
    glGenTextures(1, &g.mountainTexture);
    glGenTextures(1, &g.cloudTexture);
    glGenTextures(1, &g.pineTexture);
    glGenTextures(1, &g.bulletTexture);
    glGenTextures(1, &g.missileTexture);
    glGenTextures(1, &g.playerTexture);
    glGenTextures(1, &g.enemy1Texture);
    glGenTextures(1, &g.enemy2Texture);
    glGenTextures(1, &g.enemy3Texture);
    glGenTextures(1, &g.minecraftTop);
    glGenTextures(1, &g.minecraftSide);
    glGenTextures(1, &g.minecraftBottom);
    glGenTextures(1, &g.yugiohFront);
    glGenTextures(1, &g.yugiohBack);
    glGenTextures(1, &g.yugiohBottom);
    glGenTextures(1, &g.logoTexture);
    glGenTextures(1, &g.alexisTexId);
    glGenTextures(1, &g.alonsoTexId);
    glGenTextures(1, &g.diegoTexId);
    glGenTextures(1, &g.andrewTexId);

    //=========================================================================
    // Mountain Layer
    //=========================================================================
    int w = img[0].width;
    int h = img[0].height;

    glBindTexture(GL_TEXTURE_2D, g.mountainTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

    //=========================================================================
    // Cloud Layer
    //=========================================================================
    w = img[1].width;
    h = img[1].height;
    
    glBindTexture(GL_TEXTURE_2D, g.cloudTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    unsigned char *cloudData = buildAlphaData(&img[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, cloudData);
    free(cloudData);

    //=========================================================================
    // Pine Tree Layer
    //=========================================================================
    w = img[7].width;
    h = img[7].height;

    glBindTexture(GL_TEXTURE_2D, g.pineTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	unsigned char *pineData = buildAlphaData(&img[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, pineData);
	free(pineData);

    //=========================================================================
    // Bullet Layer
    //=========================================================================
    w = img[8].width;
    h = img[8].height;

    glBindTexture(GL_TEXTURE_2D, g.bulletTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *bulletData = buildAlphaData(&img[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, bulletData);
    free(bulletData);

    //=========================================================================
    // Missile Layer
    //=========================================================================
    w = img[9].width;
    h = img[9].height;

    glBindTexture(GL_TEXTURE_2D, g.missileTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *missileData = buildAlphaData(&img[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, missileData);
    free(missileData);

    //=========================================================================
    // Player Jet
    //=========================================================================
    w = img[10].width;
    h = img[10].height;

    glBindTexture(GL_TEXTURE_2D, g.playerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *playerData = buildAlphaData(&img[10]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, playerData);
    free(playerData);

    //=========================================================================
    // Enemy1 Jet
    //=========================================================================
    w = img[11].width;
    h = img[11].height;
    
    glBindTexture(GL_TEXTURE_2D, g.enemy1Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *enemy1Data = buildAlphaData(&img[11]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, enemy1Data);
    free(enemy1Data);

    //=========================================================================
    // Enemy2 Jet
    //=========================================================================
    w = img[12].width;
    h = img[12].height;

    glBindTexture(GL_TEXTURE_2D, g.enemy2Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *enemy2Data = buildAlphaData(&img[12]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, enemy2Data);
    free(enemy2Data);
    
    //=========================================================================
    // Enemy 3 Jet
    //=========================================================================
    w = img[13].width;
    h = img[13].height;

    glBindTexture(GL_TEXTURE_2D, g.enemy3Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *enemy3Data = buildAlphaData(&img[13]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, enemy3Data);
    free(enemy3Data);

    //=========================================================================
    // Minecraft Top
    //=========================================================================
    w = img[14].width;
    h = img[14].height;

    glBindTexture(GL_TEXTURE_2D, g.minecraftTop);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *minecraftTopData = buildAlphaData(&img[14]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, minecraftTopData);
    free(minecraftTopData);

    //=========================================================================
    // Minecraft Side
    //=========================================================================
    w = img[15].width;
    h = img[15].height;

    glBindTexture(GL_TEXTURE_2D, g.minecraftSide);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *minecraftSideData = buildAlphaData(&img[15]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, minecraftSideData);
    free(minecraftSideData);

    //=========================================================================
    // Minecraft Bottom
    //=========================================================================
    w = img[16].width;
    h = img[16].height;

    glBindTexture(GL_TEXTURE_2D, g.minecraftBottom);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *minecraftBottomData = buildAlphaData(&img[16]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, minecraftBottomData);
    free(minecraftBottomData);

    //=========================================================================
    // Yugioh Front
    //=========================================================================
    w = img[17].width;
    h = img[17].height;

    glBindTexture(GL_TEXTURE_2D, g.yugiohFront);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *yugiohFrontData = buildAlphaData(&img[17]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, yugiohFrontData);
    free(yugiohFrontData);

    //=========================================================================
    // Yugioh Back
    //=========================================================================
    w = img[18].width;
    h = img[18].height;

    glBindTexture(GL_TEXTURE_2D, g.yugiohBack);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *yugiohBackData = buildAlphaData(&img[18]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, yugiohBackData);
    free(yugiohBackData);

    //=========================================================================
    // Yugioh Bottom
    //=========================================================================
    w = img[19].width;
    h = img[19].height;

    glBindTexture(GL_TEXTURE_2D, g.yugiohBottom);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *yugiohBottomData = buildAlphaData(&img[19]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, yugiohBottomData);
    free(yugiohBottomData);

    //=========================================================================
    // Change view area of image
    //=========================================================================

    //=========================================================================
    // Mountain Image
    //=========================================================================
    g.tex.xc[0] = 0.0;
    g.tex.xc[1] = 1.0;
    g.tex.yc[0] = 0.0;
    g.tex.yc[1] = 1.0;
    
    //=========================================================================
    // Cloud Image
    //=========================================================================
    g.tex.xc[2] = 0.0;
    g.tex.xc[3] = 1.0;
    g.tex.yc[2] = 0.0;
    g.tex.yc[3] = 1.0;

    //=========================================================================
    // Pine Tree Image
    //=========================================================================
    g.tex.xc[4] = 0.0;
    g.tex.xc[5] = 1.0;
    g.tex.yc[4] = 0.0;
    g.tex.yc[5] = 1.0;

    //=========================================================================
    // Logo Picture
    //=========================================================================
    w = img[2].width;
    h = img[2].height;
    glBindTexture(GL_TEXTURE_2D, g.logoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

    //=========================================================================
    // Alexis picture
    //=========================================================================
    w = img[3].width;
    h = img[3].height;
    glBindTexture(GL_TEXTURE_2D, g.alexisTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

    //=========================================================================
    // Alonso picture
    //=========================================================================
    w = img[4].width;
    h = img[4].height;
    glBindTexture(GL_TEXTURE_2D, g.alonsoTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

    //=========================================================================
    // Diego picture
    //=========================================================================
    w = img[5].width;
    h = img[5].height;
    glBindTexture(GL_TEXTURE_2D, g.diegoTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img[5].data);

    //=========================================================================
    // Andrew picture
    //=========================================================================
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
    Player *p = &g.player;
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
	
    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
        e->type != ButtonPress &&
        e->type != MotionNotify) {
        //This is not a mouse event that we care about.
        return;
    }
    if (e->type == ButtonRelease)
        return;
    if (e->type == ButtonPress) {
        //cout << e->xbutton.button << endl;
        if (e->xbutton.button== 1) { 
	     int q = g.yres - e->xbutton.y;
	     		
		cout << "test: " << e->xbutton.x << " " << q << endl;
            if (e->xbutton.x > 813 && e->xbutton.x < 1111 && q > 748 && q < 849) {
			    abG.showStartScreen();
			    g.isPaused = false;
		 	    cout << "test: " << e->xbutton.x << " " << q << endl;
	     		} 
			if (!abG.showHow) {
				if (e->xbutton.x > 813 && e->xbutton.x < 1111 && q > 550 && q < 649) {
			            abG.showHowTo();
		 	           // cout << "test: " << e->xbutton.x << " " << q << endl;
				}
			}
			if (abG.showHow) {    
			    if (e->xbutton.x > 886 && e->xbutton.x < 1013 && q > 346 && q < 357) {
			        abG.showHowTo();
			    }
			}
	}

        if (e->xbutton.button== 3) {
        }
    }
    if (e->type == MotionNotify) {
        if (!g.isPaused && g.mouseMovement == true) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
            		//Mouse moved
            		savex = e->xbutton.x;
            		savey = e->xbutton.y;
            		int y = g.yres - e->xbutton.y;
            		p->s.center.x = e->xbutton.x;
            		p->s.center.y = y;
        	}
	}
    }
}

int check_keys(XEvent *e)
{
    //Was there input from the keyboard?
    static int shift = 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
        g.keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    if (e->type == KeyPress) {
        g.keys[key] = 1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift = 1;
		return 0;
	}
    } else {
       return 0;
    }    
        if(shift) {}	
        switch(key) {
            case XK_t:
                abG.incrementScore(1);
                authScores();
                break;
            case XK_h:
                //g.showHighScores ^= 1;
                abG.showHighScores();
                break;
            case XK_c:
                abG.showCredits();
                //g.showCredits ^= 1;
                g.showLogo ^= 1;
                break;
	    case XK_m:
		if(g.mouseMovement == true)
		    g.mouseMovement = false;
		else
		    g.mouseMovement = true;
		break;
	    case XK_p:
                if(g.isPaused)
                    g.isPaused = false;
                else
                    g.isPaused = true;
                break;
            case XK_r:
                rainDrops ^= 1;
                break;
            case XK_s:
                abG.showStartScreen();
		        g.isPaused = false;
                cube ^= 1;
                break;
	    case XK_Escape:
                return 1;
        }
        if (key == XK_Escape)
            return 1;  
    return 0;
}

void physics()
{
    //=========================================================================
    // Background Motion
    //=========================================================================
    
    //=========================================================================
    // Mountain Layer
    //=========================================================================
    g.tex.xc[0] += 0.001;
    g.tex.xc[1] += 0.001;

    //=========================================================================
    // Cloud Layer
    //=========================================================================
    g.tex.xc[2] += 0.002;
    g.tex.xc[3] += 0.002;

    //=========================================================================
    // Pine Tree Layer
    //=========================================================================
    g.tex.xc[4] += 0.008;
    g.tex.xc[5] += 0.008;

    //=========================================================================
    // Enemy Physics
    //=========================================================================
 
    struct Node* temp = g.head;
	Player *p = &g.player;
    for (int i = 0; i < g.n; i++) {
        if (temp != NULL) {
	        moveEnemy(temp);
		    checkEnemyCollision(temp);
		    checkEnemyLocation(temp,p->currentHealth);
            if (temp->data.removeEnemy) {
                removeEnemy(&g.head, temp, g.n, g.enemies1Dead, g.wave);
            }
            temp = temp->next;
	    }
    }
    if(g.mouseMovement == false) {
        if (g.keys[XK_Left] && p->s.center.x > 10) {
            movePlayerLeft(p);
        }
        if (g.keys[XK_Right] && p->s.center.x < 1910) {
            movePlayerRight(p);
        }
        if (g.keys[XK_Up] && p->s.center.y < 1070) {
            movePlayerUp(p);
        }
        if (g.keys[XK_Down] && p->s.center.y > 10) {
	    movePlayerDown(p);
    	}
    }
    
    //key press for bullets
    if (g.getBTime == 1) {
        clock_gettime(CLOCK_REALTIME, &g.bTimeStart);
        g.getBTime = 0;
    }
    
    if (g.keys[XK_space]) {
        if (!g.isPaused) {
            clock_gettime(CLOCK_REALTIME, &g.bTimeCurr);
            g.bTime = timeDiff(&g.bTimeStart, &g.bTimeCurr);
            if (g.bTime > 0.1) {
                if (getPower() == 1)
                    makeBullet(p->s.center.x - 10, p->s.center.y - 33);
                else if (getPower() == 2) {
                    makeBullet(p->s.center.x - 10, p->s.center.y - 29);
                    makeBullet(p->s.center.x - 10, p->s.center.y - 37);
                }
                else if (getPower() == 3) {
                    makeBullet(p->s.center.x - 10, p->s.center.y - 25);
                    makeBullet(p->s.center.x - 10, p->s.center.y - 33);
                    makeBullet(p->s.center.x - 10, p->s.center.y - 41);
                }
                else if (getPower() == 4) {
                    makeMissile(p->s.center.x - 10, p->s.center.y - 33);
                }
                else if (getPower() >= 5) {
                    makeMissile(p->s.center.x - 10, p->s.center.y - 13);
                    makeMissile(p->s.center.x - 10, p->s.center.y - 53);
                }
                g.getBTime = 1;
            }
        }
    }        

    abG.showGameOver(p->currentHealth);
    smokeMovement();
    bulletMovement(p->currentHealth);
    missileMovement(p->currentHealth);
    confettiMovement();
    rainMovement();
    eLex.testMovement(p->currentHealth);
    eLex.vEnemMovement(p->currentHealth);
    eLex.cEnemMovement(p->currentHealth);
    eLex.bossMovement();
    eLex.bulletMovement();
}

void render()
{
    extern ABarGlobal abG;
    extern Enemy eLex;
    Rect r;
    Rect pd;

    //All of these scrolling background layers were done by Alonso Gomez
	
    //=========================================================================
    // Background Layers
    //=========================================================================
	
    //=========================================================================
    // Mountain Layer
    //=========================================================================
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
    
    //=========================================================================
    // Rain Particles
    //=========================================================================
    if (!g.isPaused) {
        if (rainDrops) {
            makeRain();
            makeRain();
            makeRain();
            makeRain();
            makeRain();
            makeRain();
        }
    }
    printRain();
    //=========================================================================
    // Cloud Layer
    //=========================================================================
    // Done by Alonso Gomez
    glBindTexture(GL_TEXTURE_2D, g.cloudTexture);
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
	
    //=========================================================================
    // Pine Tree Layer
    //=========================================================================
    //Done by Alonso Gomez
    glBindTexture(GL_TEXTURE_2D, g.pineTexture);
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
    
    //=========================================================================
    // Creating Player
    //=========================================================================
    Player *p = &g.player;
    if(!p->s.playerExists) {
        spawnPlayer(p);
	setPlayerHealth(p, 3);
        p->s.playerExists = true;
    }
    checkPlayerLocation(p);
    //printPlayer(p);
    //glColor3ub(190,140,10);
    //=========================================================================
    // Smoke Following Player
    //=========================================================================
    if(!g.isPaused) {
	makeSmoke(p->s.center.x, p->s.center.y);
   	makeSmoke(p->s.center.x, p->s.center.y);
    	makeSmoke(p->s.center.x, p->s.center.y);
    	makeSmoke(p->s.center.x, p->s.center.y);
    }
    printSmoke();
    printPlayer(p, img[10].width, img[10].height, g.playerTexture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);

    //=========================================================================
    // Bullets
    //=========================================================================
    if (getPower() < 4)
        printBullet(img[8].width, img[8].height, g.bulletTexture);
    if (getPower() >= 4)
        printMissile(img[9].width, img[9].height, g.missileTexture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //=========================================================================
    // Cube Powerup
    //=========================================================================
    if (!g.isPaused) {
            if (g.getCTime == true || getCubeCollision(0) == true) {
                clock_gettime(CLOCK_REALTIME, &g.cTimeStart);
                g.getCTime = false;
                getCubeCollision(1);
                makeCubeCoordsNull();
            }

            clock_gettime(CLOCK_REALTIME, &g.cTimeCurr);

            g.cTime = timeDiff(&g.cTimeStart, &g.cTimeCurr);
            if (g.cTime > 2.0) {
                glPushMatrix();
                cubePower(g.minecraftTop, g.minecraftSide, g.minecraftBottom);
                glPopMatrix();
                if (g.cTime > 7.0) {
                    g.getCTime = true;
                    makeCubeCoordsNull();
                }
            }
    }

    //=========================================================================
    // Pyramid Powerup
    //=========================================================================
    if (!g.isPaused) {
            if (g.getPTime == true || getPyramidCollision(0) == true) {
                clock_gettime(CLOCK_REALTIME, &g.pTimeStart);
                g.getPTime = false;
                getPyramidCollision(1);
                makePyramidCoordsNull();
            }

            clock_gettime(CLOCK_REALTIME, &g.pTimeCurr);

            g.pTime = timeDiff(&g.pTimeStart, &g.pTimeCurr);
            if (g.pTime > 3.0) {
                glPushMatrix();
                pyramidPower(g.yugiohFront, g.yugiohBack, g.yugiohBottom);
                glPopMatrix();
                if (g.pTime > 8.0) {
                    g.getPTime = true;
                    makePyramidCoordsNull();
                }
            }
    }

    //=========================================================================
    // Creating Enemies
    //=========================================================================

    //=========================================================================
    // Alexis Enemies
    //=========================================================================
    if (g.wave > 3 && g.wave % 2 == 0) {
        if (eLex.getNumEnemy() < eLex.getMAXENEMIES() && eLex.rDead) {
            eLex.makeTest();
            if (eLex.getNumEnemy() == eLex.getMAXENEMIES()) {
                eLex.rDead = false;
            }
        }
    }
    eLex.printTest(img[12].width, img[12].height, g.enemy2Texture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (g.wave == 2) {
        if (eLex.getVNumEnemy() < eLex.getMAXENEMIES() && eLex.vDead) {
            eLex.makeVEnem();
            if (eLex.getVNumEnemy() == eLex.getMAXENEMIES()) {
                eLex.vDead = false;
            }
        }
        if (eLex.getVNumEnemy() == 0) {
            g.wave++;
            eLex.vDead = true;
        }
    }
    eLex.printVEnem(img[11].width, img[11].height, g.enemy1Texture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    

    if (g.wave == 3 || (g.wave > 4 && g.wave % 2 == 1)) {
        if (eLex.getCNumEnemy() < eLex.getCHECKMAXENEM() && eLex.cDead) {
            eLex.makeCEnem();
            if (eLex.getCNumEnemy() == eLex.getCHECKMAXENEM()) {
                eLex.cDead = false;
            }
        }
        if (eLex.getCNumEnemy() == 0) {
            g.wave++;
            eLex.cDead = true;
            eLex.rDead = true;
        }
    }
    eLex.printCEnem(img[13].width, img[13].height, g.enemy3Texture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //=========================================================================
    // Alexis Boss
    //========================================================================= 
    //eLex.makeBoss(1850, 610);
    //eLex.printBoss();

    //eLex.makeEBullet(eLex.bossX - 100, eLex.bossY);
    //eLex.printEBullet();

    //=========================================================================
    // Diego Enemies
    //=========================================================================
    Enemy1 e = g.enemy;
    struct Node* temp = g.head;
    if (g.wave == 1 || (g.wave > 3 && g.wave % 2 == 0)) {
        while( g.n < g.maxEnemy1 && g.enemies1Dead) {
            spawnEnemy(&g.head, e);
            temp = g.head;
            setEnemySize(temp, g.n);
		    setEnemyHealth(temp, 20);
            g.n++;
		    if ( g.n == 5) {
			    g.enemies1Dead = false;
		    }
        }
    }
    printEnemy(temp, g.n, img[11].width, img[11].height, g.enemy1Texture);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
        
    //=========================================================================
    // Screens
    //=========================================================================
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

    //=========================================================================
    // Start Screen
    //=========================================================================
    if (abG.showStart) {
        abG.condenseStart();	
    }

    //=========================================================================
    // How to Screen
    //=========================================================================
    if (abG.showHow) {
        abG.condenseHow();	
    }

    //=========================================================================
    // Credit Screen
    //=========================================================================
    if (abG.showCreds) {
        abG.condenseCreds();
        abG.printPicture(logo.pos[0], logo.pos[1], logo.pos[2], g.logoTexture);
        abG.printPicture(picture.pos[0],  picture.pos[1],  0, g.alexisTexId);
        abG.printPicture(picture2.pos[0], picture2.pos[1], 0, g.alonsoTexId);
        abG.printPicture(picture3.pos[0], picture3.pos[1], 0, g.diegoTexId);
        //abG.printPicture(picture4.pos[0], picture4.pos[1], 0, g.andrewTexId);
        showAlonsoText(r);
    }

    //=========================================================================
    // Credit Screen
    //=========================================================================
    if (abG.showGameIsOver) {
        glBindTexture(GL_TEXTURE_2D, g.andrewTexId);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.0); glVertex2i(0, 0);
            glTexCoord2f(0.0, 0.0); glVertex2i(0, g.yres);
            glTexCoord2f(1.0, 0.0); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0, 1.0); glVertex2i(g.xres, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //=========================================================================
    // High Score Screen
    //=========================================================================
    if (abG.showHigh) {
        abG.condenseHigh();
        if (!g.isPaused)
            makeConfetti();
        printConfetti();
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

    //=========================================================================
    // On Screen Text
    //=========================================================================
    
    if (getPrintPoints()) {
        pd.bot = getPointsX();
        pd.left = getPointsY();
        pd.center = 0;
        ggprint16(&pd, 16, 0x0ffff44, "+1000");
    }

    //unsigned int c = 0x00ffff44;
    r.bot = 100;
    r.left = 40;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffff44, "Press C to go to credits");
    ggprint16(&r, 16, 0x00ffff44, "Press H to go to High Score screen");
    ggprint16(&r, 16, 0x00ffff44, "Press R for rain");
    r.bot = 1050;
    r.left = 10;
    if (g.mouseMovement)
        ggprint16(&r, 0, 0x00ffff44, "Mouse");
    else
	ggprint16(&r, 0, 0x00ffff44, "Keys");
    r.bot = 1050;
    r.left = 80;
    ggprint16(&r,  0, 0x00ffff44, "SCORE: ");
    r.left = 160;
    ggprint16(&r, 16, 0x00ffff44, to_string(abG.highscore).c_str());
}

