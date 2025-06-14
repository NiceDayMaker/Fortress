#ifndef PLAYER_H
#define PLAYER_H

#include "projectile.h"

#define ANGLE_STEP 0.1f
#define POWER_STEP 0.1f

typedef enum {
    FIRE_STATE_IDLE,
    FIRE_STATE_ANGLE,
    FIRE_STATE_POWER,
    FIRE_STATE_READY
} FireState;

typedef struct {
    Vector2 pos;
    int facing;
    float speed;
    float angle_deg;      // 선택된 각도 (0~90도)
    float power;          // 선택된 파워 (0~1)
    float timer;          // 왕복용 타이머
    int direction;        // 왕복 방향 (1 or -1)
    FireState fire_state;
    Projectile projectile;
} Player;

void Player_init(Player* p, float x, float y, float speed, float power);
void Player_update(Player* p, const Terrain* terrain);
void Player_fire(Player* p);

#endif // PLAYER_H