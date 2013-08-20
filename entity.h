#pragma once

#include <allegro5/allegro.h>
#include <sys/queue.h>

typedef struct entity entity;

struct entity {
  float x;
  float y;
  float x_vel;
  float y_vel;
  float x_accel;
  float y_accel;
  float theta;
  float theta_vel;
  float theta_accel;
  float mass;
  int width;
  int height;
  LIST_ENTRY(entity) list;
  void (*draw)(entity*);
};

typedef struct entity_list entity_list;

LIST_HEAD(entity_list, entity) head;

			       /* this weapon declaration might belong
				  in player.h instead depending on how
			       useful it ends up being for other types */
			       
typedef enum weapon weapon;

enum weapon {
  rifle,
  laser
};

entity* spa_entity_create(float, float, float, float, float, float);

void spa_entity_init(entity *e, float x, float y, float x_vel, float y_vel, float theta, float theta_vel);

void spa_entity_destroy(void*);

void spa_entity_update(entity*);

bool spa_entity_collide(entity*, entity*);

void* spa_remove_entity(entity*);

void spa_clear_entity_list(entity_list*);

void spa_entity_attenuate(entity*);
