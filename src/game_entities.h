#pragma once
#include "tower.h"
#include "enemy.h"
#include <vector>

// ==========================
// Tower types
// ==========================
extern const TowerStats BASIC_TOWER;
extern const TowerStats SNIPER_TOWER;
extern const TowerStats CANNON_TOWER;
extern const TowerStats LASER_TOWER;

// ==========================
// Enemy types
// ==========================
extern const EnemyStats FAST_ENEMY;
extern const EnemyStats TANK_ENEMY;
extern const EnemyStats NORMAL_ENEMY;

// Optional: spawn waves
struct EnemyWave {
    std::vector<EnemyStats> enemies;
};
