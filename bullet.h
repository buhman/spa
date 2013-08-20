#pragma once

#include <allegro5/allegro.h>

#include "entity.h"

typedef enum bullet_type bullet_type;

enum bullet_type {
  pew /* pew pew ! */
};

typedef struct bullet bullet;

struct bullet {
  entity e;
  bullet_type type;
};

bullet* spa_bullet_create(float x, float y, float x_vel, float y_vel, float theta, float theta_vel);
void spa_add_bullet(entity_list*, entity*);
