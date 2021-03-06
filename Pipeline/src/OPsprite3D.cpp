
#include "./Pipeline/include/OPsprite3D.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Rendering/OPrender.h"

int SPRITE_3D_INITIALIZED = 0;
OPmodel* SPRITE_3D_QUAD_MESH;
OPeffect* EFFECT_SPRITE_3D;

void OPsprite3DInit(OPeffect* effect) {
	SPRITE_3D_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_SPRITE_3D = (OPeffect*)OPalloc(sizeof(OPeffect));
		EFFECT_SPRITE_3D->Init("Common/OPspriteSheet3D.vert", "Common/OPspriteSheet.frag");

		SPRITE_3D_INITIALIZED = 2;
	}
	else {
		EFFECT_SPRITE_3D = effect;
		SPRITE_3D_INITIALIZED = 1;
	}
}

OPsprite3D* OPsprite3DCreate(OPsprite** sprites, OPeffect* effect) {
	OPsprite3D* sprite = (OPsprite3D*)OPalloc(sizeof(OPsprite3D));
	OPbzero(sprite, sizeof(OPsprite3D));
	sprite->Scale = OPVEC3_ONE;
	sprite->Sprites = sprites;
	sprite->CurrentSprite = sprites[0];
	sprite->Direction = 1;
	sprite->FrameRate = 24.0f;
	sprite->Loop = 1;
	sprite->LoopsCompleted = 0;
	sprite->Effect = effect;
	if (effect == NULL) {
		sprite->Effect = EFFECT_SPRITE_3D;
	}
	return sprite;
}

void OPsprite3DCreateFill(OPsprite3D* sprite, OPsprite** sprites, OPeffect* effect) {
	OPbzero(sprite, sizeof(OPsprite3D));
	sprite->Scale = OPVEC3_ONE;
	sprite->Sprites = sprites;
	sprite->CurrentSprite = sprites[0];
	sprite->Direction = 1;
	sprite->FrameRate = 24.0f;
	sprite->Loop = 1;
	sprite->LoopsCompleted = 0;
	sprite->Effect = effect;
	if (effect == NULL) {
		sprite->Effect = EFFECT_SPRITE_3D;
	}
}

void OPsprite3DDestroy(OPsprite3D* sprite) {
	OPfree(sprite);
}

void OPsprite3DUpdate(OPsprite3D* sprite, ui64 elapsed) {
	sprite->CurrentElapsed += elapsed;
	if (sprite->CurrentElapsed > 1000 / sprite->FrameRate) {
		sprite->CurrentElapsed = 0;
		sprite->CurrentFrame++;
		if (sprite->CurrentFrame >= sprite->CurrentSprite->FrameCount) {
			if (sprite->Loop) {
				sprite->CurrentFrame = 0;
				sprite->LoopsCompleted++;
			}
			else  {
				sprite->CurrentFrame--;
			}
		}
	}
}

void OPsprite3DSetSprite(OPsprite3D* sprite, i32 index, bool force) {
	if (sprite->CurrentSprite == sprite->Sprites[index] && !force) return;
	sprite->CurrentSprite = sprite->Sprites[index];
	sprite->CurrentFrame = 0;
	sprite->CurrentElapsed = 0;
	sprite->LoopsCompleted = 0;
}

void OPsprite3DPrepReRender(OPsprite3D* sprite, OPvec3 offset, OPfloat rotation) {
	sprite->CurrentSprite->Frame = sprite->CurrentFrame;
	OPvec2 frameSize = sprite->CurrentSprite->FrameSize();
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPmat4 world;
	OPmat4Identity(&world);
	world.Translate(sprite->Position);
	//world = OPmat4Scl(world, widthScale, heightScale, 1.0);
	//OPmat4RotZ(&world, rotation + sprite->Rotation.z);
	//OPmat4RotY(&world, sprite->Rotation.y);
	//OPvec3 scl = sprite->Scale / 2.0f;
	//scl.x *= sprite->Direction;
	//world = OPmat4Scl(world, scl.x, scl.y, 1.0);
	//world += offset + sprite->Position;

	OPeffectSet("uWorld", &world);
	OPeffectSet("uOffset", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset);
	OPeffectSet("uSize", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size);
}

void OPsprite3DPrepRender(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation) {
	OPvec2 frameSize = sprite->CurrentSprite->FrameSize();
	//offset.x += sprite->CurrentSprite->Frames[sprite->CurrentSprite->Frame].Size.x / 2.0f;
	//offset.y += sprite->CurrentSprite->Frames[sprite->CurrentSprite->Frame].Size.y / 2.0f;
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPmat4 world;
	SPRITE_3D_QUAD_MESH->Bind();
	sprite->Effect->Bind();

	OPmat4Identity(&world);
	world.Translate(sprite->Position);
	world.Scl(sprite->Scale * OPvec3(widthScale, heightScale, 1.0));
	//world = OPmat4Scl(world, widthScale, heightScale, 1.0);
	//OPmat4RotZ(&world, rotation + sprite->Rotation.z);
	//OPmat4RotY(&world, sprite->Rotation.y);
	//OPvec3 scl = sprite->Scale / 2.0f;
	//scl.x *= sprite->Direction;
	//world = OPmat4Scl(world, scl.x * frameSize.x, scl.y * frameSize.y, 1.0);
	//world += offset + sprite->Position + OPvec3(0, sprite->Scale.y * (frameSize.y / 2.0), 0);

	//OPtextureClearActive();
	//ui32 bind = OPtextureBind(sprite->CurrentSprite->Sheet);

	//OPlog("SpriteSheet %d @ %x", bind, sprite->CurrentSprite->Sheet);
	OPeffectSet("uColorTexture", sprite->CurrentSprite->SpriteSheet->Texture, 0);
	//OPeffectParamf("uAlpha", 1.0f);
	OPeffectSet("uWorld", &world);
	OPeffectSet("uView", &camera->view);
	OPeffectSet("uProj", &camera->proj);
    OPvec2 frameOffsetScl = sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset;
    OPvec2 frameSizeScl = sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size;
    //frameOffsetScl = OPVEC2_ZERO;
    //frameSizeScl = OPVEC2_ONE;
	OPeffectSet("uOffset", &frameOffsetScl);
	OPeffectSet("uSize", &frameSizeScl);
}

void OPsprite3DRender(OPsprite3D* sprite, OPcam* camera) {
	OPsprite3DPrepRender(sprite, camera, OPVEC3_ZERO, 0);
	OPrenderDrawBufferIndexed(0);
}

void OPsprite3DRenderOffsetRot(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation) {
	OPsprite3DPrepRender(sprite, camera, offset, rotation);
	OPrenderDrawBufferIndexed(0);
}


OPvec2 OPsprite3DSize(OPsprite3D* sprite) {
	OPvec2 frameSize = sprite->CurrentSprite->FrameSize();

	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	f32 width = sprite->Scale.x * widthScale;
	f32 height = sprite->Scale.y * heightScale;
	return OPvec2(width, height);
}
