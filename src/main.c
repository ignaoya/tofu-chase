#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "raylib.h"
#include "raymath.h"

#define PLAYER_SPEED	  150
#define MAX_BALLS          20

typedef struct Sprite {
	Texture2D texture;
	Rectangle frameRec;
	int currentFrame;
	int framesCounter;
	int framesSpeed;
} Sprite;

typedef struct Entity {
	Vector2 position;
	Sprite sprite;
	float speed;
	int permanent;	// bool type to indicate if sprite should be permanent or disappear after anim cycle
} Entity;

void ClearEverything(Entity player, Entity enemy, Entity balls[]);
Entity CreatePlayer(void);
Entity CreateEnemy(void);
void UpdatePlayer(Entity *entity, float delta);
void UpdateEnemy(Entity *enemy, Entity *player, float delta);
void UpdateSpriteFrame(Sprite *sprite);
Entity CreateBall(int permanent, Vector2 pos);
void UpdateBalls(Entity balls[], int n, Entity *player, int *score, Sound sound);
int UpdateBallSprite(Sprite *sprite, int permanent);
Vector2 FindFollowPath(Vector2 posA, Vector2 posB);
int CheckGameOver(Entity *player, Entity *enemy, int gameOver, Sound waca);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

	srand(time(0));
    
    InitWindow(screenWidth, screenHeight, "The Tower");

	InitAudioDevice();
	Music music = LoadMusicStream("resources/tower.wav"); 
	Sound pop = LoadSound("resources/bubblePop.wav");
	Sound waca = LoadSound("resources/waca.wav");
	PlayMusicStream(music);

	int score = 0;
	char scoreString[50];

	Entity player = CreatePlayer();

	// Enemy creation
	// -------------------------------------------------------------------------------------
	
	Entity enemy = CreateEnemy();

	// Balls creation
	// -------------------------------------------------------------------------------------
	
	Entity balls[MAX_BALLS] = {};
	for (int i = 0; i < MAX_BALLS; i++)
	{
		balls[i] = CreateBall(0, (Vector2){ -1.0, -1.0});
	}

    SetTargetFPS(60);
	int gameOver = 0;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) 
    {
        // Update
        //----------------------------------------------------------------------------------
		//
		UpdateMusicStream(music);


		if (CheckGameOver(&player, &enemy, gameOver, waca))
		{
			// Check for Restart
			// ---------------------------------------------------------------------------------

			if (IsKeyPressed(KEY_SPACE))
			{
				ClearEverything(player, enemy, balls);

				score = 0;

				player = CreatePlayer();

				// Enemy creation
				// -------------------------------------------------------------------------------------
				
				enemy = CreateEnemy();

				// Balls creation
				// -------------------------------------------------------------------------------------
				
				for (int i = 0; i < MAX_BALLS; i++)
				{
					balls[i] = CreateBall(0, (Vector2){ -1.0, -1.0});
				}

				SetTargetFPS(60);
				gameOver = 0;
			}
			else
			{

				// Draw
				//----------------------------------------------------------------------------------
				gameOver = 1;
				BeginDrawing();

					ClearBackground(LIGHTGRAY);
					DrawText("GAME OVER", 150, 200, 80, BLACK);
					DrawText("SCORE:", 250, 300, 40, BLACK);
					DrawText(scoreString, 425, 300, 40, BLACK);

				EndDrawing();
			}
		}
		else
		{
			float deltaTime = GetFrameTime();
			UpdatePlayer(&player, deltaTime);
			UpdateEnemy(&enemy, &player, deltaTime);
			UpdateBalls(balls, MAX_BALLS, &player, &score, pop);

			sprintf(scoreString, "%i", score);
			// Draw
			//----------------------------------------------------------------------------------
			BeginDrawing();

				ClearBackground(LIGHTGRAY);

				DrawText("SCORE:", 625, 20, 20, BLACK);
				DrawText(scoreString, 700, 20, 20, BLACK);

				DrawTextureRec(enemy.sprite.texture, enemy.sprite.frameRec, enemy.position, WHITE);
				DrawTextureRec(player.sprite.texture, player.sprite.frameRec, player.position, WHITE);
				
				for (int i = 0; i < MAX_BALLS; i++)
				{
					DrawTextureRec(balls[i].sprite.texture, balls[i].sprite.frameRec, balls[i].position, WHITE);
				}
				
			EndDrawing();
		}
			//----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
	
	ClearEverything(player, enemy, balls);
	StopSoundMulti();
	UnloadSound(pop);
	UnloadSound(waca);
	UnloadMusicStream(music);
	CloseAudioDevice();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

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
}

