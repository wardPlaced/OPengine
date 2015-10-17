#include "./Pipeline/include/OPspriteSystem.h"
#include "./Core/include/Assert.h"

void OPspriteSystemEffectDefault(OPeffect* effect) {
	*effect = OPeffectGen(
		"Common/OPspriteSheet.vert",
		"Common/OPspriteSheet.frag",
		OPATTR_POSITION | OPATTR_UV,
		"Sprite sheet effect",
		0
		);
}

void OPspriteSystemInit(OPspriteSystem* system, OPsprite** sprites, OPint count, OPeffect* effect, OPspriteSystemAlign alignment) {
	if(effect == NULL) {
		system->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		OPspriteSystemEffectDefault(system->Effect);
		system->ExternalEffect = 0;
	} else {
		system->Effect = effect;
		system->ExternalEffect = 1;
	}
	system->SystemSprites = (OPspriteSystemSprite*)OPallocZero(sizeof(OPspriteSystemSprite)* count);
	system->Count = 0;
	system->Sprites = sprites;
	system->CountMax = count;
	system->FrameRate = 24.0f;

	switch (alignment) {
	case OPSPRITESYSTEMALIGN_CENTER:
		system->_mesh = OPquadCreate(0.5f, 0.5f);
		break;
	default:
		system->_mesh = OPquadCreate(0.5f, 0.5f, OPvec2Create(0.0, 0.5));
	}

	for (OPuint i = 0; i < count; i++) {
		system->SystemSprites[i].Scale = OPVEC2_ONE;
		system->SystemSprites[i].Direction = 1;
	}
}

void OPspriteSystemDestroy(OPspriteSystem* system) {
	//OPeffectDestroy(system->Effect);
	OPfree(system->Effect);
	OPfree(system->SystemSprites);
	//OPmeshDestroy(system->_mesh);
}

OPspriteSystem* OPspriteSystemCreate(OPsprite** sprites, OPint count, OPeffect* effect, OPspriteSystemAlign alignment) {
	OPspriteSystem* result = (OPspriteSystem*)OPalloc(sizeof(OPspriteSystem));
	OPspriteSystemInit(result, sprites, count, effect, alignment);
	return result;
}

OPspriteSystemSprite* OPspriteSystemAdd(OPspriteSystem* system) {
	ASSERT(system->Count != system->CountMax, "Filled up the Sprite System");
	OPspriteSystemSprite* result = &system->SystemSprites[system->Count];
	system->Count++;
	return result;
}

void OPspriteSystemRemove(OPspriteSystem* system, OPspriteSystemSprite* sprite) {
	ASSERT(system->Count != 0, "Nothing to remove in Sprite System");
	if (system->Count > 1) {
		for (OPuint i = 0; i < system->Count - 1; i++) {
			if (&system->SystemSprites[i] == sprite) {
				system->SystemSprites[i] = system->SystemSprites[system->Count - 1];
				break;
			}
		}
	}
	system->Count--;
}

void OPspriteSystemUpdate(OPspriteSystem* system, OPtimer* timer) {
	OPspriteSystemSprite* sprite;
	for (OPuint i = 0; i < system->Count; i++) {
		sprite = &system->SystemSprites[i];

		sprite->CurrentElapsed += timer->Elapsed;
		if (sprite->CurrentElapsed > 1000 / system->FrameRate) {
			sprite->CurrentElapsed = 0;
			sprite->CurrentFrame++;
			if (sprite->CurrentFrame >= system->Sprites[sprite->CurrentSprite]->FrameCount) {
				sprite->CurrentFrame = 0;
			}
		}
	}
}

void OPspriteSystemRender(OPspriteSystem* system, OPcam* cam) {
	if (system->Count == 0) return;

	OPsprite* currentSprite;
	OPmat4 world, proj, view;
	view = OPmat4Translate(cam->_pos * -1);
	OPcamGetProj((*cam), &proj);

	OPmeshBind(&system->_mesh);
	OPeffectBind(system->Effect);

	OPtextureClearActive();
	OPeffectParami("uColorTexture", OPtextureBind(system->Sprites[0]->Sheet));
	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);

	for (OPuint i = 0; i < system->Count; i++) {
		currentSprite = system->Sprites[system->SystemSprites[i].CurrentSprite];
		OPvec2 frameSize = OPspriteCurrentFrameSize(currentSprite);

		//world = // OPmat4Translate((frameSize.x / 2.0) *system->SystemSprites[i].Scale.x, (frameSize.y / 2.0) *system->SystemSprites[i].Scale.y, 0);
		world = OPmat4Scl(frameSize.x * system->SystemSprites[i].Direction, frameSize.y, 0);
		world *= OPmat4Scl(system->SystemSprites[i].Scale.x, system->SystemSprites[i].Scale.y, 1);
		world += system->SystemSprites[i].Position;
		world *= OPmat4RotZ(system->SystemSprites[i].Rotation);

		OPeffectParamMat4("uWorld", &world);
		OPeffectParamVec2("uOffset", &currentSprite->Frames[system->SystemSprites[i].CurrentFrame].Offset);
		OPeffectParamVec2("uSize", &currentSprite->Frames[system->SystemSprites[i].CurrentFrame].Size);

		OPmeshRender();
	}
}