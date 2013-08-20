#pragma once

#include <allegro5/allegro.h>

#include "entity.h"

typedef struct player player;

struct player {
  entity e;
  int health;
  int sheilds;
  weapon weapon;
};

bool spa_player_init(ALLEGRO_DISPLAY*);
void spa_player_shutdown();
void spa_player_damage(player*, int);
