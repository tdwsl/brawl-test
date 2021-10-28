#ifndef ACTOR_H
#define ACTOR_H

#include "map.h"
#include "xy.h"
#include <stdbool.h>

struct actor {
	struct xy pos, vel;
	struct map *map;
	bool grounded, flip, punching, kicking, recoiling, fromBehind;
	int cooldown, jump;
};

extern struct actor **actors;
extern int numActors;

void respawnActor(struct actor *a);
void drawActor(struct actor a, int xo, int yo);
void drawActors(int xo, int yo);
void initActor(struct actor *a, struct map *map);
void updateActor(struct actor *a);
void updateActors();
void controlActor(struct actor *a);
void aiControlActor(struct actor *a);

#endif
