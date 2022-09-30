#include <SDL2/SDL_mixer.h>

#include "common.h"

#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;

void initSounds(void) {
	memset(sounds, 0, sizeof(Mix_Chunk *) * SND_MAX);

	music = NULL;

	loadSounds();
}

void loadMusic(char *filename) {
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	music = Mix_LoadMUS(filename);
}

void playMusic(int loop) {
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel) {
	Mix_PlayChannel(channel, sounds[id], 0);
}

static void loadSounds(void) {
	sounds[SND_PISTOL] = Mix_LoadWAV("resource/sound/usp.ogg");
	sounds[SND_RIFLE] = Mix_LoadWAV("resource/sound/shoot-sound.mp3");
	sounds[SND_SHOTGUN] = Mix_LoadWAV("resource/sound/shotgun.ogg");
	sounds[SND_AMMO] = Mix_LoadWAV("resource/sound/awp_draw.mp3");
	sounds[SND_ENEMY_BULLET] = Mix_LoadWAV("resource/sound/enemyfire.ogg");
	sounds[SND_ENEMY_HIT] = Mix_LoadWAV("resource/sound/damage3.ogg");
	sounds[SND_PLAYER_HIT] = Mix_LoadWAV("resource/sound/minecraft-death-sound.mp3");
	sounds[SND_ENEMY_DIE] = Mix_LoadWAV("resource/sound/death3.ogg");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("resource/sound/tf_nemesis.mp3");
}
