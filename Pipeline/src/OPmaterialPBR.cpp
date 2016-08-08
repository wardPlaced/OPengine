#include "./Pipeline/include/OPmaterialPBR.h"

void OPmaterialPBR::Init(OPeffect* effect) {
	lightColor = OPvec4(1, 1, 1, 1);
	lightDirection = OPvec3(1, 1, 1);
	lightDirection.Norm();
	lightIntensity = 1.0;

	rootMaterial.Init(effect);
	rootMaterial.AddParam("uLight.color", &lightColor);
	rootMaterial.AddParam("uLight.direction", &lightDirection);
	rootMaterial.AddParam("uLight.intensity", &lightIntensity);
}

OPmaterialPBRInstance* OPmaterialPBR::CreateInstance() {
	OPmaterialPBRInstance* result = OPNEW(OPmaterialPBRInstance(this));
	return result;
}

inline void OPmaterialPBRInstance::Init(OPmaterialPBR* material) {
	rootMaterial = material;
	rootMaterialInstance.Init(&material->rootMaterial);

	albedoUsage = 0.0f;
	specularUsage = 0.0f;
	glossUsage = 0.0f;
	normalUsage = 0.0f;

	rootMaterialInstance.AddParam("uUsingAlbedoMap", &albedoUsage);
	rootMaterialInstance.AddParam("uUsingSpecularMap", &specularUsage);
	rootMaterialInstance.AddParam("uUsingGlossMap", &glossUsage);
	rootMaterialInstance.AddParam("uUsingNormalMap", &normalUsage);

	albedoColor = OPvec4(0, 0, 0, 1);
	normalColor = OPvec3(1);
	glossColor = 0.8f;
	specularColor = OPvec3(1, 1, 1);

	rootMaterialInstance.AddParam("uAlbedoColor", &albedoColor);
	//entity->material->AddParam("uNormalColor", &sceneExample.normalColor);
	rootMaterialInstance.AddParam("uGlossColor", &glossColor);
	rootMaterialInstance.AddParam("uSpecularColor", &specularColor);
}