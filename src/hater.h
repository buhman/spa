#include <allegro5/allegro.h>

#include "entity.h"
#include "player.h"

typedef struct hater hater;

struct hater {
  entity e;
  int health;
};

bool spa_hater_init(ALLEGRO_DISPLAY*);
void spa_hater_shutdown();

void spa_hater_add(entity_list*, int);
void spa_hater_update(hater*, player*, entity_list*, int);
