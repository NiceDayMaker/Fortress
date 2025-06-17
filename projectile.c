#include <stdio.h>
#include <math.h>
#include "game.h"
#include "projectile.h"
#include "render.h"
#include "sound.h"

void Projectile_init(Projectile* p, float power) {
    p->active = 0;
    p->power = power;
}

void Projectile_fire(Projectile* p, Vector2 start, float angle_deg, float power, int facing) {
    if (p->active) return;
    p->pos = start;

    float rad = angle_deg * (3.141592f / 180.0f);
    p->vel.x = cosf(rad) * power * p->power * facing;
    p->vel.y = -sinf(rad) * power * p->power;
    p->active = 1;
}

void Projectile_update(Projectile* p, Terrain* terrain) {
    if (!p->active) return;

    // 중력 적용
    p->vel.y += GRAVITY;

    // 이동
    p->pos.x += p->vel.x;
    p->pos.y += p->vel.y;

    float tx = p->pos.x;
    float ty = p->pos.y;

    // 충돌
    if (tx < 0 || tx >= WIDTH || ty >= HEIGHT - 1
        || terrain->get(terrain, (int)ty, (int)tx) == TILE_GROUND
        || terrain->get(terrain, (int)(ty + 0.2f), (int)(tx + 0.2f)) == TILE_GROUND
        || terrain->get(terrain, (int)(ty + 0.2f), (int)(tx - 0.2f)) == TILE_GROUND
        || terrain->get(terrain, (int)(ty - 0.2f), (int)(tx + 0.2f)) == TILE_GROUND
        || terrain->get(terrain, (int)(ty - 0.2f), (int)(tx - 0.2f)) == TILE_GROUND
    ) {
        playExplosionSound();
        Terrain_destroy_circle(terrain, (int)tx, (int)ty, 2);
        for(int i = 0; i < gameManager.playerCount; i++) {
            Player* player = &gameManager.players[i];
            float dx = player->pos.x - p->pos.x;
            float dy = player->pos.y - (p->pos.y - 1.0f);
            float distance_sqr = dx * dx + dy * dy;
            if (distance_sqr <= 10.0f) {
                player->health -= (int)((10.0f - distance_sqr) * 6.0f);
                if (player->health < 0) {
                    player->health = 0;
                }
            }
        }
        p->active = 0;
        passTurn(&gameManager);
    }
}

void Projectile_trajectory_render(const Projectile* p, Vector2 start, float angle_deg, float power, int facing) {
    float rad = angle_deg * (3.141592f / 180.0f);
    for (float t = 0; t <= 10.0f; t += 2.0f) {
        float x = start.x + cosf(rad) * power * p->power * facing * t;
        float y = start.y - sinf(rad) * power * p->power * t + 0.5f * GRAVITY * t * t;
        render_set_tile((int)x, (int)y, TILE_TRAJECTORY);
    }
}

void Projectile_render(const Projectile* p) {
    if (!p->active) return;
    int x = (int)(p->pos.x);
    int y = (int)(p->pos.y);
    render_set_tile(x, y, TILE_PROJECTILE);
}
