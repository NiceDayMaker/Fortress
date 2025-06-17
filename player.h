#ifndef PLAYER_H
#define PLAYER_H

#include "projectile.h"
#include "input.h"
#include "sound.h"

#define ANGLE_STEP 0.1f
#define POWER_STEP 0.1f

typedef enum {
    FIRE_STATE_IDLE,
    FIRE_STATE_ANGLE,
    FIRE_STATE_POWER,
    FIRE_STATE_READY
} FireState;

typedef struct Player Player;

struct Player{
    int id;
    Vector2 pos;
    int facing;
    int isTurn;
    int health;
    int fuel;
    float speed;
    float angle_deg;      // 선택된 각도 (0~90도)
    float power;          // 선택된 파워 (0~1)
    float timer;          // 왕복용 타이머
    int direction;        // 왕복 방향 (1 or -1)
    FireState fire_state;
    Projectile projectile;
    InputContext input;
    SoundEffects sound_effects;

    void (*onTurn)(Player* p);
    void (*update)(Player* p, const Terrain* terrain);
    void (*fire)(Player* p);
};

void Player_init(Player* p, int id, float x, float y, int facing, float speed, float power, const int key_map[KEY_TOTAL]);

#endif // PLAYER_H