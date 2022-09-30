#include "common.h"

#include "bullets.h"
#include "draw.h"
#include "effects.h"
#include "enemies.h"
#include "items.h"
#include "sound.h"
#include "util.h"

extern Entity *player;
extern Entity *self;
extern Stage   stage;

static void tick1(void);
static void tick2(void);
static void tick3(void);
static void die(void);

static SDL_Texture *enemyTexture[3];
static SDL_Texture *atack_enemy[3];
void initEnemies(void) {
	enemyTexture[0] = loadTexture("resource/models/skater/Walk.png");
	enemyTexture[1] = loadTexture("resource/models/shooter/Walk.png");
	enemyTexture[2] = loadTexture("resource/models/baseball_bat/Walk.png");
	atack_enemy[0] = loadTexture("resource/models/skater/Death.png");
	atack_enemy[1] = loadTexture("resource/models/shooter/Attack.png");
	atack_enemy[2] = loadTexture("resource/models/baseball_bat/Death.png");
}

void addEnemy(int x, int y) {
	Entity *e;

	e = malloc(sizeof(Entity));
	memset(e, 0, sizeof(Entity));
	stage.entityTail->next = e;
	stage.entityTail = e;

	e->side = SIDE_ENEMY;
	e->x = x;
	e->y = y;

	e->color.r = e->color.g = e->color.b = e->color.a = 255;

	switch (rand() % 12) {
		case 0:
			e->texture = enemyTexture[2];
			e->tick = tick2;
			e->radius = 32;
			e->health = 25;
			break;

		case 1:
		case 2:
			e->texture = enemyTexture[1];
			e->tick = tick3;
			e->radius = 10;
			e->health = 1;
			break;

		default:
			e->texture = enemyTexture[0];
			e->tick = tick1;
			e->radius = 32;
			e->health = 5;
			break;
	}

	SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

	e->die = die;
}

static void tick1(void) {
	if (player != NULL) {
		self->angle = getAngle(self->x, self->y, player->x, player->y);

		calcSlope(player->x, player->y, self->x, self->y, &self->dx, &self->dy);

		self->reload = MAX(self->reload - 1, 0);

		if (self->reload <= 0 && getDistance(self->x, self->y, player->x, player->y) <= 5) {
			punchEnemy(atack_enemy[0]);

			self->reload = FPS * 2;
		}
	}
}

static void tick2(void) {
	float dx, dy;

	if (++self->angle >= 360) self->angle = 0;

	if (player != NULL) {
		calcSlope(player->x, player->y, self->x, self->y, &dx, &dy);

		dx /= 1;
		dy /= 1;

		self->dx += dx;
		self->dy += dy;

		self->dx = MAX(MIN(self->dx, 4), -4);
		self->dy = MAX(MIN(self->dy, 4), -4);

		self->reload = MAX(self->reload - 1, 0);

		if (self->reload <= 0 && getDistance(self->x, self->y, player->x, player->y) <= 5) {
			punchEnemy(atack_enemy[2]);

			self->reload = 0;
		}
	}
}

static void tick3(void) {
	float dx, dy;

	self->angle -= 5;

	if (self->angle < 0) self->angle = 359;

	if (player != NULL) {
		calcSlope(player->x, player->y, self->x, self->y, &dx, &dy);

		dx /= 10;
		dy /= 10;

		self->dx += dx;
		self->dy += dy;

		self->dx = MAX(MIN(self->dx, 5), -5);
		self->dy = MAX(MIN(self->dy, 5), -5);

		self->reload = MAX(self->reload - 1, 0);

		if (self->reload <= 0 && getDistance(self->x, self->y, player->x, player->y) < SCREEN_HEIGHT) {
			fireEnemyBullet();

			self->reload = FPS;
		}
	}
}

static void die(void) {
	addEnemyDeathEffect();

	if (rand() % 3) addRandomPowerup(self->x, self->y);

	stage.score += 10;

	playSound(SND_ENEMY_DIE, -1);
}
