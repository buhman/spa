#include <stdio.h>
#define __USE_BSD
#include <math.h>
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
#include "poof.h"
#include "laser.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *font;

entity *player;

entity_list *bullet_list_head;
entity_list *hater_list_head;

poof_list *poof_list_head;

int score;
int level = 0;

int hater_count;
int bullet_count;
int poof_count;

double init_time;

double logic_time;
double event_time;
double render_time;
double osd_time;

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

    init_time = al_get_time();

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

    {
        if (!al_install_mouse()) {
            fprintf(stderr, "al_install_mouse(): failed\n");
            return false;
        }
    } /* ... */

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
    } /* ... */

    {
        if (!al_init_primitives_addon()) {
            fprintf(stderr, "al_init_primitives_addon(): failed\n");
            return false;
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

    {
        poof_list_head = malloc(sizeof(poof_list));
        LIST_INIT(poof_list_head);
    } /* ... */

    {
        al_register_event_source(event_queue, 
                al_get_display_event_source(display));

        al_register_event_source(event_queue,
                al_get_timer_event_source(timer));

        al_register_event_source(event_queue,
                al_get_keyboard_event_source());

        //al_register_event_source(event_queue,
        //		al_get_mouse_event_source());
    } /* ... */

    spa_game_reset();

    fprintf(stdout, "spa_init() in %fs\n", al_get_time() - init_time);

    return true;
}

void spa_render() {

    double t = al_get_time();

    al_clear_to_color(al_map_rgb(10, 10, 20));

    spa_draw_entity(player);

    {
        entity *bullet; 
        for (bullet = bullet_list_head->lh_first; bullet != NULL; 
                bullet = bullet->entity_p.le_next) {

            spa_draw_entity(bullet);
        }
    } /* ... */
    {
        entity *hater;
        for (hater = hater_list_head->lh_first; hater != NULL;
                hater = hater->entity_p.le_next) {

            spa_draw_entity(hater);
        }
    } /* ... */
    {
        poof *poof;
        for (poof = poof_list_head->lh_first; poof != NULL;
                poof = poof->poof_p.le_next) {

            spa_poof_draw(poof);
        }
    } /* ... */

    {
        if (player->type == laser && score > 0)
            spa_laser_draw(player, SCREEN_W, SCREEN_H);
    } /* ... */

    render_time = al_get_time() - t;
}

void spa_osd() {

    double t = al_get_time();	

    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 2, ALLEGRO_ALIGN_LEFT,
            "health: %d", player->health);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 14, ALLEGRO_ALIGN_LEFT,
            "score: %d", score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 26, ALLEGRO_ALIGN_LEFT,
            "level: %d", level);

    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 38, ALLEGRO_ALIGN_LEFT,
            "bullets: %d", bullet_count);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 50, ALLEGRO_ALIGN_LEFT,
            "haters: %d", hater_count);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 62, ALLEGRO_ALIGN_LEFT,
            "poofs: %d", poof_count);


    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W - 2, 2, ALLEGRO_ALIGN_RIGHT,
            "event: %.2fs", event_time);
    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W - 2, 14, ALLEGRO_ALIGN_RIGHT,
            "logic: %.2fs", logic_time);
    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W - 2, 26, ALLEGRO_ALIGN_RIGHT,
            "render: %.2fs", render_time);
    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W - 2, 38, ALLEGRO_ALIGN_RIGHT,
            "osd: %.2fs", osd_time);

    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, SCREEN_H - 14, ALLEGRO_ALIGN_LEFT,
            "p->t: %.2f ; p->t_v: %.2f ; p->t_a: %.2f", 
            player->theta, player->theta_vel, player->theta_accel);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, SCREEN_H - 26, ALLEGRO_ALIGN_LEFT,
            "p->y: %.2f ; p->y_v: %.2f ; p->y_a: %.2f", 
            player->y, player->y_vel, player->y_accel);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 2, SCREEN_H - 38, ALLEGRO_ALIGN_LEFT,
            "p->x: %.2f ; p->x_v: %.2f ; p->x_a: %.2f", 
            player->x, player->x_vel, player->x_accel);

    char* weapon = "null";

    switch (player->type) {
        case rifle:
            weapon = "rifle";
            break;
        case laser:
            weapon = "laser";
            break;
    }

    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 2, ALLEGRO_ALIGN_CENTER,
            "weapon: %s", weapon);

    if (player->health <= 0) {
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,
                ALLEGRO_ALIGN_CENTER, "you died");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 12,
                ALLEGRO_ALIGN_CENTER, "(press [BACKSPACE] to restart)");
    }

    al_flip_display();

    osd_time = al_get_time() - t;
}

void spa_game_reset() {

    score = 10;

    {
        spa_clear_entity_list(hater_list_head);
        spa_clear_entity_list(bullet_list_head);
    } /* ... */

    {
        if (player)
            free(player);

        player = spa_entity_create(SCREEN_W / 2, SCREEN_H - (SCREEN_H / 4), 0, 0, 0);
        spa_player_init_entity(player);
    } /* ... */

    {
        spa_create_haters(hater_list_head, SCREEN_W, SCREEN_H, 10 + level);
    } /* ... */

    al_start_timer(timer);
}

