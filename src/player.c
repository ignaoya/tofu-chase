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

		Vector2 direction = (Vector2) { 0.0f, 0.0f };

        if (IsKeyDown(KEY_LEFT))
			direction.x += 1.0;
        if (IsKeyDown(KEY_RIGHT))
			direction.x -= 1.0;
        if (IsKeyDown(KEY_DOWN))
			direction.y -= 1.0;
        if (IsKeyDown(KEY_UP))
			direction.y += 1.0;

		float distance = Vector2Distance((Vector2){ 0.0f, 0.0f }, direction);
		// Normalized direction ( to avoid duplicating speed when moving diagonally )
		Vector2 nDirection = (Vector2){ direction.x / distance, direction.y / distance };

		entity->position.x -= nDirection.x * entity->speed * delta;
		entity->position.y -= nDirection.y * entity->speed * delta;
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

