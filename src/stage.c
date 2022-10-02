#include "common.h"

#include "bullets.h"
#include "camera.h"
#include "draw.h"
#include "effects.h"
#include "enemies.h"
#include "entities.h"
#include "items.h"
#include "player.h"
#include "stage.h"
#include "text.h"
#include "util.h"
#include "sound.h"

extern App app;
extern Entity *player;
extern Stage stage;

static void logic(void);
static void draw(void);
static void drawGrid(void);
static void drawHud(void);
static void drawWeaponInfo(char *name, int type, int x, int y);
static void spawnEnemy(void);

static void resetStage(void);

static SDL_Texture *targetterTexture;
static SDL_Texture *gridTexture[4];
static int enemySpawnTimer;

static int gameOverTimer;
static int scoreDisplay;

void initStage(void) {
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	targetterTexture = loadTexture("resource/images/crosshair005.png");
	gridTexture[0] = loadTexture("resource/images/tile0.png");
	gridTexture[1] = loadTexture("resource/images/tile1.png");
	gridTexture[2] = loadTexture("resource/images/tile2.png");
	gridTexture[3] = loadTexture("resource/images/tile0.png");

	resetStage();

	addPlayer();

	enemySpawnTimer = 0;

	scoreDisplay = 0;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	memset(&app.mouse, 0, sizeof(Mouse));
}

static void resetStage(void) {
	Entity *e;
	Effect *ef;

	while (stage.entityHead.next) {
		e = stage.entityHead.next;
		stage.entityHead.next = e->next;
		free(e);
	}

	while (stage.bulletHead.next) {
		e = stage.bulletHead.next;
		stage.bulletHead.next = e->next;
		free(e);
	}

	while (stage.effectHead.next) {
		ef = stage.effectHead.next;
		stage.effectHead.next = ef->next;
		free(ef);
	}

	memset(&stage, 0, sizeof(Stage));

	stage.entityTail = &stage.entityHead;
	stage.bulletTail = &stage.bulletHead;
	stage.effectTail = &stage.effectHead;

	gameOverTimer = FPS * 2;
}

static void logic(void) {

	doPlayer();

	doEntities();

	doBullets();

	spawnEnemy();

	doEffects();

	doCamera();

	if (scoreDisplay < stage.score)
		scoreDisplay++;

	if (player == NULL && --gameOverTimer <= 0) {

		if (app.mouse.button[SDL_BUTTON_LEFT])
			initStage();
	}
}

static void spawnEnemy(void) {
	int x, y, AddE;
	AddE = 0;

	if (stage.score >= 100) AddE = AddE + 10;
	if (stage.score >= 300) AddE = AddE + 20;
	if (stage.score >= 600)	AddE = AddE + 40;
	if (stage.score >= 1000) AddE = AddE + 8;
	if (stage.score >= 3000) AddE = AddE + 16;

	if (player != NULL && --enemySpawnTimer <= 0) {
		x = player->x + (rand() % SCREEN_WIDTH) - (rand() % SCREEN_WIDTH);
		y = player->y + (rand() % SCREEN_HEIGHT) - (rand() % SCREEN_HEIGHT);

		if (getDistance(x, y, player->x, player->y) > SCREEN_WIDTH / 2) {
			addEnemy(x, y);
			enemySpawnTimer = (rand() % FPS - AddE) + 80;
		}
	}
}

static void draw(void) {
	drawGrid();

	drawEntities();

	drawBullets();

	drawEffects();

	drawHud();

	blit(targetterTexture, app.mouse.x, app.mouse.y, 1);
}

static void drawHud(void) {

	if (stage.score >= 100 && stage.score <= 110) drawText(500, 620, 250, 250, 250, TEXT_RIGHT, "BONUS RECEIVED! +20 SPEED!");
	if (stage.score >= 300 && stage.score <= 330) drawText(500, 620, 250, 250, 250, TEXT_RIGHT, "BONUS RECEIVED! +40 SPEED!");

	drawText(10, 5, 255, 255, 255, TEXT_LEFT, "HEALTH:%d", player != NULL ? player->health : 0);

	drawText(640, 5, 255, 255, 255, TEXT_CENTER, "SCORE:%05d", scoreDisplay);

	drawWeaponInfo("PISTOL", WPN_PISTOL, 1050, 5);

	drawWeaponInfo("RIFLE", WPN_RIFLE, 1050, 35);

	drawWeaponInfo("SHOTGUN", WPN_SHOTGUN, 1050, 65);
}

static void drawWeaponInfo(char *name, int type, int x, int y) {
	int r, g, b;

	if (player != NULL && player->weaponType == type) {
		r = b = 0;
		g = 255;
	} else {
		r = g = b = 255;
	}

	drawText(x, y, r, g, b, TEXT_LEFT, "%s:%03d", name, stage.ammo[type]);
}

static void drawGrid(void) {

	int x1, x2, y1, y2, x, y, mx, my, n;

	x1 = (stage.camera.x % GRID_SIZE) * -1;
	x2 = x1 + GRID_RENDER_WIDTH * GRID_SIZE + (x1 == 0 ? 0 : GRID_SIZE);

	y1 = (stage.camera.y % GRID_SIZE) * -1;
	y2 = y1 + GRID_RENDER_HEIGHT * GRID_SIZE + (y1 == 0 ? 0 : GRID_SIZE);

	mx = stage.camera.x / GRID_SIZE;
	my = stage.camera.y / GRID_SIZE;

	n = 0;

	for (x = x1; x < x2; x += GRID_SIZE) {
		for (y = y1; y < y2; y += GRID_SIZE) {
			if (mx >= 0 && my >= 0 && mx <= (ARENA_WIDTH / GRID_SIZE) - 1 && my <= (ARENA_HEIGHT / GRID_SIZE) - 1) {
				n = ((mx ^ my) / 5) % 4;
				blit(gridTexture[n], x, y, 0);
			}
			my++;
		}
		my = stage.camera.y / GRID_SIZE;
		mx++;
	}
}
