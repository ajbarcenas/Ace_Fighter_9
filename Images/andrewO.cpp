// Name: Andrew Oliveros
// Date: 9/17/2019
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND
/*
#define MAXBUTTONS 3
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} 
Button;
Button button[MAXBUTTONS];
int nbuttons=0;
*/
class sound
{
	public:
		ALuint alBuffer1_shot;
		ALuint alSource1_shot;
}


//-------------------------- SOUNDS -------------------------------------
void initSound() 
{
    alutInit(0, NULL);
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

}

void soundOneShot() 
{
    Sound s;

    s.alBufferOneShot = alutCreateBufferFromFile("./sounds/1_shot.oog");

    alGenSources(1, &s.alSourceOneShot);
    alSourcei(s.alSourceOneShot, AL_BUFFER, s.alBufferOneShot);

    alSourcef(s.alSourceOneShot, AL_GAIN, 1.0f);
    alSourcef(s.alSourceOneShot, AL_PITCH, 1.0f);
    alSourcei(s.alSourceOneShot, AL_LOOPING, AL_TRUE);

    alSourcePlay(s.alSourceOneShot);
}

void soundThreeShots() 
{
    Sound s;

    s.alBufferThreeShots = alutCreateBufferFromFile("./sounds/3_shos.oog");

    alGenSources(1, &s.alSourceThreeShots);
    alSourcei(s.alSourceThreeShots, AL_BUFFER, s.alBufferThreeShots);

    alSourcef(s.alSourceThreeShots, AL_GAIN, 1.0f);
    alSourcef(s.alSourceThreeShots, AL_PITCH, 1.0f);
    alSourcei(s.alSourceThreeShots, AL_LOOPING, AL_TRUE);

    alSourcePlay(s.alSourceThreeShots);
}

void removeSound() 
{
    Sound s;

    alDeleteSources(1, &s.alSourceOneShot);
    alDeleteSources(1, &s.alSourceThreeShots);

    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);

    alutExit();
}
//-------------------------- END SOUNDS -----------------------------------


void ajPic(int x, int y, GLuint textid){
    Rect r;
    r.bot = 380;
    r.left = 250;
    r.center = 0;
    ggprint16(&r, 16, 0x0084ff, "ANDREW OLIVEROS - TESTING");

}

/*
void initButtons(void) { 

    nbuttons=0;
    //size and position
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = 50;
    button[nbuttons].r.bot = 100;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Button 1");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

    //size and position
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 100;
    button[nbuttons].r.left = 10;
    //button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
    button[nbuttons].r.bot = 50;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Button 2");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

    for (int i=0; i<nbuttons; i++) {
        if (button[i].over) {
            glColor3f(1.0f, 0.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
            glPushMatrix();
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glPopMatrix();
            glEnd();
            glLineWidth(1);
        }
        if (button[i].down) {
            glColor3fv(button[i].dcolor);
        } else {
            glColor3fv(button[i].color);
        }
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex2i(button[i].r.left,  button[i].r.bot);
        glVertex2i(button[i].r.left,  button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.bot);
        glEnd();
        Rect r;
        r.left = button[i].r.centerx;
        r.bot  = button[i].r.centery-8;
        r.center = 1;
        glPopMatrix();
        if (button[i].down) {
            ggprint16(&r, 0, button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button[i].text_color, button[i].text);
        }
    }
}


void buttonDraw(Rect r)
{
    //glDisable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_2D);
    void initButtons();
    for (int i=0; i<nbuttons; i++) {
        if (button[i].over) {
            glColor3f(1.0f, 0.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
            glPushMatrix();
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glPopMatrix();
            glEnd();
            glLineWidth(1);
        }
        if (button[i].down) {
            glColor3fv(button[i].dcolor);
        } else {
            glColor3fv(button[i].color);
        }
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex2i(button[i].r.left,  button[i].r.bot);
        glVertex2i(button[i].r.left,  button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.bot);
        glEnd();
        r.left = button[i].r.centerx;
        r.bot  = button[i].r.centery-8;
        r.center = 1;
        glPopMatrix();
        if (button[i].down) {
            ggprint16(&r, 0, button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button[i].text_color, button[i].text);
        }
    }
}

void mouse_click(int ibutton, int action, int x, int y)
{
    if (action == 1) {
        int i;
        //buttons?
        for (i=0; i<nbuttons; i++) {
            if (button[i].over) {
                button[i].down = 1;
                button[i].click = 1;
                if (i==0) {
                    //user clicked QUIT
                    //done = 1;
                }
                if (i==1) {
                    //user clicked button 0

                }
            }
        }
    }
}
*/
