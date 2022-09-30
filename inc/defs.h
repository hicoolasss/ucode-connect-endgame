#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define STRNCPY(dest, src, n) \
	strncpy(dest, src, n);    \
	dest[n - 1] = '\0'
#define PI 3.14159265358979323846

#define SCREEN_WIDTH  1250
#define SCREEN_HEIGHT 700

#define ARENA_WIDTH	 (SCREEN_WIDTH * 5)
#define ARENA_HEIGHT (SCREEN_HEIGHT * 5)

#define MAX_NAME_LENGTH		  32
#define MAX_LINE_LENGTH		  1024
#define MAX_SCORE_NAME_LENGTH 16

#define FPS 60

#define GRID_SIZE 32

#define GRID_RENDER_WIDTH  (SCREEN_WIDTH / GRID_SIZE)
#define GRID_RENDER_HEIGHT ((SCREEN_HEIGHT / GRID_SIZE) + 1)

#define MAX_KEYBOARD_KEYS 350
#define MAX_MOUSE_BUTTONS 6

#define MAX_SND_CHANNELS 16

#define NUM_HIGHSCORES 8

#define PLAYER_SPEED 5

#define GLYPH_WIDTH	 18
#define GLYPH_HEIGHT 29

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};

enum
{
	WPN_PISTOL,
	WPN_RIFLE,
	WPN_SHOTGUN,
	WPN_MAX
};

enum
{
	SIDE_NONE,
	SIDE_PLAYER,
	SIDE_ENEMY
};

enum
{
	SND_PISTOL,
	SND_RIFLE,
	SND_SHOTGUN,
	SND_ENEMY_BULLET,
	SND_AMMO,
	SND_ENEMY_HIT,
	SND_PLAYER_HIT,
	SND_ENEMY_DIE,
	SND_PLAYER_DIE,
	SND_MAX
};

enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ITEM,
	CH_ENEMY_BULLET,
	CH_HIT
};
