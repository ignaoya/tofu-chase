#include "game.h"

Entity CreatePlayer(void)
{
    Entity player = { 0 };
    player.position = (Vector2){350.0f, 280.0f};
    player.sprite.texture = LoadTexture("resources/tofu.png");  // Texture Loading
    player.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)player.sprite.texture.width, (float)player.sprite.texture.height/8 };
    player.sprite.currentFrame = 1;
    player.sprite.framesCounter = 0;
    player.sprite.framesSpeed = 8;
    player.speed = 250.0f;
    player.permanent = 1;
    player.type = 'p';

    return player;
}

void UpdatePlayer(Entity *entity, float delta)
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_UP))
    {
        UpdateSpriteFrame(&(entity->sprite));

        if (IsKeyDown(KEY_LEFT))
            entity->position.x -= entity->speed * delta;
        if (IsKeyDown(KEY_RIGHT))
            entity->position.x += entity->speed * delta;
        if (IsKeyDown(KEY_DOWN))
            entity->position.y += entity->speed * delta;
        if (IsKeyDown(KEY_UP))
            entity->position.y -= entity->speed * delta;
    }
    else
    {
        entity->sprite.currentFrame = 0;
        entity->sprite.frameRec.y = (float)entity->sprite.currentFrame*(float)entity->sprite.texture.height/8;
    }

    if (entity->speed < 250.0f)
        entity->speed++;

    if (entity->position.x < -20)
        entity->position.x = 800;
    else if (entity->position.x > 800)
        entity->position.x = -20;

    if (entity->position.y < -20)
        entity->position.y = 600;
    else if (entity->position.y > 600)
        entity->position.y = -20;
}

