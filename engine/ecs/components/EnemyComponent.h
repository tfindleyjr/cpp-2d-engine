#pragma once

struct EnemyComponent
{
    float moveSpeed = 120.0f;
    int contactDamage = 1;
    float attackCooldown = 0.75f;
    float attackTimer = 0.0f;
};
