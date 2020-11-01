#include "game.h"


Entity CreateBall(int permanent, char type, Vector2 position)
{
    if (!permanent)
    {
        int lower = 10, upperHeight = 550, upperWidth = 750;
        int typeRandom = (rand() % 10) + 1;
        Entity ball = { 0 };
        ball.position = (Vector2){(float)((rand() % (upperWidth - lower + 1)) + lower), (float)((rand() % (upperHeight - lower + 1)) + lower)};

        if (typeRandom < 8)
        {
            ball.sprite.texture = LoadTexture("resources/blue-ball.png");   // Texture Loading
            ball.type = 'b';
        }
        else if (typeRandom < 10)
        {
            ball.sprite.texture = LoadTexture("resources/green-ball.png");  // Texture Loading
            ball.type = 'g';
        }
        else
        {
            ball.sprite.texture = LoadTexture("resources/red-ball.png");
            ball.type = 'r';
        }

        ball.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)ball.sprite.texture.width/4, (float)ball.sprite.texture.height };
        ball.sprite.currentFrame = 0;
        ball.sprite.framesCounter = 0;
        ball.sprite.framesSpeed = 8;
        ball.speed = 0.0f;
        ball.permanent = 1;
        return ball;
    }
	else
    {
        Entity boom = { 0 };
        boom.position = position;

        if (type == 'b')
            boom.sprite.texture = LoadTexture("resources/boom.png");
        else if (type == 'g')
            boom.sprite.texture = LoadTexture("resources/redBoom.png");

        boom.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)boom.sprite.texture.width/4, (float)boom.sprite.texture.height };
        boom.sprite.currentFrame = 0;
        boom.sprite.framesCounter = 0;
        boom.sprite.framesSpeed = 8;
        boom.speed = 0.0f;
        boom.permanent = 0;
        return boom;
    }
}

void UpdateBalls(Entity balls[], int n, Entity *player, int *score, Sound soundA, Sound soundB, Sound soundC)
{
    for (int i = 0; i < n; i++)
    {
        if (CheckCollisionCircles(balls[i].position, 10, player->position, 10) && balls[i].permanent)
        {
            UnloadTexture(balls[i].sprite.texture);
            int permanent = balls[i].permanent;
            Vector2 pos = (Vector2){ balls[i].position.x, balls[i].position.y };
            char type = balls[i].type;
            balls[i] = CreateBall(permanent, type, pos);
            if (type == 'b')
            {
                (*score)++;
                PlaySoundMulti(soundA);
            }
            else if (type == 'g')
            {
                player->speed = 150;
                PlaySoundMulti(soundB);
            }
            else
            {
                PlaySoundMulti(soundC);
                (*score) -= 5;
            }

        }
        else
        {
            bool ballFinishedPopping;
            ballFinishedPopping = UpdateBallSprite(&(balls[i].sprite), balls[i].permanent);
            if (ballFinishedPopping)
            {
                UnloadTexture(balls[i].sprite.texture);
                int permanent = balls[i].permanent;
                Vector2 pos = (Vector2){ -1.0f, -1.0f };
                char type = 'n';
                balls[i] = CreateBall(permanent, type, pos);
            }
		}
	}
}

