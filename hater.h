#include <allegro5/allegro.h>

#include "entity.h"

bool spa_hater_init(ALLEGRO_DISPLAY*);
void spa_hater_destroy();
void spa_create_haters(entity_list*, int, int, int);
void spa_hater_update(entity*, entity*, entity_list*, int);
void spa_hater_dead(entity*);
