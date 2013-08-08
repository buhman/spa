# define M_PI_2     1.57079632679489661923  /* pi/2 */

#include <math.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"
#include "laser.h"
#include "xlines.h"

void spa_laser_draw(entity* laser, int width, int height) {

	int r = width + height;

	float x2 = cos(laser->theta - M_PI_2) * r + laser->x;
	float y2 = sin(laser->theta - M_PI_2) * r + laser->y;

	al_draw_line(laser->x, laser->y, x2, y2,
			al_map_rgb(0, 255, 0), 2);
}

bool spa_laser_collide(entity *e, entity *l, int width, int height) {

	int r = width + height;

	float lx2 = cos(l->theta - M_PI_2) * r + l->x;
	float ly2 = sin(l->theta - M_PI_2) * r + l->y;

	int x2 = e->x + e->width / 2;
	int x1 = e->x - e->width / 2;
	int y2 = e->y + e->height / 2;
	int y1 = e->y - e->width / 2;

	long *x = 0, *y = 0;
	
	if (lines_intersect(x1, y1, x2, y1,
				l->x, l->y, lx2, ly2) ||
			lines_intersect(x2, y1, x2, y2,
				l->x, l->y, lx2, ly2) ||
			lines_intersect(x2, y2, x2, y1,
				l->x, l->y, lx2, ly2) ||
			lines_intersect(x2, y1, x1, y1,
				l->x, l->y, lx2, ly2))
		return true;
	return false;
}


