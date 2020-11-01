#ifndef GAME_H
#define GAME_H

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "raylib.h"
#include "raymath.h"


// Global Constants
//---------------------------------------------------------------------------------------
#define WIDTH             800
#define HEIGHT            600
#define MAX_BALLS          20


// Struct Definitions
//---------------------------------------------------------------------------------------
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
    int permanent;  // bool to indicate if sprite should be permanent or disappear after anim cycle
    char type;      // char type to indicate which type of Entity it is, and especially which type of ball
} Entity;


// Player functions
//---------------------------------------------------------------------------------------
Entity CreatePlayer(void);
void UpdatePlayer(Entity *entity, float delta);

// Enemy functions
//---------------------------------------------------------------------------------------
Entity CreateEnemy(void);
void UpdateEnemy(Entity *enemy, Entity *player, float delta);
Vector2 FindPath(Vector2 posA, Vector2 posB);

// Ball functions
//---------------------------------------------------------------------------------------
Entity CreateBall(int permanent, char type, Vector2 pos);
void UpdateBalls(Entity balls[], int n, Entity *player, int *score, Sound soundA, Sound soundB, Sound soundC);

// Sprite functions
//---------------------------------------------------------------------------------------
void UpdateSpriteFrame(Sprite *sprite);
bool UpdateBallSprite(Sprite *sprite, int permanent);

// GameOver Functions
//---------------------------------------------------------------------------------------
bool CheckGameOver(Entity *player, Entity *enemy, bool gameOver, Sound waca);
void ClearEverything(Entity player, Entity enemy, Entity balls[]);

#endif
