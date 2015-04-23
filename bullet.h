#pragma once

#include <allegro5/allegro.h>

#include "entity.h"

bool spa_bullet_init(ALLEGRO_DISPLAY*);
void spa_bullet_init_entity(entity*);
void spa_bullet_destroy(void);
void spa_add_bullet(entity_list*, entity*);
entity* spa_remove_bullet(entity*);