bool spa_loop(bool *redraw) {

    double t = al_get_time();

    ALLEGRO_EVENT ev;
    ALLEGRO_KEYBOARD_STATE ks;

    al_wait_for_event(event_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) {
        *redraw = true;
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return true;

    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                player->y_accel = -0.1;
                break;
            case ALLEGRO_KEY_S:
                player->y_accel = 0.1;
                break;
            case ALLEGRO_KEY_Q:
                player->x_accel = -0.1;
                break;
            case ALLEGRO_KEY_E:
                player->x_accel = 0.1;
                break;
            case ALLEGRO_KEY_A:
                player->theta_accel = -0.0078;
                break;
            case ALLEGRO_KEY_D:
                player->theta_accel = 0.0078;
                break;
            case ALLEGRO_KEY_SPACE:
                if (score > 0) {
                    al_get_keyboard_state(&ks);
                    if (al_key_down(&ks, ALLEGRO_KEY_LCTRL))
                        player->type = laser;
                    else {
                        player->type = rifle;
                        spa_add_bullet(bullet_list_head, player);
                        score -= 2;
                    }
                }
                break;
            case ALLEGRO_KEY_BACKSPACE:
                spa_game_reset();
                break;
        }
    }

    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                if (player->y_accel < 0)
                    player->y_accel = 0;
                break;
            case ALLEGRO_KEY_S:
                if (player->y_accel > 0)
                    player->y_accel = 0;
                break;
            case ALLEGRO_KEY_Q:
                if (player->x_accel < 0)
                    player->x_accel = 0;
                break;
            case ALLEGRO_KEY_E:
                if (player->x_accel > 0)
                    player->x_accel = 0;
                break;
            case ALLEGRO_KEY_A:
                if (player->theta_accel < 0)
                    player->theta_accel = 0;
                break;
            case ALLEGRO_KEY_D:
                if (player->theta_accel > 0)
                    player->theta_accel = 0;
                break;
            case ALLEGRO_KEY_SPACE:
                if (player->type == laser) {
                    player->type = rifle;
                }
        }
    }

    event_time = al_get_time() - t;

    return false;
}

void spa_logic_update() {

    double t = al_get_time();

    entity *bullet;
    entity *hater;
    poof *poof;

    bullet_count = 0;
    bullet = bullet_list_head->lh_first;
    while (bullet != NULL) {

        {
            if (bullet->y + bullet->height < 0 || 
                    bullet->y + bullet->height > SCREEN_H) {
                bullet = spa_remove_entity(bullet);
                goto bullet_loop_end;
            }
        } /* ... */

        {
            if (spa_entity_collide(bullet, player)) {

                spa_player_damage(player, 5, timer);

                spa_poof_add(poof_list_head, bullet->x, bullet->y, al_map_rgb(255, 0, 0));
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
                    spa_poof_add(poof_list_head, bullet->x, bullet->y, al_map_rgb(255, 0, 0));
                    bullet = spa_remove_entity(bullet);

                    if (hater->health <= 0) {
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
        bullet_count++;

bullet_loop_end:
        continue;
    }

    if (player->type == laser && score > 0) {
        score -= 1;
        hater = hater_list_head->lh_first;
        while (hater != NULL) {
            long cx, cy;
            if (spa_laser_collide(hater, player, SCREEN_W, SCREEN_H, &cx, &cy)) {

                spa_poof_add(poof_list_head, cx, cy, al_map_rgb(0, 255, 0));
                hater->health -= 0.1;
                if (hater->health <= 0) {
                    hater = spa_remove_entity(hater);
                    score += 5;
                    continue;
                }
            }

            hater = hater->entity_p.le_next;
        }
    }

    hater_count = 0;
    hater = hater_list_head->lh_first;
    while (hater != NULL) {
        if (spa_entity_collide(player, hater)) {
            score += 5;
            spa_player_damage(player, 10, timer);
            hater = spa_remove_entity(hater);
            continue;
        }

        spa_hater_update(hater, player, bullet_list_head, level);
        spa_entity_update(hater, SCREEN_W);
        hater = hater->entity_p.le_next;
        hater_count++;
    }

    poof_count = 0;
    poof = poof_list_head->lh_first;
    while (poof != NULL) {
        if (poof->iteration > 25) {
            poof = spa_poof_remove(poof);
            continue;
        }
        poof = poof->poof_p.le_next;
        poof_count++;
    }

    if (hater_list_head->lh_first == NULL) {
        level++;
        spa_clear_entity_list(bullet_list_head);
        spa_create_haters(hater_list_head, SCREEN_W, SCREEN_H, 10 + level);
    }

    spa_entity_attenuate(player);
    spa_entity_update(player, SCREEN_W);

    logic_time = al_get_time() - t;
}

int main(int argc, char **argv) {

    bool redraw = true;

    {
        if (!spa_init())
            goto cleanup;
    } /* ... */

    {
        while(!spa_loop(&redraw)) {

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;
                spa_logic_update();
                spa_render();
                spa_osd();
            }
        }
    } /* ... */

cleanup:
    {
        spa_clear_entity_list(hater_list_head);
        spa_clear_entity_list(bullet_list_head);

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
