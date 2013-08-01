#include <stdio.h>
#include <allegro5/allegro.h>

#include "entity.h"
#include "hater.h"

ALLEGRO_BITMAP *hater_bitmap = NULL;

const int HATER_WIDTH = 20;
const int HATER_HEIGHT = 20;

bool spa_hater_init(ALLEGRO_DISPLAY *display) {

    {
        hater_bitmap = al_create_bitmap(HATER_WIDTH, HATER_HEIGHT);
        if (!hater_bitmap) {
            fprintf(stderr, "al_create_bitmap(): failed\n");
            return false;
        }
    } /* ... */

    {
        al_set_target_bitmap(hater_bitmap);
        al_clear_to_color(al_map_rgb(0, 255, 255));
        al_set_target_bitmap(al_get_backbuffer(display));
    } /* ... */

    return true;
}

void spa_hater_init_entity(entity* hater) {

    spa_entity_init(hater, hater_bitmap);

    hater->x -= (hater->width / 2);
    hater->y -= hater->height;
    hater->type = rand();
}

void spa_hater_destroy() {
    if (hater_bitmap)
        al_destroy_bitmap(hater_bitmap);
}

void spa_create_haters(entity_list* lh, int screen_width, 
        int screen_height, int number) {
 
    for (int i = 0; i < number; i++) {
    
        entity *hater = spa_entity_create(rand() % screen_width, 
                rand() % screen_height / 2 + HATER_WIDTH, rand() % 3 - 1, 0);
        spa_hater_init_entity(hater);

        LIST_INSERT_HEAD(lh, hater, entity_p);
    }
}
