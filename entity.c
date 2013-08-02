#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"

entity* spa_entity_create(int x, int y, int x_vel, int y_vel) {

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
        e->x_vel = x_vel;
        e->y_vel = y_vel;
        e->width = 0;
        e->height = 0;
        e->health = 1;
        e->type = 0;
        e->angle = 0.7853; //TODO
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

void spa_entity_update(entity *e, int screen_width) {

    e->x += e->x_vel;
    e->y += e->y_vel;

    if (e->x + e->x_vel < 0)
        e->x = screen_width - e->width;
    else if (e->x + e->width + e->x_vel > screen_width)
        e->x = 0;
}

bool spa_entity_collide(entity *e1, entity *e2) {
    
    int e1_x2 = fmax(e1->x, e1->x + e1->x_vel) + e1->width / 2;
    int e1_x1 = fmin(e1->x, e1->x + e1->x_vel) - e1->width / 2;
    int e1_y2 = fmax(e1->y, e1->y + e1->y_vel) + e1->height / 2;
    int e1_y1 = fmin(e1->y, e1->y + e1->y_vel) - e1->width / 2;

    int e2_x2 = fmax(e2->x, e2->x + e2->x_vel) + e2->width / 2;
    int e2_x1 = fmin(e2->x, e2->x + e2->x_vel) - e2->width / 2;
    int e2_y2 = fmax(e2->y, e2->y + e2->y_vel) + e2->height / 2;
    int e2_y1 = fmin(e2->y, e2->y + e2->y_vel) - e2->width / 2;

    if ((e1_x1 > e2_x2) ||
            (e1_y1 > e2_y2) ||
            (e2_x1 > e1_x2) ||
            (e2_y1 > e1_y2))
        return false;
    return true;
}

void spa_draw_entity(entity *e) {

    al_draw_rotated_bitmap(e->bitmap,
            e->width / 2,
            e->height / 2,
            e->x, e->y,
            e->angle, 0);

    al_draw_rectangle(e->x - (e->width / 2), 
            e->y - (e->height / 2), 
            e->x + (e->width / 2),
            e->y + (e->height / 2),
            al_map_rgb(50, 50, 50), 1);
}

entity* spa_remove_entity(entity* e) {
    LIST_REMOVE(e, entity_p);
    entity *e2 = e->entity_p.le_next;
    spa_entity_destroy(e);
    return e2;
}
