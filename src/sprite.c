#include "game.h"


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

