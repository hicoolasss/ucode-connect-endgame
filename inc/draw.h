void		 blitRotated(SDL_Texture *texture, int x, int y, float angle);
void		 blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
void		 blit(SDL_Texture *texture, int x, int y, int center);
SDL_Texture *loadTexture(char *filename);
void		 presentScene(void);
void		 prepareScene(void);
void blitplayer(SDL_Texture *texture, int x, int y);
