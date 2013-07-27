#pragma once

#include <allegro5/allegro.h>

#include "entity.h"

bool spa_bullet_init(ALLEGRO_DISPLAY*);
void spa_bullet_init_entity(entity*);
void spa_bullet_destroy();
