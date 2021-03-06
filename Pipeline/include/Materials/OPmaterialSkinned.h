#pragma once

class OPmaterialSkinned;

#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

class OPmaterialSkinned : public OPmaterial {
public:
	OPvec3 lightPos;

	OPmaterialSkinned() { }
	OPmaterialSkinned(OPmaterial* material) : OPmaterial(material) {
	}
	OPmaterialSkinned(OPeffect* effect) {
		Init(effect);
	}

	inline OPmaterial* Base() { return rootMaterial; }

	inline void SetCameraPos(OPcam* camera) {
		rootMaterial->AddParam("uCamPos", &camera->pos);
	}

	inline void SetLightPos(OPvec3 pos) {
		lightPos = pos;
	}

	inline void SetBones(OPskeleton* skeleton) {
		AddParam("uBones", skeleton->skinned, (ui8)skeleton->hierarchyCount);
	}

	inline void SetAlbedoMap(OPtexture* texture) {
		AddParam("uAlbedoMap", texture, 0);
	}
	
	inline void SetAlbedoMap(const OPchar* texture) {
		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
	}

	OPmaterialSkinned* Init(OPmaterial* material);
	OPmaterialSkinned* Init(OPeffect* effect);

	//OPmaterialSkinnedInstance* CreateInstance();
	//OPmaterialSkinned** CreateInstances(OPmodel* model, bool setMeta, bool materialPerMesh);
};

//
//struct OPmaterialSkinnedInstance {
//	OPmaterial rootMaterialInstance;
//	OPmaterialSkinned* rootMaterial;
//
//	OPmaterialSkinnedInstance() { }
//	OPmaterialSkinnedInstance(OPmaterialSkinned* material) {
//		Init(material);
//	}
//	OPmaterialSkinnedInstance(OPmaterialSkinnedInstance* instance) {
//		Init(instance->rootMaterial);
//
//		OPmaterialParam* param = NULL;
//		param = instance->GetParam("uAlbedoMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//	}
//
//	void Init(OPmaterialSkinned* material);
//
//	inline OPmaterialInstance* Base() {
//		return &rootMaterialInstance;
//	}
//
//	inline OPmaterialParam* GetParam(const OPchar* name) {
//		return rootMaterialInstance.GetParam(name);
//	}
//
//	inline void SetAlbedoMap(OPtexture* texture) {
//		rootMaterialInstance.AddParam("uAlbedoMap", texture, 0);
//	}
//
//	inline void SetAlbedoMap(const OPchar* texture) {
//		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//
//  inline void SetBones(OPskeleton* skeleton) {
//		rootMaterialInstance.AddParam("uBones", skeleton->skinned, skeleton->hierarchyCount);
//  }
//
//	inline OPmaterialSkinnedInstance* Clone() {
//		return OPNEW(OPmaterialSkinnedInstance(this));
//	}
//};
