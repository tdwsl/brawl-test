#ifndef MAP_H
#define MAP_H

#include "xy.h"
#include <stdbool.h>

struct map {
	int w, h, *tiles;
	struct xy spawn;
};

void freeMap(struct map *map);
void loadMap(struct map *map, const char *filename);
void drawMap(struct map map, int xo, int yo);
int mapTile(struct map map, int x, int y);
bool tileBlocks(int t);
bool tileDownBlocks(int t);

#endif
