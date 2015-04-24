#include <stdlib.h>
#include <math.h>
#include <sys/queue.h>

#include <allegro5/allegro.h>

#include "poof.h"

const float interval = 0.6283185307179586;

poof* spa_poof_create(int x, int y, ALLEGRO_COLOR color) {

  poof *p = malloc(sizeof(poof));

  p->x = x;
  p->y = y;
  p->color = color;

  return p;
}

void spa_poof_destroy(poof* p) {
  if (p)
    free(p);
}

void spa_poof_add(poof_list* lh, int x, int y, ALLEGRO_COLOR color) {

  poof* p = spa_poof_create(x, y, color);
  LIST_INSERT_HEAD(lh, p, poof_p);
}

poof* spa_poof_remove(poof* p) {

  poof *p2;
  LIST_REMOVE(p, poof_p);
  p2 = p->poof_p.le_next;
  spa_poof_destroy(p);
  return p2;
}

void spa_poof_draw(poof* p) {

  for (int i=0; i < 10; i++) {

    float theta = interval * i;
    float x = p->iteration * cos(theta) + p->x;
    float y = p->iteration * sin(theta) + p->y;
    al_draw_pixel(x, y, p->color);
  }

  p->iteration++;
}
