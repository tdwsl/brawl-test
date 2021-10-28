#include "map.h"
#include "sdlInstance.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void loadMap(struct map *map, const char *filename) {
	FILE *fp = fopen(filename, "r");
	assert(fp);
	fscanf(fp, "%d%d", &map->w, &map->h);
	map->tiles = malloc(sizeof(int)*map->w*map->h);
	for(int i = 0; i < map->w*map->h; i++) {
		fscanf(fp, "%d", &map->tiles[i]);
		if(map->tiles[i] == -2) {
			map->tiles[i] = -1;
			map->spawn.x = (i % map->w)*64 + 32;
			map->spawn.y = (i / map->w)*64 + 32;
		}
		/*printf("%3d ", map->tiles[i]);
		if(i % map->w == map->w-1)
			printf("\n");*/
	}
	fclose(fp);
}

void freeMap(struct map *map) {
	free(map->tiles);
}

void drawMap(struct map map, int xo, int yo) {
	SDL_Rect src, dst;
	src.w = 16;
	src.h = 16;
	dst.w = 64;
	dst.h = 64;
	dst.y = -yo;
	for(int y = 0; y < map.h; y++) {
		int *m = &map.tiles[y*map.w];
		dst.x = -xo;
		for(int x = 0; x < map.w; x++) {
			if(m[x] >= 0) {
				src.x = (m[x]%4)*16;
				src.y = (m[x]/4)*16;
				SDL_RenderCopy(renderer, tileset, &src, &dst);
			}
			dst.x += dst.w;
		}
		dst.y += dst.h;
	}
}

int mapTile(struct map map, int x, int y) {
	if(x < 0 || y < 0 || x >= map.w || y >= map.h)
		return -2;
	return map.tiles[y*map.w+x];
}

bool tileBlocks(int t) {
	if(t == -1)
		return false;
	if(t < 3)
		return true;
	return false;
}

bool tileDownBlocks(int t) {
	if(t == 3)
		return true;
	return tileBlocks(t);
}