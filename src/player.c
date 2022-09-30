#include <SDL2/SDL_mixer.h>
#include "common.h"

#include "bullets.h"
#include "draw.h"
#include "effects.h"
#include "player.h"
#include "sound.h"
#include "util.h"

extern App app;
extern Entity *player;
extern Stage stage;

static void die(void);

static SDL_Texture *playerTexture;
static SDL_Texture *shotgun_idle_r;
static SDL_Texture *shotgun_idle_l;
static SDL_Texture *shotgun_run_r;
static SDL_Texture *shotgun_run_l;
// Rifle
static SDL_Texture *rifle_idle_r;
static SDL_Texture *rifle_idle_l;
static SDL_Texture *rifle_run_r;
static SDL_Texture *rifle_run_l;
static SDL_Texture *rifle_run_rs;
static SDL_Texture *rifle_run_ls;
// Pistol
static SDL_Texture *pistol_idle_r;
static SDL_Texture *pistol_idle_l;
static SDL_Texture *pistol_run_r;
static SDL_Texture *pistol_run_l;
static SDL_Texture *pistol_run_rs;
static SDL_Texture *pistol_run_ls;

int SpeedPlayer = PLAYER_SPEED;

void initPlayer(void)
{

	playerTexture = loadTexture("resource/Idle1_right.png");
	shotgun_run_l = loadTexture("resource/models/shotgun/Run2_shotgun_l.png");
	shotgun_run_r = loadTexture("resource/models/shotgun/Run2_shotgun_r.png");
	shotgun_idle_r = loadTexture("resource/models/shotgun/Idle2_shotgun_r.png");
	shotgun_idle_l = loadTexture("resource/models/shotgun/Idle2_shotgun_l.png");
	rifle_idle_r = loadTexture("resource/models/rifle/Idle1_right.png");
	rifle_idle_l = loadTexture("resource/models/rifle/Idle1_left.png");
	pistol_idle_r = loadTexture("resource/models/pistol/Idle1_pistol_r.png");
	pistol_idle_l = loadTexture("resource/models/pistol/Idle1_pistol_l.png");
	rifle_run_r = loadTexture("resource/models/rifle/Run1_right.png");
	rifle_run_l = loadTexture("resource/models/rifle/Run1_left.png");
	rifle_run_rs = loadTexture("resource/models/rifle/Run2_right.png");
	rifle_run_ls = loadTexture("resource/models/rifle/Run2_left.png");
	pistol_run_r = loadTexture("resource/models/pistol/Run1_pistol_r.png");
	pistol_run_l = loadTexture("resource/models/pistol/Run1_pistol_l.png");
	pistol_run_rs = loadTexture("resource/models/pistol/Run2_pistol_r.png");
	pistol_run_ls = loadTexture("resource/models/pistol/Run2_pistol_l.png");
}

void addPlayer(void)
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.entityTail->next = player;
	stage.entityTail = player;

	player->texture = playerTexture;
	player->health = 15;
	player->x = SCREEN_WIDTH / 2;
	player->y = SCREEN_HEIGHT / 2;
	player->radius = 10;
	player->side = SIDE_PLAYER;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
	player->color.r = player->color.g = player->color.b = player->color.a = 255;

	stage.ammo[WPN_PISTOL] = 15;
	stage.ammo[WPN_RIFLE] = 150;
	stage.ammo[WPN_SHOTGUN] = 35;

	player->die = die;

	loadMusic("resource/music/fight_looped.ogg");
	playMusic(1);
}

