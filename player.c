#include "player.h"
#include "render.h"
#include <windows.h>
#include <math.h>

void Player_init(Player* p, float x, float y, float speed, float power) {
    p->speed = speed;
    p->pos.x = x;
    p->pos.y = y;
    p->facing = 1;

    p->angle_deg = 45.0f;
    p->power = 1.0f;
    p->timer = 0.0f;
    p->direction = 1;
    p->fire_state = FIRE_STATE_IDLE;

    Projectile_init(&p->projectile, power);
}

void Player_update(Player* p, const Terrain* terrain) {
    static int pressed_shot = 0;

    if(!(GetAsyncKeyState('W') & 0x8000)) {
        pressed_shot = 0;
    }

    Vector2 new_pos = p->pos;

    switch (p->fire_state)
    {
        case FIRE_STATE_IDLE:
            if (GetAsyncKeyState('A') & 0x8000) {
                float tx = p->pos.x - p->speed;
                float ty = p->pos.y;

                if (tx >= 0) {
                    if (terrain->get(terrain, ty, tx) == TILE_GROUND && ty > 0
                        && terrain->get(terrain, ty - 1, p->pos.x) == TILE_EMPTY
                        && terrain->get(terrain, ty - 1, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        new_pos.y = ty - 1;
                    }
                    else if (terrain->get(terrain, ty, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                    }
                }
            
                p->facing = -1;
            }

            if (GetAsyncKeyState('D') & 0x8000) {
                float tx = p->pos.x + p->speed;
                float ty = p->pos.y;
            
                if (tx < WIDTH) {
                    if (terrain->get(terrain, ty, tx) == TILE_GROUND && ty > 0
                        && terrain->get(terrain, ty - 1, p->pos.x) == TILE_EMPTY
                        && terrain->get(terrain, ty - 1, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        new_pos.y = ty - 1;
                    }
                    else if (terrain->get(terrain, ty, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                    }
                }
            
                p->facing = 1;
            }

            // 이동 반영
            p->pos = new_pos;

            if (!pressed_shot && GetAsyncKeyState('W') & 0x8000) {
                pressed_shot = 1;
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->angle_deg = 45.0f;
                p->power = 0.5f;
                p->fire_state = FIRE_STATE_ANGLE;
            }
            break;
        
        case FIRE_STATE_ANGLE:
            p->timer += ANGLE_STEP * p->direction;
            if (p->timer >= 1.0f) {
                p->timer = 1.0f;
                p->direction = -1;
            } else if (p->timer <= 0.0f) {
                p->timer = 0.0f;
                p->direction = 1;
            }
            p->angle_deg = 90.0f * p->timer;

            if (!pressed_shot && GetAsyncKeyState('W') & 0x8000) {
                pressed_shot = 1;
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->fire_state = FIRE_STATE_POWER;
            }
            break;
        
        case FIRE_STATE_POWER:
            p->timer += POWER_STEP * p->direction;
            if (p->timer >= 1.0f) {
                p->timer = 1.0f;
                p->direction = -1;
            } else if (p->timer <= 0.2f) {
                p->timer = 0.2f;
                p->direction = 1;
            }
            p->power = p->timer;

            if (!pressed_shot && GetAsyncKeyState('W') & 0x8000) {
                pressed_shot = 1;
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->fire_state = FIRE_STATE_READY;
            }
            break;
        
        case FIRE_STATE_READY:
            if (GetAsyncKeyState('W') & 0x8000) {
                pressed_shot = 1;
                p->fire_state = FIRE_STATE_IDLE;
                Player_fire(p);
            }
            break;
        
        default:
            break;
    }

    // 중력
    if (p->pos.y + 1 < HEIGHT &&
        terrain->get(terrain, p->pos.y + 1, p->pos.x) == TILE_EMPTY) {
        p->pos.y += 1;
    }

    Projectile_update(&p->projectile, (Terrain*)terrain);
}

void Player_fire(Player* p) {
    Vector2 fire_pos = { p->pos.x + p->facing, p->pos.y };
    Projectile_fire(&p->projectile, fire_pos, p->angle_deg, p->power, p->facing);
}