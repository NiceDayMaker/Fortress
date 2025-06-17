#include <math.h>
#include <windows.h>
#include "player.h"
#include "render.h"
#include "utils.h"
#include "input.h"
#include "sound.h"

void Player_onTurn_impl(Player* p) {
    p->isTurn = 1;
    p->fuel = 100;
}

void Player_update_impl(Player* p, const Terrain* terrain) {
    Vector2 new_pos = p->pos;
    int moved = 0;

    if(p->health <= 0) {
        return;
    }

    if(!p->isTurn) {
        goto PASSINPUT;
    }

    Input_update(&p->input);

    switch (p->fire_state)
    {
        case FIRE_STATE_IDLE:
            if(p->fuel <= 0){
                goto NOFUEL;
            }

            if (Input_onHold(&p->input, KEY_LEFT)) {
                float tx = p->pos.x - p->speed;
                float ty = p->pos.y;

                if (tx >= 0) {
                    if (terrain->get(terrain, ty, tx) == TILE_GROUND && ty > 0
                        && terrain->get(terrain, ty - 1, p->pos.x) == TILE_EMPTY
                        && terrain->get(terrain, ty - 1, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        new_pos.y = ty - 1;
                        moved = 1;
                    }
                    else if (terrain->get(terrain, ty, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        moved = 1;
                    }
                }
            
                p->facing = -1;
            }

            if (Input_onHold(&p->input, KEY_RIGHT)) {
                float tx = p->pos.x + p->speed;
                float ty = p->pos.y;
            
                if (tx < WIDTH) {
                    if (terrain->get(terrain, ty, tx) == TILE_GROUND && ty > 0
                        && terrain->get(terrain, ty - 1, p->pos.x) == TILE_EMPTY
                        && terrain->get(terrain, ty - 1, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        new_pos.y = ty - 1;
                        moved = 1;
                    }
                    else if (terrain->get(terrain, ty, tx) == TILE_EMPTY) {
                        new_pos.x = tx;
                        moved = 1;
                    }
                }
            
                p->facing = 1;
            }

            // 이동 반영
            p->pos = new_pos;
            
            NOFUEL:

            if (moved) {
                p->fuel--;
                playMoveSound(&p->sound_effects);
            }
            else {
                stopMoveSound(&p->sound_effects);
            }

            if (Input_onDown(&p->input, KEY_SHOOT)) {
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->angle_deg = 45.0f;
                p->power = 0.5f;
                p->fire_state = FIRE_STATE_ANGLE;
                stopMoveSound(&p->sound_effects);
                playSetSound(&p->sound_effects);
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

            if (Input_onDown(&p->input, KEY_SHOOT)) {
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->fire_state = FIRE_STATE_POWER;
                playSetSound(&p->sound_effects);
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

            if (Input_onDown(&p->input, KEY_SHOOT)) {
                p->timer = 0.5f; // 타이머 초기화
                p->direction = 1; // 왕복 방향 초기화
                p->fire_state = FIRE_STATE_READY;
                playSetSound(&p->sound_effects);
            }
            break;
        
        case FIRE_STATE_READY:
            if (Input_onDown(&p->input, KEY_SHOOT)) {
                p->fire_state = FIRE_STATE_IDLE;
                p->fire(p);
            }
            break;
        
        default:
            break;
    }

    PASSINPUT:

    // 중력
    if (p->pos.y + 1 < HEIGHT &&
        terrain->get(terrain, p->pos.y + 1, p->pos.x) == TILE_EMPTY) {
        p->pos.y += 1;
    }

    Projectile_update(&p->projectile, (Terrain*)terrain);
}

void Player_fire_impl(Player* p) {
    p->isTurn = 0;
    p->fuel = 0;
    Vector2 fire_pos = { p->pos.x + p->facing, p->pos.y };
    playShootSound(&p->sound_effects);
    Projectile_fire(&p->projectile, fire_pos, p->angle_deg, p->power, p->facing);
}

void Player_init(Player* p, int id, float x, float y, int facing, float speed, float power, const int key_map[KEY_TOTAL]) {
    Input_init(&p->input, key_map);
    initSounds(&p->sound_effects);

    p->onTurn = Player_onTurn_impl;
    p->update = Player_update_impl;
    p->fire = Player_fire_impl;

    p->id = id;
    p->pos.x = x;
    p->pos.y = y;
    p->health = 100;
    p->facing = facing;
    p->speed = speed;

    p->angle_deg = 45.0f;
    p->power = 1.0f;
    p->timer = 0.0f;
    p->direction = 1;
    p->fire_state = FIRE_STATE_IDLE;

    Projectile_init(&p->projectile, power);
}