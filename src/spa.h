#pragma once

const int SCREEN_W = 640 * SCALE;
const int SCREEN_H = 640 * ASPECT * SCALE;
const float FPS = 60;

bool spa_init(void);
void spa_game_reset(void);
void spa_logic_update(void);
void spa_osd(void);
bool spa_loop(bool *redraw);
void spa_render(void);
void print_version(const char* text, int v);
