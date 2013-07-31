#include <stdio.h>
#include <allegro5/allegro.h>

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
        e->bitmap = NULL;
    } /* ... */

    return e;
}

void spa_entity_init(entity *e, ALLEGRO_BITMAP *b) {
    {
        e->bitmap = b;
        e->width = al_get_bitmap_width(b);
        e->height = al_get_bitmap_height(b);
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
