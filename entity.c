#include <stdio.h>
#define __USE_BSD
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"

const int TERMINAL_VELOCITY = 2;
const float TERMINAL_THETA_VEL = M_PI_4 / 10;

entity* spa_entity_create(float x, float y, float x_vel, float y_vel, float theta) {

    entity *e = NULL;

    {
        e = malloc(sizeof(entity));
        if (!e) {
            fprintf(stderr, "malloc(): failed\n");
            return NULL;
        }
    } /* ... */

    {
        e->x = x;
        e->y = y;
        e->theta = theta;

        e->x_vel = x_vel;
        e->y_vel = y_vel;
		e->theta_vel = 0;

		e->x_accel = 0;
		e->y_accel = 0;
		e->theta_accel = 0;

        e->width = 0;
        e->height = 0;

        e->health = 1;
		e->mass= 1;

        e->type = 0;
		e->last_update = 0;

        e->bitmap = NULL;
    } /* ... */

    return e;
}

void spa_entity_init(entity *e, ALLEGRO_BITMAP *b) {
    {
        e->bitmap = b;
        e->width = al_get_bitmap_width(b);
        e->height = al_get_bitmap_height(b);

        e->x -= e->width / 2;
        e->y -= e->height / 2;
    } /* ... */
}

void spa_entity_destroy(entity *e) {

    if (e)
        free(e);
}

void spa_entity_attenuate(entity *e) {

	if (e->x_accel == 0 && e->x_vel != 0) {
		e->x_vel *= 0.96;
	}
	if (e->y_accel == 0 && e->y_vel != 0) {
		e->y_vel *= 0.96;
	}
	if (e->theta_accel == 0 && e->theta_vel != 0) {
		e->theta_vel *= 0.96;
	}
}

void spa_entity_update(entity *e, int screen_width) {	

    if (e->x_vel + e->x_accel < TERMINAL_VELOCITY && 
            e->x_vel + e->x_accel > -TERMINAL_VELOCITY)
        e->x_vel += e->x_accel;
    if (e->y_vel + e->y_accel < TERMINAL_VELOCITY && 
            e->y_vel + e->y_accel > -TERMINAL_VELOCITY)
        e->y_vel += e->y_accel;

    if (e->theta_vel + e->theta_accel < TERMINAL_THETA_VEL && 
            e->theta_vel + e->theta_accel > -TERMINAL_THETA_VEL)
        e->theta_vel += e->theta_accel;

    e->theta += e->theta_vel;
    e->x += e->y_vel * cos(e->theta + M_PI_2);
    e->y += e->y_vel * sin(e->theta + M_PI_2);
    e->x += e->x_vel * cos(e->theta);
    e->y += e->x_vel * sin(e->theta);

    if (e->x + e->x_vel < 0)
        e->x = screen_width - e->width;
    else if (e->x + e->width + e->x_vel > screen_width)
        e->x = 0;
}

bool spa_entity_collide(entity *e1, entity *e2) {
    
    int e1_x2 = e1->x + e1->width / 2;
    int e1_x1 = e1->x - e1->width / 2;
    int e1_y2 = e1->y + e1->height / 2;
    int e1_y1 = e1->y - e1->width / 2;

    int e2_x2 = e2->x + e2->width / 2;
    int e2_x1 = e2->x - e2->width / 2;
    int e2_y2 = e2->y + e2->height / 2;
    int e2_y1 = e2->y - e2->width / 2;

    if ((e1_x1 > e2_x2) ||
            (e1_y1 > e2_y2) ||
            (e2_x1 > e1_x2) ||
            (e2_y1 > e1_y2))
        return false;
    return true;
}

void spa_entity_bounce(entity *e1, entity *e2) {

	float r_1 = e1->mass / e2->mass;
	float r_2 = 1 / r_1;

	e1->x_vel = (e1->x_vel * r_1 - e2->x_vel * r_2) / 2;
	e1->y_vel = (e1->y_vel * r_1 - e2->y_vel * r_2) / 2;
	e2->x_vel = (e2->x_vel * r_1 - e1->x_vel * r_2) / 2;
	e2->y_vel = (e2->y_vel * r_1 - e1->y_vel * r_2) / 2;
}

void spa_draw_entity(entity *e) {

    al_draw_rotated_bitmap(e->bitmap,
            e->width / 2,
            e->height / 2,
            e->x, e->y,
            e->theta, 0);

    int e_x2 = fmax(e->x, e->x + e->x_vel) + e->width / 2;
    int e_x1 = fmin(e->x, e->x + e->x_vel) - e->width / 2;
    int e_y2 = fmax(e->y, e->y + e->y_vel) + e->height / 2;
    int e_y1 = fmin(e->y, e->y + e->y_vel) - e->width / 2;

    al_draw_rectangle(e_x1, e_y1, e_x2, e_y2,
            al_map_rgb(50, 50, 50), 1);
}

entity* spa_remove_entity(entity* e) {
    LIST_REMOVE(e, entity_p);
    entity *e2 = e->entity_p.le_next;
    spa_entity_destroy(e);
    return e2;
}

void spa_clear_entity_list(entity_list* lh) {

    entity *e = lh->lh_first;
    while (e != NULL) {
        e = spa_remove_entity(e);
    }
}
