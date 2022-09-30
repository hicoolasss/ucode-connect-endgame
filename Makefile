NAME = endgame

SRC_D = src

INC_D = inc

SRC_F = src/*.c

INCLUDE = -I/usr/include/SDL2 -D_REENTRANT -lSDL2

CFLAGS = clang -std=c11 -Wall -Wextra -Werror -Wpedantic

SDL_FLAGS =  -lSDL2_mixer -lSDL2main -lSDL2_ttf -lSDL2_image -Wno-switch -lm

RM = rm -rf

all: clean $(NAME)

$(NAME):
	@$(CFLAGS) $(SDL_FLAGS) $(INCLUDE) $(SRC_F) -o $@ -I $(INC_D)

clean:
	@$(RM) $(NAME)

uninstall: clean
	@$(RM) $(NAME)

reinstall: uninstall all
