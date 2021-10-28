#ifndef XY_H
#define XY_H

#include <stdbool.h>

struct xy {
	int x, y;
};

bool xyIn(struct xy p, int x, int y, int w, int h);

#endif