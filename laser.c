# define M_PI_2     1.57079632679489661923  /* pi/2 */

#include <math.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"
#include "laser.h"
#include "xlines.h"

const int RR = 1.4142135623730951;

void spa_laser_draw(entity* player, int width, int height) {

	int x1 = player->x + (player->width * cos(player->theta - M_PI_2) / RR);
	int y1 = player->y + (player->height * sin(player->theta - M_PI_2) / RR);

	int r = width + height;

	float x2 = cos(player->theta - M_PI_2) * r + x1;
	float y2 = sin(player->theta - M_PI_2) * r + y1;

	al_draw_line(x1, y1, x2, y2,
			al_map_rgb(0, 255, 0), 2);
}

bool spa_laser_collide(entity *e, entity *player, int width, int height) {

	int lx1 = player->x + (player->width * cos(player->theta - M_PI_2) / RR);
	int ly1 = player->y + (player->height * sin(player->theta - M_PI_2) / RR);

	int r = width + height;

	float lx2 = cos(player->theta - M_PI_2) * r + ly1;
	float ly2 = sin(player->theta - M_PI_2) * r + lx1;

	int x2 = e->x + e->width / 2;
	int x1 = e->x - e->width / 2;
	int y2 = e->y + e->height / 2;
	int y1 = e->y - e->width / 2;
	
	if (lines_intersect(x1, y1, x2, y1,
				lx1, ly1, lx2, ly2) ||
			lines_intersect(x2, y1, x2, y2,
				lx1, ly1, lx2, ly2) ||
			lines_intersect(x2, y2, x2, y1,
				lx1, ly1, lx2, ly2) ||
			lines_intersect(x2, y1, x1, y1,
				lx1, ly1, lx2, ly2))
		return true;
	return false;
}


