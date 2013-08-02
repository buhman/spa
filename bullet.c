#include <stdio.h>
#include <sys/queue.h>
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

void spa_bullet_init_entity(entity* bullet) {

    spa_entity_init(bullet, bullet_bitmap);

    bullet->y -= bullet->height / 2;
    bullet->y_vel -= 1;
}

void spa_bullet_destroy() {
    if (bullet_bitmap)
        al_destroy_bitmap(bullet_bitmap);
}

void spa_add_bullet(entity_list *lh, entity *e) {
    
    int x = e->x;
    int y = e->y;

    entity *bullet = spa_entity_create(x, y, e->x_vel, e->y_vel);
    spa_bullet_init_entity(bullet);
    bullet->angle = e->angle;

    LIST_INSERT_HEAD(lh, bullet, entity_p);
}
