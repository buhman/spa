#include <stdio.h>
#include <allegro5/allegro.h>

#include "spa.h"
#include "entity.h"
#include "player.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

entity *player = NULL;

bool spa_init() {
    {
        if (!al_init()) {
            fprintf(stderr, "al_init(): failed\n");
            return false;
        }
    } /* ... */

    {
        timer = al_create_timer(1.0 / FPS);
        if (!timer) {
            fprintf(stderr, "al_create_timer(): failed\n");
            return false;
        }
    } /* ... */

    {
        display = al_create_display(SCREEN_W, SCREEN_H);
        if (!display) {
            fprintf(stderr, "al_create_display(): failed\n");
            return false;
        }
    } /* ... */

    {
        event_queue = al_create_event_queue();
        if (!event_queue) {
            fprintf(stderr, "al_create_event_queue(): failed\n");
            return false;
        }
    } /* ... */

    /*
    {
        if (!al_install_mouse()) {
            fprintf(stderr, "al_install_mouse(): failed\n");
            return false;
        }
    } */ /* ... */

    {
        if (!al_install_keyboard()) {
            fprintf(stderr, "al_install_keyboard(): failed\n");
            return false;
        }
    } /* ... */

    {
        if (!spa_player_init(display)) {
            fprintf(stderr, "spa_player_init(): failed\n");
            return false;
        }
    }

    return true;
}

void spa_render() {

    al_clear_to_color(al_map_rgb(10, 10, 20));

    al_draw_bitmap(player->bitmap, player->x, player->y, 0);

    al_flip_display();
}

bool spa_loop(bool *redraw) {

    ALLEGRO_EVENT ev;

    al_wait_for_event(event_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) {
        *redraw = true;
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return true;

    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                player->y_vel = -PLAYER_VEL;
                break;
            case ALLEGRO_KEY_S:
                player->y_vel = PLAYER_VEL;
                break;
            case ALLEGRO_KEY_A:
                player->x_vel = -PLAYER_VEL;
                break;
            case ALLEGRO_KEY_D:
                player->x_vel = PLAYER_VEL;
                break;
            case ALLEGRO_KEY_SPACE:
                fprintf(stdout, "pew!\n");
                break;
        }
    }

    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                if (player->y_vel < 0)
                    player->y_vel = 0;
                break;
            case ALLEGRO_KEY_S:
                if (player->y_vel > 0)
                    player->y_vel = 0;
                break;
            case ALLEGRO_KEY_A:
                if (player->x_vel < 0)
                    player->x_vel = 0;
                break;
            case ALLEGRO_KEY_D:
                if (player->x_vel > 0)
                    player->x_vel = 0;
                break;
        }
    }

    return false;
}

int main(int argc, char **argv) {

    bool redraw = true;

    {
        if (!spa_init())
            goto cleanup;
    } /* ... */

    {
        player = spa_entity_create();
        if (!player) {
            fprintf(stderr, "spa_entity_create(): failed\n");
            goto cleanup;
        }

        spa_player_init_entity(player);
    } /* ... */

    {
        al_register_event_source(event_queue, 
                al_get_display_event_source(display));

        al_register_event_source(event_queue,
                al_get_timer_event_source(timer));

        al_register_event_source(event_queue,
                al_get_keyboard_event_source());

        /*
        al_register_event_source(event_queue,
                al_get_mouse_event_source());
        */

        al_start_timer(timer);
    } /* ... */

    {
        while(!spa_loop(&redraw)) {

            if (player->x + player->x_vel < 0)
                player->x = SCREEN_W - player->width;
            else if (player->x + player->width + player->x_vel > SCREEN_W)
                player->x = 0;

            spa_entity_update(player);

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;
                spa_render();
            }
        }
    } /* ... */

cleanup:
    {
        if (player)
            spa_entity_destroy(player);
        if (event_queue)
            al_destroy_event_queue(event_queue);
        if (display)
            al_destroy_display(display);
        if (timer)
            al_destroy_timer(timer);
    }

    return 0;
}
