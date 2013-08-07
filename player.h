#pragma once

#include <allegro5/allegro.h>

#include "entity.h"

typedef enum WEAPON WEAPON;

enum WEAPON {
	rifle,
	laser
};

bool spa_player_init(ALLEGRO_DISPLAY*);
void spa_player_init_entity(entity*);
void spa_player_destroy();
void spa_player_damage(entity*, int, ALLEGRO_TIMER*);
