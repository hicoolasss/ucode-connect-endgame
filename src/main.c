#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "title.h"

App		   app;
Entity	   *player;
Entity	   *self;
Stage	   stage;

static void capFrameRate(long *then, float *remainder);

int main() {
	long  then;
	float remainder;

	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	initSDL();

	atexit(cleanup);

	initGame();

	initTitle();

	then = SDL_GetTicks();

	remainder = 0;

	while (1) {
		prepareScene();

		doInput();

		app.delegate.logic();

		app.delegate.draw();

		presentScene();

		capFrameRate(&then, &remainder);
	}

	return 0;
}

static void capFrameRate(long *then, float *remainder) {
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1) wait = 1;

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}
