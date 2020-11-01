#include "game.h"

int CheckGameOver(Entity *player, Entity *enemy, int gameOver, Sound waca)
{
    if (CheckCollisionCircles(enemy->position, 10, player->position, 10))
    {
        if (!gameOver)
            PlaySoundMulti(waca);

        return 1;
    }
    else
        return 0;
}

void ClearEverything(Entity player, Entity enemy, Entity balls[])
{
    UnloadTexture(player.sprite.texture);
    UnloadTexture(enemy.sprite.texture);
    for (int i = 0; i < MAX_BALLS; i++)
    {
        UnloadTexture(balls[i].sprite.texture);
    }
}

