#include "xy.h"
#include <stdbool.h>

bool xyIn(struct xy p, int x, int y, int w, int h) {
	return (p.x >= x && p.y >= y && p.x < x+w && p.y < y+h);
}