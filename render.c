#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "render.h"
#include "utils.h"

// 더블 버퍼링용 버퍼
static TileType curr_buffer[HEIGHT][WIDTH];
static TileType prev_buffer[HEIGHT][WIDTH];
static Tile ui_buffer[HEIGHT][WIDTH * 2];

// 타일을 문자로 변환
Tile tile_to_char(TileType tile_type) {
    Tile tile;
    switch (tile_type) {
        case TILE_GROUND:       tile = (Tile){"##", COLORFG_BRIGHT_WHITE, COLORBG_BROWN}; break;
        case TILE_BEDROCK:      tile = (Tile){"##", COLORFG_BLACK, COLORBG_BRIGHT_BLACK}; break;
        case TILE_PLAYER_LEFT:  tile = (Tile){"=M", COLORFG_BRIGHT_WHITE, COLORBG_BRIGHT_BLUE}; break;
        case TILE_PLAYER_RIGHT: tile = (Tile){"M=", COLORFG_BRIGHT_WHITE, COLORBG_BRIGHT_BLUE}; break;
        case TILE_PROJECTILE:   tile = (Tile){"()", COLORFG_WHITE, COLORBG_BRIGHT_BLUE}; break;
        case TILE_TRAJECTORY:   tile = (Tile){"*", COLORFG_BRIGHT_WHITE, COLORBG_BRIGHT_BLUE}; break;
        default:                tile = (Tile){"  ", COLORFG_BRIGHT_WHITE, COLORBG_BRIGHT_BLUE}; break;
    }
    return tile;
}

// 렌더링 초기화
void render_init() {
    system("chcp 65001"); // UTF-8 코드 페이지 설정
    printf("\033[?25l"); // 커서 숨기기
    printf("\033[2J"); // 전체 초기화
    fflush(stdout);

    // prev 버퍼 초기화
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            prev_buffer[y][x] = TILE_NULL;
        }
    }
}

// 렌더링 종료
void render_shutdown() {
    printf("\033[%d;1H", HEIGHT + 1); // 커서 아래로
    printf("\033[?25h"); // 커서 보이기
}

// 버퍼 초기화
void render_clear_buffer() {
    memset(curr_buffer, 0, sizeof(curr_buffer));
    memset(ui_buffer, 0, sizeof(ui_buffer));
}

// 타일 설정
void render_set_tile(int x, int y, TileType tile) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(tile == TILE_TRAJECTORY && curr_buffer[y][x] != TILE_EMPTY){
            return;
        }
        curr_buffer[y][x] = tile;
    }
}

// UI 타일 설정
void render_set_ui(int x, int y, Tile tile) {
    if (x >= 0 && x < WIDTH * 2 && y >= 0 && y < HEIGHT) {
        ui_buffer[y][x] = tile;
        curr_buffer[y][x / 2] = TILE_UI;
    }
}

// 플레이어 렌더링
void render_set_player(const Player* player) {
    // 플레이어 위치와 방향에 따라 타일 설정
    if (player->pos.x >= 0 && player->pos.x < WIDTH && player->pos.y >= 0 && player->pos.y < HEIGHT) {
        curr_buffer[(int)player->pos.y][(int)player->pos.x] = player->facing == 1 ? TILE_PLAYER_RIGHT : TILE_PLAYER_LEFT;
    }
    
    if(player->fire_state != FIRE_STATE_IDLE) {
        Vector2 fire_pos = { player->pos.x + player->facing, player->pos.y };
        Projectile_trajectory_render(&player->projectile, fire_pos, player->angle_deg, player->power, player->facing);
    }

    // 플레이어의 발사체 렌더링
    Projectile_render(&player->projectile);

    // 플레이어 상태 UI 렌더링
    int state_bar_width = 10;
    int health_units = (int)((player->health / 100.0f) * state_bar_width);
    int fuel_units = (int)((player->fuel / 100.0f) * state_bar_width);

    for (int i = 0; i < state_bar_width; i++) {
        int bar_x = (int)player->pos.x * 2 + 1 - state_bar_width / 2 + i;
        int bar_y = (int)player->pos.y + 2;

        if (bar_x >= 0 && bar_x < WIDTH * 2 && bar_y >= 0) {
            render_set_ui(bar_x, bar_y, (Tile){"▀",i < health_units ? COLORFG_BRIGHT_RED : COLORFG_BLACK, i < fuel_units ? COLORBG_BRIGHT_BROWN : COLORBG_BLACK});
        }
    }
}

// 변경사항 렌더링 (더블 버퍼링)
void render_present() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // 현재 버퍼와 이전 버퍼 비교
            if (curr_buffer[y][x] != prev_buffer[y][x]) {
                Tile tile = tile_to_char(curr_buffer[y][x]);
                gotoxy(x * 2 + 1, y + 1);
                colorprint(tile.color_fg, tile.color_bg, tile.str);
                prev_buffer[y][x] = curr_buffer[y][x];
            }

            // UI 타일 렌더링
            if (ui_buffer[y][x * 2].color_fg){
                gotoxy(x * 2 + 1, y + 1);
                colorprint(ui_buffer[y][x * 2].color_fg, ui_buffer[y][x * 2].color_bg, ui_buffer[y][x * 2].str);
            }
            if (ui_buffer[y][x * 2 + 1].color_fg){
                gotoxy(x * 2 + 2, y + 1);
                colorprint(ui_buffer[y][x * 2 + 1].color_fg, ui_buffer[y][x * 2 + 1].color_bg, ui_buffer[y][x * 2 + 1].str);
            }
        }
    }
    fflush(stdout);
}