void UpdateEnemy(Entity *enemy, Entity *player, float delta)
{
	if (!CheckCollisionCircles(enemy->position, 10, player->position, 10))
	{ 
		Vector2 direction = FindFollowPath(enemy->position, player->position);
		enemy->position.x -= direction.x * enemy->speed * delta;
		enemy->position.y -= direction.y * enemy->speed * delta;

		UpdateSpriteFrame(&(enemy->sprite));
	}
	else
	{
		enemy->sprite.currentFrame = 0;
		enemy->sprite.frameRec.y = (float)enemy->sprite.currentFrame*(float)enemy->sprite.texture.height/8;
	}
}

void UpdateBalls(Entity balls[], int n, Entity *player, int *score, Sound sound)
{
	for (int i = 0; i < n; i++)
	{
		if (CheckCollisionCircles(balls[i].position, 10, player->position, 10) && balls[i].permanent)
		{
			(*score)++;
			PlaySoundMulti(sound);
			UnloadTexture(balls[i].sprite.texture);
			int permanent = balls[i].permanent;
			Vector2 pos = (Vector2){ balls[i].position.x, balls[i].position.y };
			balls[i] = CreateBall(permanent, pos);
		}
		else
		{
			int cleanUp;
			cleanUp = UpdateBallSprite(&(balls[i].sprite), balls[i].permanent);
			if (cleanUp)
			{
				UnloadTexture(balls[i].sprite.texture);
				int permanent = balls[i].permanent;
				Vector2 pos = (Vector2){ -1.0f, -1.0f };
				balls[i] = CreateBall(permanent, pos);
			}
		}
	}
}

// Returns bool flag that indicates whether the animation of a temporal sprite (i.e. an explosion) has finished and needs to be 
// unloaded
int UpdateBallSprite(Sprite *sprite, int permanent)
{
		sprite->framesCounter++;

		if (sprite->framesCounter >= (60/sprite->framesSpeed))
		{
			sprite->framesCounter = 0;
			sprite->currentFrame++;

			if (sprite->currentFrame > 4)
			{
				if (permanent)
				{
					sprite->currentFrame = 0;
				}
				else
				{
					return 1;
				}
			}
			sprite->frameRec.x = (float)sprite->currentFrame*(float)sprite->texture.width/4;
			return 0;
		}
		return 0;
}


void UpdateSpriteFrame(Sprite *sprite)
{
		sprite->framesCounter++;

		if (sprite->framesCounter >= (60/sprite->framesSpeed))
		{
			sprite->framesCounter = 0;
			sprite->currentFrame++;

			if (sprite->currentFrame > 4 || sprite->currentFrame == 0) sprite->currentFrame = 1;

			sprite->frameRec.y = (float)sprite->currentFrame*(float)sprite->texture.height/8;
		}
}

Vector2 FindFollowPath(Vector2 posA, Vector2 posB)
{
	Vector2 d = (Vector2){ posA.x - posB.x, posA.y - posB.y };
	float D = Vector2Distance(posA, posB);
	Vector2 n = (Vector2){ d.x / D, d.y / D };
	return n;
}

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

Entity CreatePlayer(void)
{
	Entity player = { 0 };
	player.position = (Vector2){350.0f, 280.0f};
	player.sprite.texture = LoadTexture("resources/tofu.png");	// Texture Loading
	player.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)player.sprite.texture.width, (float)player.sprite.texture.height/8 };
	player.sprite.currentFrame = 1;
	player.sprite.framesCounter = 0;
	player.sprite.framesSpeed = 8;
	player.speed = 250.0f;
	player.permanent = 1;

	return player;
}

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
	enemy.sprite.texture = LoadTexture("resources/bufo.png");	// Texture Loading
	enemy.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)enemy.sprite.texture.width, (float)enemy.sprite.texture.height/8 };
	enemy.sprite.currentFrame = 1;
	enemy.sprite.framesCounter = 0;
	enemy.sprite.framesSpeed = 8;
	enemy.speed = 200.0f;
	enemy.permanent = 1;

	return enemy;
}

Entity CreateBall(int permanent, Vector2 position)
{
	if (!permanent)
	{
		int lower = 10, upperHeight = 550, upperWidth = 750;
		Entity ball = { 0 };
		ball.position = (Vector2){(float)((rand() % (upperWidth - lower + 1)) + lower), (float)((rand() % (upperHeight - lower + 1)) + lower)};
		ball.sprite.texture = LoadTexture("resources/blue-ball.png");	// Texture Loading
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
		boom.sprite.texture = LoadTexture("resources/boom.png");
		boom.sprite.frameRec = (Rectangle){ 0.0f, 0.0f, (float)boom.sprite.texture.width/4, (float)boom.sprite.texture.height };
		boom.sprite.currentFrame = 0;
		boom.sprite.framesCounter = 0;
		boom.sprite.framesSpeed = 8;
		boom.speed = 0.0f;
		boom.permanent = 0;
		return boom;
	}
}