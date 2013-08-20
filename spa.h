#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "entity.h"
#include "player.h"

typedef enum text_position text_position;

enum text_position {
  top_left,
  top_right,
  bottom_left,
  bottom_right,
  center_center
};

double spa_render(entity_list* spl,
		  entity_list* shl,
		  entity_list* sbl,
		  entity_list* sel);

void spa_draw_text(ALLEGRO_FONT* font,
		   int cy, text_position tp,
		   char* text, double n);

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
	       player* player);

bool spa_events(ALLEGRO_EVENT_QUEUE *event_queue,
		bool* redraw,
		player* player,
		entity_list* sbl);
