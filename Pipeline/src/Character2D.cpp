#include "./Data/include/ContentManager.h"
#include "./Pipeline/include/Character2D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"
#include "./Human/include/Rendering/Renderer.h"

int CHARACTER_2D_INITIALIZED = 0;
OPmesh CHARACTER_2D_QUAD_MESH;
OPeffect* EFFECT_CHARACTER_2D;

void OPcharacter2DInit(OPeffect* effect) {
	CHARACTER_2D_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_CHARACTER_2D = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_CHARACTER_2D = OPrenderBuildEffect(
			"Common/OPspriteSheet.vert",
			"Common/OPspriteSheet.frag",
			OPATTR_POSITION | OPATTR_UV,
			"Sprite sheet effect"
			);

		CHARACTER_2D_INITIALIZED = 2;
	}
	else {
		EFFECT_CHARACTER_2D = effect;
		CHARACTER_2D_INITIALIZED = 1;
	}
}

OPcharacter2D* OPcharacter2DCreate(OPsprite** sprites, OPeffect* effect) {
	OPcharacter2D* character = (OPcharacter2D*)OPalloc(sizeof(OPcharacter2D));
	OPbzero(character, sizeof(OPcharacter2D));
	character->Scale = OPvec2One;
	character->Sprites = sprites;
	character->CurrentSprite = sprites[0];
	character->Direction = 1;
	character->FrameRate = 24.0f;
	character->Loop = 1;
	character->Effect = effect;
	if (effect == NULL) {
		character->Effect = EFFECT_CHARACTER_2D;
	}
	return character;
}

void OPcharacter2DDestroy(OPcharacter2D* character) {
	OPfree(character);
}

void OPcharacter2DUpdate(OPcharacter2D* character, OPtimer* timer) {
	character->CurrentSprite->Elapsed += timer->Elapsed;
	if (character->CurrentSprite->Elapsed > 1000 / character->FrameRate) {
		character->CurrentSprite->Elapsed = 0;
		character->CurrentSprite->Frame++;
		if (character->CurrentSprite->Frame >= character->CurrentSprite->FrameCount) {
			if (character->Loop) character->CurrentSprite->Frame = 0;
			else  character->CurrentSprite->Frame--;
		}
	}
}

void OPcharacter2DSetSprite(OPcharacter2D* character, i32 sprite) {
	character->CurrentSprite = character->Sprites[sprite];
	character->CurrentSprite->Frame = 0;
	character->CurrentSprite->Elapsed = 0;
}

void OPcharacter2DRender(OPcharacter2D* character) {
	OPrenderBindMesh(&CHARACTER_2D_QUAD_MESH);
	OPrenderBindEffect(character->Effect);


	OPvec2 frameSize = OPspriteCurrentFrameSize(character->CurrentSprite);
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPrenderDepth(0);

	OPmat4 world;
	OPmat4identity(&world);
	OPmat4rotZ(&world, character->Rotation);
	OPvec2 scl = character->Scale / 2.0;
	scl.x *= character->Direction * widthScale * OPrenderGetWidthAspectRatio();
	scl.y *= heightScale;
	world *= scl;
	world += character->Position;

	OPtextureClearActive();
	OPtextureSmooth();
	OPrenderParami("uColorTexture", OPtextureBind(character->CurrentSprite->Sheet));
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamVec2("uOffset", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Offset);
	OPrenderParamVec2("uSize", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size);
	OPrenderMesh();
}





