#include <stdio.h>
#define __USE_BSD
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "entity.h"
#include "hater.h"
#include "bullet.h"

ALLEGRO_BITMAP *hater_bitmap = NULL;

const int HATER_WIDTH = 20 * SCALE;
const int HATER_HEIGHT = 20 * SCALE;
const float HATER_THETA = M_PI_4 / 500000;

bool spa_hater_init(ALLEGRO_DISPLAY *display) {

    {
        hater_bitmap = al_create_bitmap(HATER_WIDTH, HATER_HEIGHT);
        if (!hater_bitmap) {
            fprintf(stderr, "al_create_bitmap(): failed\n");
            return false;
        }
    } /* ... */

    {
        int width = HATER_WIDTH - 1;
        int height = HATER_HEIGHT - 1;

        al_set_target_bitmap(hater_bitmap);

        al_clear_to_color(al_map_rgba(0, 0, 0, 0));

        al_draw_circle(
                width / 2,
                height / 2 + height / 6,
                width / 2.8,
                al_map_rgb(0, 255, 255), 1);

        al_draw_circle(
                width / 8,
                height / 8,
                width / 6,
                al_map_rgb(0, 255, 255), 1);

        al_draw_circle(
                width - width / 8,
                height / 8,
                width / 6,
                al_map_rgb(0, 255, 255), 1);

        al_set_target_bitmap(al_get_backbuffer(display));
    } /* ... */

    return true;
}

void spa_hater_init_entity(entity* hater) {

    spa_entity_init(hater, hater_bitmap);

    hater->type = rand();
    hater->mass = 3;
}

void spa_hater_destroy(void) {
    if (hater_bitmap)
        al_destroy_bitmap(hater_bitmap);
}

void spa_hater_update(entity* hater, entity* player, entity_list *lh, int level) {

    {
        float o = (hater->y - player->y);
        float a = (hater->x - player->x);

        float theta = atan2(o, a);

        hater->theta = theta - M_PI_2;
    } /* ... */

    {
        int r = rand() % 255;
        if (r > 255 - (level * 1.1)) {

            spa_add_bullet(lh, hater);
        }

    } /* ... */
}

void spa_create_haters(entity_list* lh, int screen_width,
        int screen_height, int number) {

    for (int i = 0; i < number; i++) {

        entity *hater = spa_entity_create(rand() % screen_width + HATER_WIDTH / 2,
                rand() % screen_height + HATER_HEIGHT / 2, 0, 0, 0);
        spa_hater_init_entity(hater);

        LIST_INSERT_HEAD(lh, hater, entity_p);
    }
}
