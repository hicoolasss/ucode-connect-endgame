#include "common.h"

#include "draw.h"
#include "entities.h"
#include "util.h"

extern Entity *player;
extern Entity *self;
extern Stage stage;
extern App app;

static void touchOthers(void);

void doEntities(void) {
        
	Entity *e, *prev;

	prev = &stage.entityHead;

	for (e = stage.entityHead.next; e != NULL; e = e->next) {
		self = e;

		if (e->tick) e->tick();

		e->x += e->dx;
		e->y += e->dy;

		if (e->touch) touchOthers();

		e->reload = MAX(e->reload - 1, 0);

		if (e == player) {       
			e->x = MIN(MAX(e->x, e->w / 2), ARENA_WIDTH - e->w / 2);
			e->y = MIN(MAX(e->y, e->h / 2), ARENA_HEIGHT - e->h / 2);
		}

		if (e->health <= 0) {
			if (e->die) e->die();

			if (e == stage.entityTail) stage.entityTail = prev;

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

static void touchOthers(void) {
	Entity *e;
	int distance;

	for (e = stage.entityHead.next; e != NULL; e = e->next) {
		if (e != self) {
			distance = getDistance(self->x, self->y, e->x, e->y);

			if (distance < e->radius + self->radius) self->touch(e);
		}
	}
}

void drawEntities(void) {
	Entity *e;

	for (e = stage.entityHead.next; e != NULL; e = e->next) {
		SDL_SetTextureColorMod(e->texture, e->color.r, e->color.g, e->color.b);

		blitplayer(e->texture, e->x - stage.camera.x, e->y - stage.camera.y - 40);
	}
}
