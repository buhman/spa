#pragma once

#include <allegro5/allegro.h>
#include <sys/queue.h>

typedef struct entity entity;
typedef struct entity_list entity_list;

struct entity {
    float x;
    float y;
    float x_vel;
    float y_vel;
    int width;
    int height;
    int health;
    int type;
    float angle;
    ALLEGRO_BITMAP *bitmap;
	LIST_ENTRY(entity) entity_p;
};

LIST_HEAD(entity_list, entity);

entity* spa_entity_create(int, int, int, int);

void spa_entity_init(entity*, ALLEGRO_BITMAP*);

void spa_entity_destroy(entity*);

void spa_entity_update(entity*, int);

bool spa_entity_collide(entity*, entity*);

void spa_draw_entity(entity*);

entity* spa_remove_entity(entity*);
