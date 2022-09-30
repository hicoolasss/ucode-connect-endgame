#include <SDL2/SDL_image.h>

#include "common.h"

#include "draw.h"

extern App app;

void prepareScene(void) {
	SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void) {
	SDL_RenderPresent(app.renderer);
}

static SDL_Texture *getTexture(char *name) {
	Texture *t;

	for (t = app.textureHead.next; t != NULL; t = t->next) {
		if (strcmp(t->name, name) == 0) return t->texture;
	}

	return NULL;
}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture) {
	Texture *texture;

	texture = malloc(sizeof(Texture));
	memset(texture, 0, sizeof(Texture));
	app.textureTail->next = texture;
	app.textureTail = texture;

	STRNCPY(texture->name, name, MAX_NAME_LENGTH);
	texture->texture = sdlTexture;
}

SDL_Texture *loadTexture(char *filename) {
	SDL_Texture *texture;

	texture = getTexture(filename);

	if (texture == NULL) {
		texture = IMG_LoadTexture(app.renderer, filename);
		addTextureToCache(filename, texture);
	}

	return texture;
}


void blit(SDL_Texture *texture, int x, int y, int center) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	if (center) {
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}


void blitplayer(SDL_Texture *texture, int x, int y) {
	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100) % 4;
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	SDL_Rect srcrect = { sprite * 48, 0, 40, 64 };
    SDL_Rect dstrect = { x, y, 40, 64 };

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(app.renderer, texture, &srcrect, &dstrect);
}


void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void blitRotated(SDL_Texture *texture, int x, int y, float angle) {
	SDL_Rect dstRect;

	dstRect.x = x;
	dstRect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dstRect.x -= (dstRect.w / 2);
	dstRect.y -= (dstRect.h / 2);

	SDL_RenderCopyEx(app.renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
}
