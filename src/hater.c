#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "entity.h"
#include "hater.h"
#include "bullet.h"
#include "player.h"

ALLEGRO_BITMAP *hater_bitmap = NULL;

bool spa_hater_init(ALLEGRO_DISPLAY *display) {

  {
    hater_bitmap = al_create_bitmap(SPA_HATER_WIDTH, SPA_HATER_HEIGHT);
    if (!hater_bitmap) {
      fprintf(stderr, "al_create_bitmap(): failed\n");
      return false;
    }
  } /* ... */

  {
    int width = SPA_HATER_WIDTH - 1;
    int height = SPA_HATER_HEIGHT - 1;

    al_set_target_bitmap(hater_bitmap);

    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    al_draw_circle(
		   width / 2,
		   height / 2 + height / 6,
		   width / 2.8,
		   SPA_HATER_COLOR, 1);

    al_draw_circle(
		   width / 8,
		   height / 8,
		   width / 6,
		   SPA_HATER_COLOR, 1);

    al_draw_circle(
		   width - width / 8,
		   height / 8,
		   width / 6,
		   SPA_HATER_COLOR, 1);

    al_set_target_bitmap(al_get_backbuffer(display));
  } /* ... */

  return true;
}

void spa_hater_shutdown() {
  if (hater_bitmap)
    al_destroy_bitmap(hater_bitmap);
}

void spa_hater_update(hater* hater, player* player, entity_list *sbl, int level) {

  {
    float o = (hater->e.y - player->e.y);
    float a = (hater->e.x - player->e.x);

    float theta = atan2(o, a);

    hater->e.theta = theta - M_PI_2;
  } /* ... */

  {
    int r = rand() % 255;
    if (r > 255 - (level * 1.1)) {

      spa_add_bullet(sbl, (entity*)hater);
    }

  } /* ... */
}

void spa_hater_add(entity_list *shl, int quantity) {

  for (int i = 0; i < quantity; i++) {

    hater* h;
    LIST_INSERT_HEAD(shl, (entity*)h, list);
  }
}
