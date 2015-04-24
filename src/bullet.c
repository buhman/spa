#include <stdio.h>
#include <math.h>
#include <sys/queue.h>
#include <allegro5/allegro.h>

#include "config.h"
#include "entity.h"
#include "bullet.h"

ALLEGRO_BITMAP *bullet_bitmap = NULL;

bullet* spa_bullet_create(float x, float y, float x_vel, float y_vel, float theta, float theta_vel) {

  bullet *b = NULL;

  {
    b = malloc(sizeof(bullet));
    if (!b) {
      fprintf(stderr, "malloc(): failed\n");
      return NULL;
    }
  } /* ... */

  spa_entity_init((entity*)b, x, y - 1, x_vel, y_vel, theta, theta_vel);

  b->e.width = SPA_BULLET_RADIUS;
  b->e.height = SPA_BULLET_RADIUS;

  return b;
}

void spa_add_bullet(entity_list *lh, entity *e) {

  int x = e->x + ((SPA_BULLET_RADIUS + e->width) * cos(e->theta - M_PI_2));
  int y = e->y + ((SPA_BULLET_RADIUS + e->height) * sin(e->theta - M_PI_2));

  bullet *bullet = spa_bullet_create(x, y, e->x_vel, e->y_vel, e->theta, 0);

  LIST_INSERT_HEAD(lh, (entity*)bullet, list);
}
