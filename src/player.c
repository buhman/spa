#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "entity.h"
#include "player.h"

ALLEGRO_BITMAP *player_bitmap = NULL;

bool spa_player_init(ALLEGRO_DISPLAY *display) {

  {
    player_bitmap = al_create_bitmap(SPA_PLAYER_WIDTH, SPA_PLAYER_HEIGHT); 
    if (!player_bitmap) { 
      fprintf(stderr, "al_create_bitmap(): failed\n"); 
      return false; 
    } 
  } /* ... */

  {
    int width = SPA_PLAYER_WIDTH - 1;
    int height = SPA_PLAYER_HEIGHT - 1;
    ALLEGRO_COLOR color = SPA_PLAYER_COLOR;

    al_set_target_bitmap(player_bitmap);

    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    al_draw_triangle(0, height,
		     width, height,
		     width / 2, 0,
		     color, 1);

    al_draw_triangle(0, height,
		     1, 1,
		     width / 3, height,
		     color, 1);
    
    al_draw_triangle(width, height,
		     width, 1,
		     width - (width / 3), height ,
		     color, 1);

    al_set_target_bitmap(al_get_backbuffer(display));
  } /* ... */

  return true;
}

void spa_player_shutdown() {
  if (player_bitmap)
    al_destroy_bitmap(player_bitmap);
}

void spa_player_damage(player* player, int damage) {

  if (player->health - damage <= 0) {
    player->health = 0;
  }
  else
    player->health -= damage;
}
