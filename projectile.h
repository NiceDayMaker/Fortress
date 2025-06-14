#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "utils.h"
#include "terrain.h"

#define GRAVITY 0.1f

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float power;
    int active;
} Projectile;

void Projectile_init(Projectile* p, float power);
void Projectile_fire(Projectile* p, Vector2 start, float angle_deg, float power, int facing);
void Projectile_update(Projectile* p, Terrain* terrain);
void Projectile_trajectory_render(const Projectile* p, Vector2 start, float angle_deg, float power, int facing);
void Projectile_render(const Projectile* p);

#endif
