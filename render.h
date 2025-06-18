#ifndef RENDER_H
#define RENDER_H

#include "terrain.h"
#include "player.h"
#include "utils.h"

// 타일 [문자열, 전경색, 배경색]
typedef struct {
    char* str;
    ColorFG color_fg;
    ColorBG color_bg;
} Tile;

// 초기화
void render_init();
void render_shutdown();

// 버퍼 초기화
void render_clear_buffer();

// 버퍼 쓰기
void render_set_tile(int x, int y, TileType tile);
void render_set_ui(int x, int y, Tile tile);
void render_set_player(const Player* player);

// 변경사항 업데이트
void render_present();

#endif
