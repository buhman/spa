#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "entity.h"
#include "player.h"

ALLEGRO_BITMAP *player_bitmap = NULL;

const int PLAYER_WIDTH = 32;
const int PLAYER_HEIGHT = 32;

bool spa_player_init(ALLEGRO_DISPLAY *display) {

    { 
        player_bitmap = al_create_bitmap(PLAYER_WIDTH, PLAYER_HEIGHT); 
        if (!player_bitmap) { 
            fprintf(stderr, "al_create_bitmap(): failed\n"); 
            return false; 
        } 
    } /* ... */ 

    {
        int width = PLAYER_WIDTH - 1;
        int height = PLAYER_HEIGHT - 1;

        al_set_target_bitmap(player_bitmap);

        al_clear_to_color(al_map_rgba(0, 0, 0, 0));

        al_draw_triangle(0, height, width, height, width / 2, 0,
            al_map_rgb(255, 0, 255), 1);

        al_draw_triangle(0, height, 1, 1, width / 3, height, al_map_rgb(255, 0, 255), 1);
        al_draw_triangle(width, height, width, 1, width - (width / 3), height , al_map_rgb(255, 0, 255), 1);

        al_set_target_bitmap(al_get_backbuffer(display));
    } /* ... */

    return true;
}

void spa_player_init_entity(entity *e) {
    
        spa_entity_init(e, player_bitmap);

        e->mass = 5;
    e->health = 100;
}

void spa_player_destroy() {
    if (player_bitmap)
        al_destroy_bitmap(player_bitmap);
}

void spa_player_damage(entity* player, int damage, ALLEGRO_TIMER *timer) {

    if (player->health - damage <= 0) {
        player->health = 0;
        al_stop_timer(timer);
    }
    else
        player->health -= damage;
}
