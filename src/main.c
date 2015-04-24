#include "init.h"
#include "spa.h"

int main(int argc, char **argv) {

  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_FONT *font;

  int score, level;
  int hater_count, bullet_count, poof_count;
  double event_time, logic_time, render_time, osd_time;

  entity_list spl, shl, sbl, sel;

  {
    if (!spa_init(&event_queue,
		  &display,
		  &timer,
		  &font))
      goto cleanup;

    LIST_INIT(&spl);
    LIST_INIT(&shl);
    LIST_INIT(&sbl);
    LIST_INIT(&sel);
  } /* ... */

  {
    bool redraw;

    while(!spa_events(event_queue, &redraw, (player*)spl.lh_first, sbl)) {

      if (redraw && al_is_event_queue_empty(event_queue)) {
	redraw = false;

	spa_render(&spl, &shl, &sbl, &sel);

	spa_osd(font, score, level,
		hater_count, bullet_count, poof_count,
		event_time, logic_time, render_time, osd_time,
		(player*)spl->lh_first);
      }
    }
  } /* ... */

 cleanup:
  spa_shutdown(event_queue,
	       display,
	       timer,
	       font);

  return 0;
}
