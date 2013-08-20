#pragma once

#include "entity.h"

void spa_laser_draw(entity *player);
bool spa_laser_collide(entity* e, entity* player, long* x, long* y);
