#pragma once

#include <allegro5/allegro.h>
#include <sys/queue.h>

typedef struct entity entity;

struct entity {
    int x;
    int y;
    int x_vel;
    int y_vel;
    int width;
    int height;
    ALLEGRO_BITMAP *bitmap;
	LIST_ENTRY(foo) entity_p;
};

//LIST_HEAD(entity_list, entity) entity_list_head;

entity* spa_entity_create(int, int, int, int);

void spa_entity_init(entity*, ALLEGRO_BITMAP*);

void spa_entity_destroy(entity*);

void spa_entity_update(entity*, int);
