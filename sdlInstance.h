#ifndef SDLINSTANCE_H
#define SDLINSTANCE_H

#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *playerTex, *tileset;

void initSDL();
void endSDL();

#endif
