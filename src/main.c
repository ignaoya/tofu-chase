#include "game.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WIDTH;
    const int screenHeight = HEIGHT;

	srand(time(0));
    
    InitWindow(screenWidth, screenHeight, "The Tower");

	InitAudioDevice();
	Music music = LoadMusicStream("resources/tower.wav"); 
	Sound pop = LoadSound("resources/bubblePop.wav");
	Sound pft = LoadSound("resources/bubblePft.wav");
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
	
	Entity balls[MAX_BALLS] = { 0 };
	for (int i = 0; i < MAX_BALLS; i++)
	{
		balls[i] = CreateBall(0, 'n', (Vector2){ -1.0, -1.0});
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
					balls[i] = CreateBall(0, 'n', (Vector2){ -1.0, -1.0});
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
			UpdateBalls(balls, MAX_BALLS, &player, &score, pop, pft, waca);

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
	UnloadSound(pft);
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

// Returns bool flag that indicates whether the animation of a temporal sprite (i.e. an explosion) has finished and needs to be 
// unloaded
bool UpdateBallSprite(Sprite *sprite, int permanent)
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
					return true;
				}
			}
			sprite->frameRec.x = (float)sprite->currentFrame*(float)sprite->texture.width/4;
			return false;
		}
		return false;
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
	enemy.type = 'e';

	return enemy;
}

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
			ball.sprite.texture = LoadTexture("resources/blue-ball.png");	// Texture Loading
			ball.type = 'b';
		}
		else if (typeRandom < 10)
		{
			ball.sprite.texture = LoadTexture("resources/green-ball.png");	// Texture Loading
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
