#pragma once

#include <sys/queue.h>

typedef struct poof poof;
typedef struct poof_list poof_list;

struct poof {
	float x;
	float y;
	float iteration;
	LIST_ENTRY(poof) poof_p;
};

LIST_HEAD(poof_list, poof);

poof* spa_poof_create(int, int);
void spa_poof_destroy(poof*);

void spa_poof_add(poof_list*, int, int);
poof* spa_poof_remove(poof*);

void spa_poof_draw(poof*);
