#ifndef OPENGINE_PIPELINE_SPRITE2D
#define OPENGINE_PIPELINE_SPRITE2D

#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/Timer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"

typedef struct {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPint Direction;
	OPsprite** Sprites;
	OPsprite* CurrentSprite;
	OPfloat FrameRate;
	OPint Loop;
	OPeffect* Effect;
} OPsprite2D;

void OPsprite2DInit(OPeffect* effect);
OPsprite2D* OPsprite2DCreate(OPsprite** sprites, OPeffect* effect);
void OPsprite2DDestroy(OPsprite2D* sprite);
void OPsprite2DUpdate(OPsprite2D* sprite, OPtimer* timer);
void OPsprite2DSetSprite(OPsprite2D* sprite, i32 index);
void OPsprite2DRender(OPsprite2D* sprite);


#endif