#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

void print_version(char* t, int v);
bool spa_init(ALLEGRO_EVENT_QUEUE **event_queue,
	      ALLEGRO_DISPLAY **display,
	      ALLEGRO_TIMER **timer,
	      ALLEGRO_FONT **font);
void spa_shutdown(ALLEGRO_EVENT_QUEUE *event_queue,
		  ALLEGRO_DISPLAY *display,
		  ALLEGRO_TIMER *timer,
		  ALLEGRO_FONT *font);
