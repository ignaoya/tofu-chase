#include "game.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WIDTH;
    const int screenHeight = HEIGHT;

	srand(time(0));
    
    InitWindow(screenWidth, screenHeight, "Tofu Chase");

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
	bool gameOver = false;
	bool intro = true;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) 
    {
        // Update
        //----------------------------------------------------------------------------------
		//
		UpdateMusicStream(music);



		if (intro)
		{
			BeginDrawing();

				ClearBackground(LIGHTGRAY);
				DrawText("Tofu Chase", 150, 200, 80, BLACK);
				DrawText("a game by Ignacio Oyarzabal", 250, 280, 20, BLACK);
				DrawText("with art from 'Tofuman' by Emcee Flesher", 190, 300, 20, BLACK);
				DrawText("Press SPACE to Start", 150, 500, 40, BLACK);

			EndDrawing();
			if (IsKeyPressed(KEY_SPACE))
				intro = false;
		}
		else if (CheckGameOver(&player, &enemy, gameOver, waca))
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
