#include <stdio.h>
#include <sys/queue.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "spa.h"
#include "entity.h"
#include "player.h"
#include "bullet.h"
#include "hater.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *font;

entity *player;
entity_list *bullet_list_head;
entity_list *hater_list_head;

int score;

void print_version(char* text, int v) {

    fprintf(stdout, "%s version: %d.%d.%dr%d\n", text,
            v >> 24, (v >> 16) & 255, (v >> 8) & 255, v & 255);
}

bool spa_init() {
    {
        if (!al_init()) {
            fprintf(stderr, "al_init(): failed\n");
            return false;
        }
        else {
            print_version("allegro", al_get_allegro_version());
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
    } /* ... */

    {
        if (!spa_bullet_init(display)) {
            fprintf(stderr, "spa_bullet_init(): failed\n");
            return false;
        }
    } /* ... */

    {
        if (!spa_hater_init(display)) {
            fprintf(stderr, "spa_hater_init(): failed\n");
            return false;
        }
    } /* ... */

    {
        al_init_font_addon();
        al_init_ttf_addon();
        font = al_load_font("DejaVuSansMono.ttf", 12, 0);
        if (!font) {
            fprintf(stderr, "al_load_font(): failed\n");
            return false;
        }
        else {
            /*
            print_version("allegro_font", al_get_allegro_font_version());
            print_version("allegro_ttf", al_get_allegro_ttf_version());
            */
        }
    } /* ... */

    {
        if (!al_init_primitives_addon()) {
            fprintf(stderr, "al_init_primitives_addon(): failed\n");
            return false;
        }
        else {
            /*
            print_version("allegro_primitives", 
                    al_get_allegro_primitives_version());
            */
        }
    } /* ... */

    {
        bullet_list_head = malloc(sizeof(entity_list));
        LIST_INIT(bullet_list_head);
    } /* ... */

    {
        hater_list_head = malloc(sizeof(entity_list));
        LIST_INIT(hater_list_head);
    } /* ... */

    return true;
}

void spa_render() {

    al_clear_to_color(al_map_rgb(10, 10, 20));

    al_draw_bitmap(player->bitmap, player->x, player->y, 0);

    {
        entity *bullet; 
        for (bullet = bullet_list_head->lh_first; bullet != NULL; 
                bullet = bullet->entity_p.le_next) {

            al_draw_bitmap(bullet->bitmap, bullet->x, bullet->y, 0);
        }
    }
    {
        entity *hater;
        for (hater = hater_list_head->lh_first; hater != NULL;
                hater = hater->entity_p.le_next) {

            al_draw_bitmap(hater->bitmap, hater->x, hater->y, 0);
        }
    }
}

void spa_osd() {

    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 2, ALLEGRO_ALIGN_LEFT,
            "health: %d", player->health);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 14, ALLEGRO_ALIGN_LEFT,
            "score: %d", score);

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
                spa_add_bullet(bullet_list_head, player->x + (player->width / 2),
                        player->y, player->x_vel, player->y_vel);
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
    score = 0;

    {
        if (!spa_init())
            goto cleanup;
    } /* ... */

    {
        player = spa_entity_create(SCREEN_W / 2, SCREEN_H - (SCREEN_H / 4), 0, 0);
        if (!player) {
            fprintf(stderr, "spa_entity_create(): failed\n");
            goto cleanup;
        }

        spa_player_init_entity(player);
    } /* ... */

    {
        spa_create_haters(hater_list_head, SCREEN_W, SCREEN_H, 10);
    }

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
        entity *bullet;
        entity *hater;

        while(!spa_loop(&redraw)) {
            
            bullet = bullet_list_head->lh_first;
            while (bullet != NULL) {
                
                {
                    if (bullet->y + bullet->height < 0) {
                        bullet = spa_remove_entity(bullet);
                        goto bullet_loop_end;
                    }
                } /* ... */
                
                {
                    if (spa_entity_collide(bullet, player)) {
                        player->health -= 5;
                        bullet = spa_remove_entity(bullet);
                        goto bullet_loop_end;
                    }
                } /* ... */

                {
                    hater = hater_list_head->lh_first;
                    while (hater != NULL) {
                        if (spa_entity_collide(bullet, hater)) {

                            score += 10;
                            hater->health -= 5;
                            bullet = spa_remove_entity(bullet);

                            if (hater->health < 0) {
                                // this isn't used anyway, since we go 
                                // immediately to the next bullet
                                hater = spa_remove_entity(hater);
                            }
                            
                            goto bullet_loop_end;
                        }

                        hater = hater->entity_p.le_next;
                    }
                } /* ... */

                spa_entity_update(bullet, SCREEN_W);
                bullet = bullet->entity_p.le_next;

            bullet_loop_end:
                continue;
            }

            hater = hater_list_head->lh_first;
            while (hater != NULL) {
                if (spa_entity_collide(player, hater)) {
                    score += 5;
                    player->health -= 10;
                    hater = spa_remove_entity(hater);
                    continue;
                }

                spa_entity_update(hater, SCREEN_W);
                hater = hater->entity_p.le_next;
            }

            spa_entity_update(player, SCREEN_W);

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;
                spa_render();
                spa_osd();
            }
        }
    } /* ... */

cleanup:
    {
        spa_player_destroy();
        spa_bullet_destroy();
        spa_hater_destroy();

        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_primitives_addon();

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
