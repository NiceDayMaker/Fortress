#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "render.h"
#include "utils.h"

static TileType curr_buffer[HEIGHT][WIDTH];
static TileType prev_buffer[HEIGHT][WIDTH];

static int player_facing = 1;

Tile tile_to_char(TileType tile_type) {
    Tile tile;
    switch (tile_type) {
        case TILE_GROUND:       tile = (Tile){"##", COLORFG_WHITE, COLORBG_BROWN}; break;
        case TILE_PLAYER:       tile = (Tile){player_facing == 1 ? "M=" : "=M", COLORFG_WHITE, COLORBG_BRIGHT_BLUE}; break;
        case TILE_PROJECTILE:   tile = (Tile){"()", COLORFG_WHITE, COLORBG_BRIGHT_BLUE}; break;
        case TILE_TRAJECTORY:   tile = (Tile){"* ", COLORFG_WHITE, COLORBG_BRIGHT_BLUE}; break;
        default:                tile = (Tile){"  ", COLORFG_WHITE, COLORBG_BRIGHT_BLUE}; break;
    }
    return tile;
}

void render_init() {
    printf("\033[?25l"); // 커서 숨기기
    system("color 9F");
    printf("\033[2J"); // 전체 초기화
    fflush(stdout);
}

void render_shutdown() {
    printf("\033[%d;1H", HEIGHT + 1); // 커서 아래로
    printf("\033[?25h"); // 커서 보이기
}

void render_clear_buffer() {
    memset(curr_buffer, 0, sizeof(curr_buffer));
    player_facing = 1;
}

void render_set_tile(int x, int y, TileType tile) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(tile == TILE_TRAJECTORY && curr_buffer[y][x] != TILE_EMPTY){
            return;
        }
        curr_buffer[y][x] = tile;
    }
}

void render_set_player(const Player* player) {
    if (player->pos.x >= 0 && player->pos.x < WIDTH && player->pos.y >= 0 && player->pos.y < HEIGHT) {
        curr_buffer[(int)player->pos.y][(int)player->pos.x] = TILE_PLAYER;
        player_facing = player->facing;
    }
    
    if(player->fire_state != FIRE_STATE_IDLE) {
        Vector2 fire_pos = { player->pos.x + player->facing, player->pos.y };
        Projectile_trajectory_render(&player->projectile, fire_pos, player->angle_deg, player->power, player->facing);
    }

    Projectile_render(&player->projectile);
}

void render_present() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (curr_buffer[y][x] != prev_buffer[y][x] || curr_buffer[y][x] == TILE_PLAYER) {
                Tile tile = tile_to_char(curr_buffer[y][x]);
                gotoxy(x, y);
                colorprint(tile.color_fg, tile.color_bg, tile.str);
                prev_buffer[y][x] = curr_buffer[y][x];
            }
        }
    }
    fflush(stdout);
}
