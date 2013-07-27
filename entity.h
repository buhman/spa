#pragma once

#include <allegro5/allegro.h>

#define GFOREACH(item, list) for(GList *__glist = list; __glist && (item = __glist->data, true); __glist = __glist->next)

typedef struct entity entity;

struct entity {
    int x;
    int y;
    int x_vel;
    int y_vel;
    int width;
    int height;
    ALLEGRO_BITMAP *bitmap;
};

entity* spa_entity_create(int, int, int, int);

void spa_entity_init(entity*, ALLEGRO_BITMAP*);

void spa_entity_destroy(entity*);

void spa_entity_update(entity*);
