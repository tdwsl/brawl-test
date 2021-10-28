#include "actor.h"
#include "map.h"
#include "sdlInstance.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

struct actor **actors;
int numActors = 0;

void initActor(struct actor *a, struct map *map) {
	a->map = map;
	respawnActor(a);
	
	actors = realloc(actors, sizeof(struct actor*)*(numActors+1));
	actors[numActors] = a;
	numActors++;
}

void respawnActor(struct actor *a) {
	a->pos.x = a->map->spawn.x;
	a->pos.y = a->map->spawn.y;
	
	a->vel.x = 0;
	a->vel.y = 0;
	a->jump = 0;
	a->grounded = false;
	a->flip = true;
	a->cooldown = 0;
	a->punching = false;
	a->kicking = false;
	a->recoiling = false;
	a->fromBehind = false;
}

void drawActor(struct actor a, int xo, int yo) {
	SDL_Rect src, dst;
	src.w = 16;
	src.h = 16;
	src.x = 0;
	src.y = 0;
	if(a.vel.x) {
		int s = (a.pos.x / 20) % 4;
		if(s%2)
			src.x = 16 + (s/2)*16;
		else
			src.x = 0;
	}
	if(!a.grounded)
		src.x = 48;
	if(a.punching) {
		src.y = 16;
		if(a.cooldown > 15)
			src.x = 0;
		else
			src.x = 32;
	}
	if(a.kicking) {
		src.y = 16;
		if(a.cooldown > 15)
			src.x = 0;
		else
			src.x = 16;
	}
	if(a.recoiling) {
		src.y = 16;
		src.x = 48;
		if(a.fromBehind) {
			src.x = 0;
			src.y = 32;
		}
	}
	
	dst.w = 64;
	dst.h = 64;
	dst.x = a.pos.x - xo - 32;
	dst.y = a.pos.y - yo - 32;
	SDL_Point pvt;
	pvt.x = 32;
	pvt.y = 32;
	float r = 0;
	SDL_RendererFlip f = SDL_FLIP_NONE;
	if(a.flip)
		f = SDL_FLIP_HORIZONTAL;
	SDL_RenderCopyEx(renderer, playerTex, &src, &dst, r, &pvt, f);
}

void drawActors(int xo, int yo) {
	for(int i = 0; i < numActors; i++)
		drawActor(*actors[i], xo, yo);
}

