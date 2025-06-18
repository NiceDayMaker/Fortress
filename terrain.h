#ifndef TERRAIN_H
#define TERRAIN_H

#define WIDTH 50
#define HEIGHT 25

// 타일 타입
typedef enum {
    TILE_NULL = -1,
    TILE_EMPTY = 0,
    TILE_GROUND = 1,
    TILE_BEDROCK = 2,
    TILE_PLAYER_LEFT = 3,
    TILE_PLAYER_RIGHT = 4,
    TILE_PROJECTILE = 5,
    TILE_TRAJECTORY = 6,

    TILE_UI = 100
} TileType;

// 지형 구조체
typedef struct Terrain Terrain;

struct Terrain {
    TileType map[HEIGHT][WIDTH];

    void (*generate)(Terrain*);
    TileType (*get)(const Terrain*, int y, int x);
};

void Terrain_init(Terrain* t);
void Terrain_destroy_circle(Terrain* t, int cx, int cy, int radius);

#endif // TERRAIN_H
