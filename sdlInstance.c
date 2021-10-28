#include "sdlInstance.h"
#include <SDL2/SDL.h>
#include <assert.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *playerTex, *tileset;

SDL_Texture *loadTexture(const char *filename) {
	SDL_Surface *loadedSurface = SDL_LoadBMP(filename);
	assert(loadedSurface);
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xff, 0x00, 0xff));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	assert(tex);
	return tex;
}

void initSDL() {
	assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0);
	window = SDL_CreateWindow("street", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
	assert(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	assert(renderer);
	SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xff);
	
	playerTex = loadTexture("img/player.bmp");
	tileset = loadTexture("img/tileset.bmp");
}

void endSDL() {
	SDL_DestroyTexture(playerTex);
	SDL_DestroyTexture(tileset);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}