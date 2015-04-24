#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "entity.h"

entity* spa_entity_create(float x, float y, float x_vel, float y_vel, float theta, float theta_vel) {

  entity *e = NULL;

  {
    e = malloc(sizeof(entity));
    if (!e) {
      fprintf(stderr, "malloc(): failed\n");
      return NULL;
    }
  } /* ... */

  {
    spa_entity_init(e, x, y, x_vel, y_vel, theta, theta_vel);
  } /* ... */

  return e;
}

void spa_entity_init(entity *e, float x, float y, float x_vel, float y_vel, float theta, float theta_vel) {

  {
    e->x = x;
    e->y = y;
    e->theta = theta;

    e->x_vel = x_vel;
    e->y_vel = y_vel;
    e->theta_vel = theta_vel;

    e->x_accel = 0;
    e->y_accel = 0;
    e->theta_accel = 0;

    e->width = 1;
    e->height = 1;

    e->mass= 1;
  } /* ... */
}

void spa_entity_destroy(void *e) {

  if (e)
    free(e);
}

void spa_entity_attenuate(entity *e) {

  if (!(fabs(e->x_accel) > 0.0) && fabs(e->x_vel) > 0.0) {
    e->x_vel *= 0.96;
  }
  if (!(fabs(e->y_accel) > 0.0) && fabs(e->y_vel) > 0.0) {
    e->y_vel *= 0.96;
  }
  if (!(fabs(e->theta_accel) > 0.0) && fabs(e->theta_vel) > 0.0) {
    e->theta_vel *= 0.96;
  }
}

void spa_entity_update(entity *e) {

  if (e->x_vel + e->x_accel < SPA_ENTITY_MAX_DISP_VEL &&
      e->x_vel + e->x_accel > -SPA_ENTITY_MAX_DISP_VEL)
    e->x_vel += e->x_accel;
  if (e->y_vel + e->y_accel < SPA_ENTITY_MAX_DISP_VEL &&
      e->y_vel + e->y_accel > -SPA_ENTITY_MAX_DISP_VEL)
    e->y_vel += e->y_accel;

  if (e->theta_vel + e->theta_accel < SPA_ENTITY_MAX_THETA_VEL &&
      e->theta_vel + e->theta_accel > -SPA_ENTITY_MAX_THETA_VEL)
    e->theta_vel += e->theta_accel;

  e->theta += e->theta_vel;
  e->x += e->y_vel * cos(e->theta + M_PI_2);
  e->y += e->y_vel * sin(e->theta + M_PI_2);
  e->x += e->x_vel * cos(e->theta);
  e->y += e->x_vel * sin(e->theta);

  if (e->x + e->x_vel < 0)
    e->x = SPA_DISPLAY_WIDTH - e->width;
  else if (e->x + e->width + e->x_vel > SPA_DISPLAY_WIDTH)
    e->x = 0;
}

bool spa_entity_collide(entity *e1, entity *e2) {

  int e1_x2 = e1->x + e1->width / 2;
  int e1_x1 = e1->x - e1->width / 2;
  int e1_y2 = e1->y + e1->height / 2;
  int e1_y1 = e1->y - e1->width / 2;

  int e2_x2 = e2->x + e2->width / 2;
  int e2_x1 = e2->x - e2->width / 2;
  int e2_y2 = e2->y + e2->height / 2;
  int e2_y1 = e2->y - e2->width / 2;

  if ((e1_x1 > e2_x2) ||
      (e1_y1 > e2_y2) ||
      (e2_x1 > e1_x2) ||
      (e2_y1 > e1_y2))
    return false;
  return true;
}

void* spa_remove_entity(entity* e) {
  LIST_REMOVE(e, list);
  void *e2 = e->list.le_next;
  spa_entity_destroy(e);
  return e2;
}

void spa_clear_entity_list(entity_list* lh) {

  if (lh) {
    entity *e = lh->lh_first;
    while (e != NULL) {
      e = spa_remove_entity(e);
    }
  }
}
