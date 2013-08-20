#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"

void print_version(char* t, int v) {

  fprintf(stdout, "%s version: %d.%d.%dr%d\n", t,
	  v >> 24, (v >> 16) & 255, (v >> 8) & 255, v & 255);
}

bool spa_init(ALLEGRO_EVENT_QUEUE **event_queue,
	      ALLEGRO_DISPLAY **display,
	      ALLEGRO_TIMER **timer,
	      ALLEGRO_FONT **font) {
  {
    if (!al_init()) {
      fprintf(stderr, "al_init(): failed\n");
      return false;
    }
    else {
      print_version("allegro", al_get_allegro_version());
    }
  } /* ... */

  double init_time = al_get_time();

  {
    *event_queue = al_create_event_queue();
    if (!(*event_queue)) {
      fprintf(stderr, "al_create_event_queue(): failed\n");
      return false;
    }
  } /* ... */

  {
    *display = al_create_display(SPA_DISPLAY_WIDTH, SPA_DISPLAY_HEIGHT);
    if (!(*display)) {
      fprintf(stderr, "al_create_display(): failed\n");
      return false;
    }
  } /* ... */
  
  {
    *timer = al_create_timer(1.0 / SPA_DISPLAY_FPS);
    if (!(*timer)) {
      fprintf(stderr, "al_create_timer(): failed\n");
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
    al_register_event_source(*event_queue,
			     al_get_display_event_source(*display));
    
    al_register_event_source(*event_queue,
			     al_get_timer_event_source(*timer));

    al_register_event_source(*event_queue,
			     al_get_keyboard_event_source());

    al_register_event_source(*event_queue,
			     al_get_mouse_event_source());
  } /* ... */

  {
    al_init_font_addon();
    al_init_ttf_addon();
    *font = al_load_font("DejaVuSansMono.ttf", SPA_FONT_SIZE, 0);
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
  
  fprintf(stdout, "spa_init() in %fs\n", al_get_time() - init_time);

  return true;
}

void spa_shutdown(ALLEGRO_EVENT_QUEUE *event_queue,
		  ALLEGRO_DISPLAY *display,
		  ALLEGRO_TIMER *timer,
		  ALLEGRO_FONT *font) {
  
  al_shutdown_primitives_addon();

  {
    al_destroy_font(font);
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
  } /* ... */
  
  {
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
  } /* ... */
}
