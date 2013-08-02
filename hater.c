#include <stdio.h>
#define __USE_BSD
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"
#include "hater.h"

ALLEGRO_BITMAP *hater_bitmap = NULL;

const int HATER_WIDTH = 20;
const int HATER_HEIGHT = 20;
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
}

void spa_hater_destroy() {
    if (hater_bitmap)
        al_destroy_bitmap(hater_bitmap);
}

void spa_hater_update(entity* hater, entity* player) {
    
    float o = (hater->y - player->y);
    float a = (hater->x - player->x);

    float theta = atan2(o, a) - M_PI_2 - hater->theta;

    //hater->theta = theta - M_PI_2; //FIXME

    hater->theta_vel += theta / 1000.f;
}

void spa_create_haters(entity_list* lh, int screen_width, 
        int screen_height, int number) {
 
    for (int i = 0; i < number; i++) {
    
        entity *hater = spa_entity_create(rand() % screen_width, 
                rand() % screen_height / 2 + HATER_WIDTH, 0, -(rand() % 2), M_PI);
        spa_hater_init_entity(hater);

        LIST_INSERT_HEAD(lh, hater, entity_p);
    }
}
