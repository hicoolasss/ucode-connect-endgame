
#include "common.h"

#include "draw.h"
#include "items.h"
#include "sound.h"

extern Entity *player;
extern Entity *self;
extern Stage   stage;

static void tick(void);

static void addHealthPowerup(int x, int y);
static void addRiflePowerup(int x, int y);
static void addShotgunPowerup(int x, int y);
static void rifleTouch(Entity *other);
static void shotgunTouch(Entity *other);
static void healthTouch(Entity *other);


static SDL_Texture *rifleTexture;
static SDL_Texture *shotgunTexture;
static SDL_Texture *healthTexture;


void initItems(void) {
	rifleTexture = loadTexture("resource/images/rifle.png");
	shotgunTexture = loadTexture("resource/images/shotgun3.png");
	healthTexture = loadTexture("resource/images/health3.png");
}

void addRandomPowerup(int x, int y) {
	int r;

	r = rand() % 5;

	if (r == 0) addHealthPowerup(x, y);
	else if (r < 3) addRiflePowerup(x, y);
	else addShotgunPowerup(x, y);
}

static Entity *createPowerup(int x, int y) {
	Entity *e;

	e = malloc(sizeof(Entity));
	memset(e, 0, sizeof(Entity));
	stage.entityTail->next = e;
	stage.entityTail = e;

	e->x = x;
	e->y = y;
	e->health = FPS * 5;
	e->tick = tick;
	e->radius = 16;

	e->dx = -200 + (rand() % 400);
	e->dy = -200 + (rand() % 400);

	e->dx /= 100;
	e->dy /= 100;

	e->color.r = e->color.g = e->color.b = e->color.a = 255;

	return e;
}

static void tick(void) {
	self->health--;

	self->dx *= 0.98;
	self->dy *= 0.98;
}

static void addHealthPowerup(int x, int y) {
	Entity *e;

	e = createPowerup(x, y);

	e->texture = healthTexture;
	e->touch = healthTouch;
}

static void addRiflePowerup(int x, int y) {
	Entity *e;

	e = createPowerup(x, y);

	e->texture = rifleTexture;
	e->touch = rifleTouch;
}

static void addShotgunPowerup(int x, int y) {
	Entity *e;

	e = createPowerup(x, y);

	e->texture = shotgunTexture;
	e->touch = shotgunTouch;
}

static void rifleTouch(Entity *other) {
	if (other == player) {
		self->health = 0;

		stage.ammo[WPN_RIFLE] += 50;

		playSound(SND_AMMO, CH_ITEM);
	}
}

static void shotgunTouch(Entity *other) {
	if (other == player) {
		self->health = 0;

		stage.ammo[WPN_SHOTGUN] += 7;

		playSound(SND_AMMO, CH_ITEM);
	}
}

static void healthTouch(Entity *other) {
	if (other == player) {
		self->health = 0;

		player->health += 3;

		playSound(SND_AMMO, CH_ITEM);
	}
}