void updateActor(struct actor *a) {
	for(int i = 0; i < numActors; i++) {
		if(actors[i] == a)
			continue;
		if(!xyIn(a->pos, actors[i]->pos.x-56, actors[i]->pos.y-28, 112, 56))
			continue;
		if((a->pos.x+32 < actors[i]->pos.x && !actors[i]->flip) || (a->pos.x-32 > actors[i]->pos.x && actors[i]->flip))
			continue;
		if(!(actors[i]->kicking || actors[i]->punching))
			continue;
		if(actors[i]->cooldown > 15)
			continue;
		
		if(actors[i]->flip)
			a->vel.x = -8;
		else
			a->vel.x = 8;
		a->vel.y = -5;
		if(actors[i]->kicking) {
			a->vel.x *= 1.2;
			a->vel.y = -7;
		}
		a->fromBehind = (a->flip == actors[i]->flip);
		a->recoiling = true;
		a->cooldown = 20;
		
		a->punching = false;
		a->kicking = false;
	}
	
	int yv = a->vel.y, xv = a->vel.x;
	while(yv || xv) {
		if(xv < 0) {
			a->pos.x--;
			xv++;
		}
		if(xv > 0) {
			a->pos.x++;
			xv--;
		}
		if(yv < 0) {
			a->pos.y--;
			yv++;
		}
		if(yv > 0) {
			a->pos.y++;
			yv--;
		}
	
		if(a->pos.x < 16)
			a->pos.x = 16;
		if(a->pos.x >= a->map->w*64-16)
			a->pos.x = a->map->w*64-16;
		if(a->pos.y < 4)
			a->pos.y = 4;
		if(a->pos.y >= a->map->h*64-32)
			a->pos.y = a->map->h*64-32;
		
		for(int ym = -1; ym <= 1; ym++) {
			int x = a->pos.x;
			int y = a->pos.y + 10 + ym*16;
			int x1 = (x-20)/64;
			int x2 = (x+20)/64;
			x /= 64;
			y /= 64;
			
			if(tileBlocks(mapTile(*a->map, x2, y)))
				a->pos.x -= a->pos.x%64 - 44;
			if(tileBlocks(mapTile(*a->map, x1, y))) {
				a->pos.x += 64;
				a->pos.x -= a->pos.x%64 + 44;
			}
		}
		
		for(int xm = -1; xm <= 1; xm++) {
			int x = a->pos.x + xm*16;
			int y = a->pos.y;
			int y1 = (y-32)/64;
			int y2 = (y+32)/64;
			x /= 64;
			y /= 64;
			
			a->grounded = true;
			if(tileBlocks(mapTile(*a->map, x, y2)))
				a->pos.y -= a->pos.y%64 - 32;
			else if(tileDownBlocks(mapTile(*a->map, x, y2)) && a->vel.y > 0)
				a->pos.y -= a->pos.y%64 - 32;
			else if(!tileDownBlocks(mapTile(*a->map, x, y2)))
				a->grounded = false;
			if(tileBlocks(mapTile(*a->map, x, y1))) {
				a->pos.y += 64;
				a->pos.y -= a->pos.y%64 + 32;
				a->vel.y = 0;
				yv = 0;
			}
		}
	}
	
	if(a->grounded) {
		if(a->vel.x > 0)
			a->vel.x--;
		if(a->vel.x < 0)
			a->vel.x++;
	}
	
	float tyv = 0;
	if(!a->grounded)
		tyv = 16;
	if(a->jump) {
		tyv = -12;
		a->jump--;
	}
	if(a->vel.y > tyv)
		a->vel.y--;
	if(a->vel.y < tyv)
		a->vel.y++;
	
	if(a->cooldown > 0)
		a->cooldown--;
	else {
		a->punching = false;
		a->kicking = false;
		a->recoiling = false;
	}
}

void updateActors() {
	for(int i = 0; i < numActors; i++)
		updateActor(actors[i]);
}

void controlActor(struct actor *a) {
	if(a->punching || a->kicking || a->recoiling)
		return;
	
	const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
	if((keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_SPACE]) && a->grounded && !a->cooldown) {
		a->vel.y = -20;
		a->jump = 5;
	}
	if(keyboardState[SDL_SCANCODE_LEFT] && a->vel.x > -3) {
		a->vel.x -= 2;
		a->flip = true;
	}
	if(keyboardState[SDL_SCANCODE_RIGHT] && a->vel.x < 3) {
		a->vel.x += 2;
		a->flip = false;
	}
	if(keyboardState[SDL_SCANCODE_Z] && !a->cooldown) {
		a->cooldown = 30;
		a->punching = true;
	}
	if(keyboardState[SDL_SCANCODE_X] && !a->cooldown) {
		a->cooldown = 40;
		a->kicking = true;
	}
}

void aiControlActor(struct actor *a) {
	if(a->punching || a->kicking || a->recoiling)
		return;
	
	struct actor *t = 0;
	for(int i = 0; i < numActors && actors[i] != a; i++)
		t = actors[i];
	if(!t)
		return;
	
	if(a->pos.x+48 < t->pos.x && !a->cooldown && a->vel.x < 3) {
		a->vel.x += 2;
		a->flip = false;
	}
	else if(a->pos.x-48 > t->pos.x && !a->cooldown && a->vel.x > -3) {
		a->vel.x -= 2;
		a->flip = true;
	}
	else if(a->pos.y-64 > t->pos.y && !a->cooldown && a->grounded) {
		a->vel.y = -20;
		a->jump = 5;
	}
	else if(!a->cooldown && abs(a->vel.x) < 3)
		a->vel.x += 2 - (t->pos.x < a->pos.x)*4;
	
	if(pow(t->pos.y-a->pos.y, 2) + pow(t->pos.x-a->pos.x, 2) < 60*60 && !a->cooldown) {
		a->cooldown = 31;
		a->punching = true;
	}
}