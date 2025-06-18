#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "terrain.h"

#define MAX_PLAYERS 2

typedef struct GameManager GameManager;
struct GameManager {
    Terrain terrain;
    Player players[MAX_PLAYERS];
    int playerCount;
    int gameSet;

    void (*addPlayer)(GameManager* gm, int id, float x, float y, int facing, float speed, float power, const int key_map[KEY_TOTAL]);
};

extern GameManager gameManager;

//초기화
void initGameManager(GameManager* gm);
void shutdownGameManager(GameManager* gm);

// 게임 루프
void updateGame(GameManager* gm);
void passTurn(GameManager* gm);

#endif // GAME_H