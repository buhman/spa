AM_CFLAGS = \
	$(ALLEGRO_CFLAGS) \
	$(ALLEGRO_FONT_CFLAGS) \
	$(ALLEGRO_TTF_CFLAGS) \
	$(ALLEGRO_PRIMITIVES_CFLAGS)

bin_PROGRAMS = spa

spa_SOURCES = \
	bullet.h bullet.c \
	entity.h entity.c \
	hater.h hater.c \
	laser.h laser.c \
	player.h player.c \
	poof.h poof.c \
	spa.h spa.c \
	xlines.h xlines.c

spa_LDADD = \
	$(ALLEGRO_LIBS) \
	$(ALLEGRO_FONT_LIBS) \
	$(ALLEGRO_TTF_LIBS) \
	$(ALLEGRO_PRIMITIVES_LIBS) \
	-lm
