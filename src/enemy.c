#include "game.h"

Entity CreateEnemy(void)
{
    Vector2 spawnPositions[9] = {
        (Vector2){ 200, 100},
        (Vector2){ 400, 100},
        (Vector2){ 600, 100},
        (Vector2){ 200, 300},
        (Vector2){ 700, 300},
        (Vector2){ 100, 500},
        (Vector2){ 400, 500},
        (Vector2){ 600, 500}
    };

    Entity enemy = { 0 };
    int randomPosition = rand() % 7;
    enemy.position = spawnPositions[randomPosition];
    enemy.sprite.texture = LoadTexture("resources/bufo.png");   // Texture Loading
    enemy.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)enemy.sprite.texture.width, (float)enemy.sprite.texture.height/8 };
    enemy.sprite.currentFrame = 1;
    enemy.sprite.framesCounter = 0;
    enemy.sprite.framesSpeed = 8;
    enemy.speed = 200.0f;
    enemy.permanent = 1;
    enemy.type = 'e';

    return enemy;
}

void UpdateEnemy(Entity *enemy, Entity *player, float delta)
{
    if (!CheckCollisionCircles(enemy->position, 10, player->position, 10))
    {
        Vector2 direction = FindPath(enemy->position, player->position);
        enemy->position.x -= direction.x * enemy->speed * delta;
        enemy->position.y -= direction.y * enemy->speed * delta;

        UpdateSpriteFrame(&(enemy->sprite));
    }
    else
    {
        enemy->sprite.currentFrame = 0;
        enemy->sprite.frameRec.y = (float)enemy->sprite.currentFrame*(float)enemy->sprite.texture.height/8;
    }

    if (enemy->position.x < -20)
        enemy->position.x = 800;
    else if (enemy->position.x > 800)
        enemy->position.x = -20;

    if (enemy->position.y < -20)
        enemy->position.y = 600;
    else if (enemy->position.y > 600)
        enemy->position.y = -20;
}

Vector2 FindPath(Vector2 posA, Vector2 posB)
{
    Vector2 possiblePositions[5] = { 0 };
    float distances[5] = { 0 };
    possiblePositions[0] = posA;
    possiblePositions[1] = (Vector2){ posA.x, posA.y - HEIGHT };
    possiblePositions[2] = (Vector2){ posA.x, posA.y + HEIGHT };
    possiblePositions[3] = (Vector2){ posA.x - WIDTH, posA.y };
    possiblePositions[4] = (Vector2){ posA.x + WIDTH, posA.y };

    for (int i = 0; i < 5; i++)
    {
        distances[i] = Vector2Distance(possiblePositions[i], posB);
    }
    int closestPosition = 0;
    for (int i = 1; i < 5; i++)
    {
        if (distances[i] < distances[closestPosition])
            closestPosition = i;
    }

    Vector2 d = (Vector2){ possiblePositions[closestPosition].x - posB.x, possiblePositions[closestPosition].y - posB.y };
    float D = distances[closestPosition];
    Vector2 n = (Vector2){ d.x / D, d.y / D };
    return n;
}
