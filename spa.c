#include <allegro5/allegro.h>

#include "config.h"
#include "spa.h"
#include "entity.h"

double spa_render(entity_list* spl,
		entity_list* shl,
	        entity_list* sbl,
		entity_list* sel) {

  double t = al_get_time();

  al_clear_to_color(al_map_rgb(10, 10, 20));

  entity_list* list_array[] = {spl, shl, sbl, sel};

  for (int i = 0; i < sizeof(list_array); i++) {
    
    for (entity* e = list_array[i]->lh_first;
	 e != NULL; e = e->list.le_next) {

      (e->draw)(e);
    }
  }

  return al_get_time() - t;
}

void spa_draw_text(ALLEGRO_FONT* font,
		   int cy, text_position tp,
		   char *text, double n) {

  ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
 
  switch(tp) {
  case top_left:
    al_draw_textf(font, color,
		  SPA_FONT_SPACING,
		  SPA_FONT_SPACING + (SPA_FONT_SIZE * cy),
		  ALLEGRO_ALIGN_LEFT,
		  text, n);
    break;
  case top_right:
    al_draw_textf(font, color,
		  SPA_DISPLAY_WIDTH - SPA_FONT_SPACING,
		  SPA_FONT_SPACING + (SPA_FONT_SIZE * cy),
		  ALLEGRO_ALIGN_RIGHT,
		  text, n);
    break;
  case bottom_left:
    al_draw_textf(font, color,
		  SPA_FONT_SPACING,
		  SPA_DISPLAY_HEIGHT - SPA_FONT_SPACING - (SPA_FONT_SIZE * cy),
		  ALLEGRO_ALIGN_LEFT,
		  text, n);
    break;
  case bottom_right:
    al_draw_textf(font, color,
		  SPA_DISPLAY_WIDTH - SPA_FONT_SPACING,
		  SPA_DISPLAY_HEIGHT - SPA_FONT_SPACING - (SPA_FONT_SIZE * cy),
		  ALLEGRO_ALIGN_RIGHT,
		  text, n);
    break;
  case center_center:
    al_draw_textf(font, color,
		  SPA_DISPLAY_WIDTH / 2,
		  SPA_DISPLAY_HEIGHT / 2 + (SPA_FONT_SIZE * cy),
		  ALLEGRO_ALIGN_CENTER,
		  text, n);
    break;
  }
}

double spa_osd(ALLEGRO_FONT* font,
	       int score,
	       int level,
	       
	       int hater_count,
	       int bullet_count,
	       int poof_count,

	       double event_time,
	       double logic_time,
	       double render_time,
	       double osd_time,

	       player* player) {

  double t = al_get_time();

  {
    spa_draw_text(font, 0, top_left, "health: %d", player->health);
    spa_draw_text(font, 1, top_left, "score: %d", score);
    spa_draw_text(font, 2, top_left, "level: %d", level);

    spa_draw_text(font, 3, top_left, "bullets: %d", bullet_count);
    spa_draw_text(font, 4, top_left, "haters: %d", hater_count);
    spa_draw_text(font, 5, top_left, "poofs: %d", poof_count);

    spa_draw_text(font, 0, top_right, "event: %.2fs", event_time);
    spa_draw_text(font, 1, top_right, "logic: %.2fs", logic_time);
    spa_draw_text(font, 2, top_right, "render: %.2fs", render_time);
    spa_draw_text(font, 3, top_right, "osd: %.2fs", osd_time);
  } /* ... */
  
  {
    char* weapon;
    switch (player->weapon) {
    case rifle:
      weapon = "rifle";
      break;
    case laser:
      weapon = "laser";
      break;
    }

    al_draw_textf(font, al_map_rgb(255, 255, 255), SPA_DISPLAY_WIDTH / 2, SPA_FONT_SPACING,
		  ALLEGRO_ALIGN_CENTER, "weapon: %s", weapon);
  } /* ... */

  {
    if (player->health <= 0) {
      spa_draw_text(font, 0, center_center, "you died %d", 0);
      spa_draw_text(font, 1, center_center,
		    "(press [BACKSPACE] to reset) %d", 0);
    }
  } /* ... */

  return al_get_time() - t;
}

bool spa_events(ALLEGRO_EVENT_QUEUE *event_queue,
		bool *redraw,
		player *player,
		entity_list *sbl) {
  
  ALLEGRO_EVENT ev;
  ALLEGRO_KEYBOARD_STATE ks;

  al_wait_for_event(event_queue, &ev);

  if (ev.type == ALLEGRO_EVENT_TIMER) {
    *redraw = true;
  }
  else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    return true;

  return false;
}
