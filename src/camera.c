#include "common.h"

#include "camera.h"

extern Entity *player;
extern Stage   stage;

void doCamera(void) {
	if (player != NULL) {
		stage.camera.x = player->x - (SCREEN_WIDTH / 2);
		stage.camera.y = player->y - (SCREEN_HEIGHT / 2);
	}
}
