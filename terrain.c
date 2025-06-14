#include "terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

static void Terrain_generate_impl(Terrain* t) {
    int ground[WIDTH];
    int y = HEIGHT * 2 / 3;
    srand((unsigned int)time(NULL));

    for (int x = 0; x < WIDTH / 2; x++) {
        y += (rand() % 3) - 1;
        if (y < HEIGHT / 3) y = HEIGHT / 3;
        if (y > HEIGHT - 3) y = HEIGHT - 3;
        ground[x] = y;
    }

    for (int x = 0; x < WIDTH / 2; x++) {
        ground[WIDTH - 1 - x] = ground[x];
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            t->map[y][x] = (y >= ground[x]) ? TILE_GROUND : TILE_EMPTY;
        }
    }
}

static TileType Terrain_get_impl(const Terrain* t, int y, int x) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return TILE_EMPTY;
    return t->map[y][x];
}

void Terrain_init(Terrain* t) {
    t->generate = Terrain_generate_impl;
    t->get = Terrain_get_impl;
    t->generate(t);
}

void Terrain_destroy_circle(Terrain* t, int cx, int cy, int radius) {
    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) continue;
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy <= radius * radius) {
                if (t->map[y][x] == TILE_GROUND)
                    t->map[y][x] = TILE_EMPTY;
            }
        }
    }
}