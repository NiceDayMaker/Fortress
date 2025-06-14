#ifndef TERRAIN_H
#define TERRAIN_H

#define WIDTH 48
#define HEIGHT 27

typedef enum {
    TILE_EMPTY = 0,
    TILE_GROUND = 1,
    TILE_PLAYER = 2,
    TILE_PROJECTILE = 3,
    TILE_TRAJECTORY = 4,
} TileType;

typedef struct Terrain Terrain;

struct Terrain {
    TileType map[HEIGHT][WIDTH];

    void (*generate)(Terrain*);
    TileType (*get)(const Terrain*, int y, int x);
};

void Terrain_init(Terrain* t);
void Terrain_destroy_circle(Terrain* t, int cx, int cy, int radius);

#endif // TERRAIN_H
