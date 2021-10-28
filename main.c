#include <SDL2/SDL.h>
#include <stdbool.h>
#include "sdlInstance.h"
#include "map.h"
#include "actor.h"

struct map map;
struct actor player, bot;

void init() {
	loadMap(&map, "map.txt");
	initActor(&player, &map);
	initActor(&bot, &map);
}

void end() {
	freeMap(&map);
}

void draw() {
	SDL_RenderClear(renderer);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	int xo, yo;
	if(w/64 < map.w)
		xo = player.pos.x - w/2;
	else
		xo = map.w*32-32 - w/2;
	yo = player.pos.y - h/2;
	drawMap(map, xo, yo);
	drawActors(xo, yo);
	SDL_RenderPresent(renderer);
}

void update() {
	controlActor(&player);
	aiControlActor(&bot);
	updateActors();
}

int main(int argc, char **args) {
	initSDL();
	init();
	
	bool quit = false;
	int lastUpdate = SDL_GetTicks();
	
	while(!quit) {
		static SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		
		int currentTime = SDL_GetTicks();
		while(currentTime - lastUpdate > 15) {
			update();
			lastUpdate += 15;
		}
		draw();
	}
	
	end();
	endSDL();
	
	return 0;
}