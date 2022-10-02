
#include "common.h"
#include <SDL2/SDL_mixer.h>
#include "draw.h"
#include "stage.h"
#include "text.h"
#include "title.h"
#include "sound.h"

extern App app;

static void logic(void);
static void draw(void);
static void drawLogo(void);

static int timeout;
static SDL_Texture *SkateDestroyerTexture;
static int SkateDestroyerY = -200;
static float SkateDestroyerDY = 0;

void initTitle(void) {
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	memset(&app.mouse, 0, sizeof(Mouse));

	SkateDestroyerTexture = loadTexture("resource/images/skatedestroyer.jpg");
	if (rand() % 5 == 0) {
		SkateDestroyerY = -200;
		SkateDestroyerDY = 0;
	}

	loadMusic("resource/music/MenuOst.mp3");
	playMusic(1);
	timeout = FPS * 5;
}

static void logic(void)
{

	SkateDestroyerDY = MIN(SkateDestroyerDY + 0.25, 25);

	if (SCREEN_WIDTH < 1450)
		SkateDestroyerY = MIN(SkateDestroyerY + SkateDestroyerDY, 200);
	if (SCREEN_WIDTH >= 1450)
		SkateDestroyerY = MIN(SkateDestroyerY + SkateDestroyerDY, 530);
	if (SkateDestroyerY == 200)

		if (SkateDestroyerY == 200) {
			SkateDestroyerDY = -SkateDestroyerDY * 0.5;

			if (SkateDestroyerDY > -1)
				SkateDestroyerDY = 0;
		}
	if (app.mouse.button[SDL_BUTTON_LEFT])
	{
		Mix_HaltMusic();
		initStage();
	}
}

static void draw(void) {
	drawLogo();
}

static void drawLogo(void) {

	blit(SkateDestroyerTexture, SCREEN_WIDTH / 2, SkateDestroyerY, 1);
}