void doPlayer(void)
{

	if (player != NULL)
	{

		bool run = false;
		bool idle = true;
		player->dx *= 0.85;
		player->dy *= 0.85;

		if (stage.score >= 100 && stage.score <= 200) SpeedPlayer = PLAYER_SPEED + 1;
		if (stage.score > 200 && stage.score < 300) SpeedPlayer = PLAYER_SPEED;
		if (stage.score >= 300 && stage.score <= 400) SpeedPlayer = PLAYER_SPEED + 2;
		if (stage.score > 400 && stage.score <= 500) SpeedPlayer = PLAYER_SPEED;
		if (stage.score > 1000 && stage.score < 2000) SpeedPlayer = PLAYER_SPEED + 1;
		if (stage.score == 2000) SpeedPlayer = PLAYER_SPEED;
		if (stage.score > 2000 && stage.score < 3000) SpeedPlayer = PLAYER_SPEED + 2;
		if (stage.score == 3000) SpeedPlayer = PLAYER_SPEED;
		if (stage.score > 3000) SpeedPlayer = PLAYER_SPEED + 4;

		if (app.keyboard[SDL_SCANCODE_1]) {
			player->weaponType = WPN_PISTOL;
			app.keyboard[SDLK_1] = 1;
		} else if (app.keyboard[SDL_SCANCODE_2]) {
			player->weaponType = WPN_RIFLE;
			app.keyboard[SDL_SCANCODE_2] = 1;
		} else if (app.keyboard[SDL_SCANCODE_3]) {
				player->weaponType = WPN_SHOTGUN;
			app.keyboard[SDL_SCANCODE_3] = 1;
		}

		if (app.keyboard[SDL_SCANCODE_R]) {
			if (player->weaponType == WPN_PISTOL)
				stage.ammo[WPN_PISTOL] = 15;
			app.keyboard[SDL_SCANCODE_R] = 1;
		}

		if (app.keyboard[SDL_SCANCODE_W]) player->dy = -SpeedPlayer;
		if (app.keyboard[SDL_SCANCODE_S]) player->dy = SpeedPlayer;
		if (app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) player->dx = -SpeedPlayer;
		if (app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) player->dx = SpeedPlayer;
		if (app.keyboard[SDL_SCANCODE_W] || app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_A]) {
			run = true;
			idle = false;
		} else if (!app.keyboard[SDL_SCANCODE_W] || !app.keyboard[SDL_SCANCODE_S] || !app.keyboard[SDL_SCANCODE_D] || !app.keyboard[SDL_SCANCODE_A]) {
			run = false;
			idle = true;
		}

		if (player->weaponType == WPN_RIFLE) {
			if (idle && !run && app.mouse.button[SDL_BUTTON_LEFT]) player->texture = rifle_idle_r;
			if (idle && !run && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = rifle_idle_r;
			if (!app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D]) player->texture = rifle_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) player->texture = rifle_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) player->texture = rifle_run_r;
			if (app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) player->texture = rifle_run_r;

			if (app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = rifle_run_r;
				player->texture = rifle_run_r;
			}
			if (app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = rifle_run_l;
				player->texture = rifle_run_l;
			}
			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_W]) player->texture = rifle_run_l;
			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_S]) player->texture = rifle_run_l;

			if (app.mouse.button[SDL_BUTTON_LEFT]) {
				if (app.keyboard[SDL_SCANCODE_A])
					player->texture = rifle_run_ls;
				if (app.keyboard[SDL_SCANCODE_D])
					player->texture = rifle_run_rs;

				Entity *b;

				for (b = stage.bulletHead.next; b != NULL; b = b->next) {
					if (!app.keyboard[SDL_SCANCODE_W] || !app.keyboard[SDL_SCANCODE_S] || !app.keyboard[SDL_SCANCODE_D] || !app.keyboard[SDL_SCANCODE_A]) {
						if ((b->x) < (player->x)) player->texture = rifle_idle_l;
						if ((b->x) > (player->x)) player->texture = rifle_idle_r;
					}
					if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_A]) {
						if (!idle && run) {
							if ((b->x) < (player->x)) player->texture = rifle_run_ls;
							if ((b->x) > (player->x)) player->texture = rifle_run_rs;
						}
					}
				}

				if (player->reload == 0 && stage.ammo[player->weaponType] > 0) {
					firePlayerBullet();

					stage.ammo[player->weaponType]--;
				}
			}
		}
		// End rifle output

		// Start pistol output
		if (player->weaponType == WPN_PISTOL) {
			if (idle && !run && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = pistol_idle_r;
			if (idle && !run && app.mouse.button[SDL_BUTTON_LEFT]) player->texture = pistol_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D] && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = pistol_idle_r;
			if (!app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_A] && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = pistol_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) player->texture = pistol_run_r;
			if (app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) player->texture = pistol_run_r;
			if (app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = pistol_run_r;
				player->texture = pistol_run_r;
			}
			if (app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = pistol_run_l;
				player->texture = pistol_run_l;
			}

			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_W]) player->texture = pistol_run_l;
			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_S]) player->texture = pistol_run_l;

			player->angle = getAngle(player->x, player->y, app.mouse.x, app.mouse.y);

			if (app.mouse.button[SDL_BUTTON_LEFT]) {
				if (app.keyboard[SDL_SCANCODE_A])
					player->texture = pistol_run_ls;
				if (app.keyboard[SDL_SCANCODE_D])
					player->texture = pistol_run_ls;

				Entity *b;

				for (b = stage.bulletHead.next; b != NULL; b = b->next)
				{
					if (!app.keyboard[SDL_SCANCODE_W] || !app.keyboard[SDL_SCANCODE_S] || !app.keyboard[SDL_SCANCODE_D] || !app.keyboard[SDL_SCANCODE_A]) {
						if ((b->x) < (player->x)) player->texture = pistol_idle_l;
						if ((b->x) > (player->x)) player->texture = pistol_idle_r;
						if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_A]) {
							if (!idle && run) {
								if ((b->x) < (player->x)) player->texture = pistol_run_ls;
								if ((b->x) > (player->x)) player->texture = pistol_run_rs;
							}
						}
					}
				}

				if (player->reload == 0 && stage.ammo[player->weaponType] > 0) {
					firePlayerBullet();

					stage.ammo[player->weaponType]--;
				}
			}
		}
		// End Pistol Output

		// Start Shotgun Output
		if (player->weaponType == WPN_SHOTGUN) {
			if (idle && !run && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = shotgun_idle_r;
			if (idle && !run && app.mouse.button[SDL_BUTTON_LEFT]) player->texture = shotgun_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D] && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = shotgun_idle_r;
			if (!app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_A] && !app.mouse.button[SDL_BUTTON_LEFT]) player->texture = shotgun_idle_r;
			if (app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) player->texture = shotgun_run_r;
			if (app.keyboard[SDL_SCANCODE_S] && !app.keyboard[SDL_SCANCODE_W] && !app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) player->texture = shotgun_run_r;
			if (app.keyboard[SDL_SCANCODE_D] && !app.keyboard[SDL_SCANCODE_A]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = shotgun_run_r;
				player->texture = shotgun_run_r;
			}
			if (app.keyboard[SDL_SCANCODE_A] && !app.keyboard[SDL_SCANCODE_D]) {
				if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_W])
					player->texture = shotgun_run_l;
				player->texture = shotgun_run_l;
			}
			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_W]) player->texture = shotgun_run_l;
			if (app.keyboard[SDL_SCANCODE_A] && app.keyboard[SDL_SCANCODE_D] && app.keyboard[SDL_SCANCODE_S]) player->texture = shotgun_run_l;

			if (app.mouse.button[SDL_BUTTON_LEFT]) {
				if (app.keyboard[SDL_SCANCODE_A])
					player->texture = shotgun_run_l;
				if (app.keyboard[SDL_SCANCODE_D])
					player->texture = shotgun_run_r;

				Entity *b;

				for (b = stage.bulletHead.next; b != NULL; b = b->next) {
					if (!app.keyboard[SDL_SCANCODE_W] || !app.keyboard[SDL_SCANCODE_S] || !app.keyboard[SDL_SCANCODE_D] || !app.keyboard[SDL_SCANCODE_A]) {
						if ((b->x) < (player->x)) player->texture = shotgun_idle_l;
						if ((b->x) > (player->x)) player->texture = shotgun_idle_r;
						if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_A]) {
							if (!idle && run) {
								if ((b->x) < (player->x)) player->texture = shotgun_run_l;
								if ((b->x) > (player->x)) player->texture = shotgun_run_r;
							}
						}
					}
				}

				if (player->reload == 0 && stage.ammo[player->weaponType] > 0) {
					firePlayerBullet();

					stage.ammo[player->weaponType]--;
				}
			}
		}
	}
}

static void die(void)
{
	addPlayerDeathEffect();

	player = NULL;
	Mix_HaltMusic();
	playSound(SND_PLAYER_DIE, CH_PLAYER);
}
