#include "common.h"

#include "input.h"

extern App app;

static void doKeyUp(SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
		app.keyboard[event->keysym.scancode] = 0;
}

static void doKeyDown(SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
		app.keyboard[event->keysym.scancode] = 1;
}

static void doMouseButtonUp(SDL_MouseButtonEvent *event) {
	app.mouse.button[event->button] = 0;
}

static void doMouseButtonDown(SDL_MouseButtonEvent *event) {
	app.mouse.button[event->button] = 1;
}

void doInput(void) {
	SDL_Event event;

	memset(app.inputText, '\0', MAX_LINE_LENGTH);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
	
		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			doKeyDown(&event.key);
			break;

		case SDL_KEYUP:
			doKeyUp(&event.key);
			break;

		case SDL_MOUSEBUTTONDOWN:
			doMouseButtonDown(&event.button);
			break;

		case SDL_MOUSEBUTTONUP:
			doMouseButtonUp(&event.button);
			break;
			
		case SDL_TEXTINPUT:
			STRNCPY(app.inputText, event.text.text, MAX_LINE_LENGTH);
			break;

		default:
			break;
		}
	}
	SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
}
