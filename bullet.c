#include <stdio.h>
#include <allegro5/allegro.h>

#include "entity.h"
#include "bullet.h"

ALLEGRO_BITMAP *bullet_bitmap = NULL;

const int BULLET_WIDTH = 4;
const int BULLET_HEIGHT = 6;

bool spa_bullet_init(ALLEGRO_DISPLAY *display) {

    {
        bullet_bitmap = al_create_bitmap(BULLET_WIDTH, BULLET_HEIGHT);
        if (!bullet_bitmap) {
            fprintf(stderr, "al_create_bitmap(): failed\n");
            return false;
        }
    } /* ... */

    {
        al_set_target_bitmap(bullet_bitmap);
        al_clear_to_color(al_map_rgb(255, 255, 0));
        al_set_target_bitmap(al_get_backbuffer(display));
    } /* ... */

    return true;
}

void spa_bullet_init_entity(entity* e) {

    spa_entity_init(e, bullet_bitmap);
}

void spa_bullet_destroy() {
    if (bullet_bitmap)
        al_destroy_bitmap(bullet_bitmap);
}
