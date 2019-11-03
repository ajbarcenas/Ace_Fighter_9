// Name: Andrew Oliveros
// Date: 9/17/2019
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"
#include <AL/al.h>
#include <AL/alut.h>

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
/*
//---------------------- SOUNDS Andrew Oliveros 10/12/19 ---------------------
class Sound
{
	public:
		ALuint alBufferOneShot;
        ALuint alBufferThreeShots;


        ALuint alSourceOneShot;
        ALuint alSourceThreeShots;

};

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

//------------------- SOUNDS END Andrew Oliveros 10/12/19 --------------------
*/

void ajPic(int x, int y, GLuint textid){
    Rect r;
    r.bot = 380;
    r.left = 250;
    r.center = 0;
    ggprint16(&r, 16, 0x0084ff, "ANDREW OLIVEROS - TESTING");

}

void hpDisplay(int x, int y, int hp, GLuint textid) {
    hp = 0;
    Rect r;
    r.bot = 80;
    r.left = 250;
    r.center = 0;
    ggprint16(&r, 16, 0x0084ff, "Current HP %d", hp);

}


/*
void playerHP (int currentHP, int pATK){




}


void enemyHP (int enemyCurrentHP, int eATK){
	



}



*/
