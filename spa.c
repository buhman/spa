#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>

const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BITMAP_SIZE = 32;
const float FPS = 60;

int main(int argc, char **argv) {

    int status = 0;
    
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *bitmap = NULL;
    
    bool redraw = true;

    int iter = 0;

    {
        if (!al_init()) {
            fprintf(stderr, "al_init(): failed\n");
            status = -1;
            goto exit;
        }
    } /* ... */

    {
        timer = al_create_timer(1.0 / FPS);
        if (!timer) {
            fprintf(stderr, "al_create_timer(): failed\n");
            status = -1;
            goto exit;
        }
    } /* ... */

    {
        display = al_create_display(640, 480);
        if (!display) {
            fprintf(stderr, "al_create_display(): failed\n");
            status = -1;
            goto cleanupTimer;
        }
    } /* ... */

    {
        bitmap = al_create_bitmap(BITMAP_SIZE, BITMAP_SIZE);
        if (!bitmap) {
            fprintf(stderr, "al_create_bitmap(): failed\n");
            status = -1;
            goto cleanupDisplay;
        }
    } /* ... */

    {
        event_queue = al_create_event_queue();
        if (!event_queue) {
            fprintf(stderr, "al_create_event_queue(): failed\n");
            status = -1;
            goto cleanupBitmap;
        }
    } /* ... */

/*    {
        al_set_target_bitmap(bitmap);
        al_clear_to_color(al_map_rgb(255, 0, 255));
        al_set_target_bitmap(al_get_backbuffer(display));
    } */ /* ... */

    {
        al_register_event_source(event_queue, 
                al_get_display_event_source(display));

        al_register_event_source(event_queue,
                al_get_timer_event_source(timer));

        al_start_timer(timer);
    } /* ... */

    {
        while(1) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_TIMER) {
                redraw = true;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;
                
                al_clear_to_color(al_map_rgb(10, 10, 20));
 
                al_set_target_bitmap(bitmap);

                al_clear_to_color(al_color_hsv(iter, 1.f, 1.f));
                al_set_target_bitmap(al_get_backbuffer(display));

                al_draw_bitmap(bitmap, 200, 200, 0);

                al_flip_display();

                iter++;
            }
        }
    } /* ... */



//cleanupEvents:
    {
        al_destroy_event_queue(event_queue);
    }
cleanupBitmap:
    {   
        al_destroy_bitmap(bitmap);
    }
cleanupDisplay:
    {
        al_destroy_display(display);
    } /* ... */
cleanupTimer:
    {
        al_destroy_timer(timer);
    }
exit:
    {
        return status;
    }
}
