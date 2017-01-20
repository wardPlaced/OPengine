#include "./Human/include/Rendering/OPrendererDeferred.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

void OPrendererDeferredInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;

	renderer->camera = camera;

	deferredRenderer->defaultGBufferEffect = OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag"));
	deferredRenderer->defaultGBufferMaterial = OPNEW(OPmaterial(deferredRenderer->defaultGBufferEffect));
	deferredRenderer->passes[0] = deferredRenderer->defaultGBufferMaterial;
	
	deferredRenderer->defaultLightingEffect = OPNEW(OPeffect("Common/DeferredLighting.vert", "Common/DeferredLighting.frag"));
	deferredRenderer->defaultLightingMaterial = OPNEW(OPmaterial(deferredRenderer->defaultLightingEffect));
	deferredRenderer->passes[1] = deferredRenderer->defaultLightingMaterial;
	deferredRenderer->defaultLightingMaterial->depth = false;
	deferredRenderer->defaultLightingMaterial->cull = false;

	deferredRenderer->renderBucket[0].Init(maxCalls, renderer->camera);
	deferredRenderer->renderBucket[1].Init(maxLights, renderer->camera);
	deferredRenderer->spotLights = OPALLOC(OPlightSpot, maxLights);
	deferredRenderer->spotLightsInd = 0;

	OPtextureDesc gBufferDesc[3];
	// Position
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	//gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Normal
	//gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB16F, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	deferredRenderer->gBuffer.Init(gBufferDesc, 3);

	OPtextureDesc lightBufferDec = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	deferredRenderer->lightBuffer.Init(lightBufferDec, &deferredRenderer->gBuffer.depthTexture);

	// Directional light
	deferredRenderer->defaultLightingMaterialInstance = OPNEW(OPmaterial(deferredRenderer->defaultLightingMaterial));

	// Spot light
	deferredRenderer->defaultLightingSpotEffect = OPNEW(OPeffect("Common/DeferredLightingSpot.vert", "Common/DeferredLightingSpot.frag"));
	deferredRenderer->defaultLightingSpotMaterial = OPNEW(OPmaterial(deferredRenderer->defaultLightingSpotEffect));
	deferredRenderer->defaultLightingSpotMaterial->depth = true;
	deferredRenderer->defaultLightingSpotMaterialInstance = OPNEW(OPmaterial(deferredRenderer->defaultLightingSpotMaterial));
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uInvertViewProjection", &deferredRenderer->invertViewProjection);
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferDepth", &deferredRenderer->gBuffer.depthTexture, 0);
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 1);

	// Combine Material
	deferredRenderer->defaultCombineEffect = OPNEW(OPeffect("Common/DeferredCombine.vert", "Common/DeferredCombine.frag"));
	deferredRenderer->defaultCombineMaterial = OPNEW(OPmaterial(deferredRenderer->defaultCombineEffect));
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);
	deferredRenderer->defaultCombineMaterial->AddParam("uLightBuffer", deferredRenderer->lightBuffer.texture, 3);
	deferredRenderer->defaultCombineMaterial->depth = false;
	deferredRenderer->defaultCombineMaterial->cull = false;


	//deferredRenderer->sphereMesh = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::UV));
	deferredRenderer->quadMesh = OPquadCreate(1.0f, 1.0f);
	deferredRenderer->sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
}

OPmaterial* OPrendererDeferredCreateMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	OPmaterial* result = OPNEW(OPmaterial(deferredRenderer->passes[0]));
	return result;
}

OPmaterial* OPrendererDeferredGetMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	return deferredRenderer->passes[pass];
}

void OPrendererDeferredSetMaterial(OPrenderer* renderer, OPmaterial* material, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->passes[pass] = material;
}

void OPrendererDeferredSetMaterials(OPrenderer* renderer, OPrendererEntity* entity) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	//if (!entity->desc.animated) {
		entity->material = (OPmaterial*)OPNEW(OPmaterial(deferredRenderer->passes[0]));
		//entity->shadowMaterial = deferredRenderer->shadowMaterial.CreateInstances(entity->model, entity->desc.materialPerMesh);
	//}
	//else {
	//	ui32 count = entity->model->meshCount;
	//	if (!entity->desc.materialPerMesh) {
	//		count = 1;
	//	}
	//	entity->material = OPALLOC(OPmaterialInstance*, count);

	//	OPmaterialSkinnedInstance** instances = deferredRenderer->skinnedMaterial.CreateInstances(entity->model, true, entity->desc.materialPerMesh);
	//	for (ui32 i = 0; i < count; i++) {
	//		entity->material[i] = instances[i]->Base();
	//	}
	//	entity->shadowMaterial = deferredRenderer->shadowSkinnedMaterial.CreateInstances(entity->model, entity->desc.materialPerMesh);
	//}
}

void OPrendererDeferredSetMaterialEffect(OPrenderer* renderer, OPeffect* effect, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->passes[pass]->effect = effect;
}

void OPrendererDeferredBegin(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->gBuffer.Bind();
	OPrenderClear(0, 0, 0);
}

void OPrendererDeferredSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(model, world, material, true);
}

void OPrendererDeferredSubmitModelMaterial(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(model, world, material);
}

void OPrendererDeferredSubmitMeshMaterial(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, bool shadowed, OPmaterial* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(mesh, world, material);
}

void OPrendererDeferredSubmitRendererEntity(OPrenderer* renderer, OPrendererEntity* rendererEntity) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
	//if (rendererEntity->shadowEmitter) {
	//	deferredRenderer->renderBucket[0].Submit(rendererEntity->model, &rendererEntity->world, deferredRenderer->defaultShadowMaterialInstance);
	//}
}

void OPrendererDeferredSubmitLight(OPrenderer* renderer, OPlightSpot* light) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->spotLights[deferredRenderer->spotLightsInd++] = *light;
	//deferredRenderer->renderBucket[1].Submit(deferredRenderer->sphereMesh, world, deferredRenderer->defaultLightingSpotMaterialInstance);
}

void OPrendererDeferredEnd(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;

	// DRAW SCENE
	{
		OPrenderBlend(false);
		deferredRenderer->renderBucket[0].Sort();
		deferredRenderer->renderBucket[0].Flush(false);

		deferredRenderer->gBuffer.Unbind();
	}


	// DRAW LIGHTS
	{
		deferredRenderer->invertViewProjection = (*deferredRenderer->rendererRoot.camera)->view * (*deferredRenderer->rendererRoot.camera)->proj;
		OPmat4Inverse(&deferredRenderer->invertViewProjection, deferredRenderer->invertViewProjection);

		deferredRenderer->lightBuffer.Bind();
		f32 ambient = 0.25;
		OPrenderClearColor(ambient, ambient, ambient, 1.0);

		OPrenderDepthWrite(false);
		OPrenderDepth(false);
		OPrenderBlend(true);
		OPRENDERER_ACTIVE->SetBlendModeAdditive();
		//OPrenderCullMode(2);

		deferredRenderer->defaultLightingSpotEffect->Bind();
		deferredRenderer->sphereMesh->Bind();
		OPmat4 world;

		//OPrenderSetWireframe(wireframe);
		OPrenderCull(true);
		OPrenderCullMode(OPcullFace::BACK);
		for (ui32 i = 0; i < deferredRenderer->spotLightsInd; i++) {
			world.SetScl(deferredRenderer->spotLights[i].radius)->Translate(deferredRenderer->spotLights[i].position);
			OPeffectSet("uWorld", 1, &world);
			OPeffectSet("uLightPos", &deferredRenderer->spotLights[i].position);
			OPeffectSet("uLightColor", &deferredRenderer->spotLights[i].color);
			OPeffectSet("uLightRadius", deferredRenderer->spotLights[i].radius);
			OPeffectSet(*renderer->camera);
			OPeffectSet("uGbufferDepth", &deferredRenderer->gBuffer.depthTexture, 0);
			OPeffectSet("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 1);
			OPrenderDrawBufferIndexed(0);
		}
		OPRENDERER_ACTIVE->SetBlendModeAlpha();
		//OPrenderSetWireframe(false);

		//OPrenderBlend(true);
		//deferredRenderer->renderBucket[1].Submit(deferredRenderer->quadMesh, &deferredWorld, deferredRenderer->defaultLightingMaterialInstance);
		//deferredRenderer->renderBucket[1].Sort();
		//deferredRenderer->renderBucket[1].Flush(false);

		deferredRenderer->lightBuffer.Unbind();

		OPrenderCullMode(OPcullFace::FRONT);
		OPrenderDepth(true);
		OPrenderDepthWrite(true);

		deferredRenderer->spotLightsInd = 0;
	}

}

void OPrendererDeferredPresent(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	// Combine GBuffer and Lighting

	// COMBINE
	{
		OPrenderClear(0,0,0,1);
		OPrenderCullMode(OPcullFace::FRONT);
		OPrenderDepth(false);
		OPrenderDepthWrite(false);
		//OPrenderResetViewport();

		// Draw fullscreen quad to combine
		deferredRenderer->defaultCombineEffect->Bind();
		deferredRenderer->quadMesh->Bind();
		OPrenderDepth(0);
		OPrenderCull(false);
		OPmat4 world = OPMAT4_IDENTITY;

		OPeffectSet("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
		OPeffectSet("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
		OPeffectSet("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);
		OPeffectSet("uLightBuffer", deferredRenderer->lightBuffer.texture, 3);
		OPeffectSet("uWorld", 1, &world);

		OPrenderDrawBufferIndexed(0);

		OPrenderCullMode(OPcullFace::FRONT);
		OPrenderDepth(true);
		OPrenderDepthWrite(true);
	}
}

OPrendererDeferred* OPrendererDeferred::Setup() {
	rendererRoot._Init = OPrendererDeferredInit;
	rendererRoot._CreateMaterial = OPrendererDeferredCreateMaterial;
	rendererRoot._GetMaterial = OPrendererDeferredGetMaterial;
	rendererRoot._SetMaterial = OPrendererDeferredSetMaterial;
	rendererRoot._SetMaterials = OPrendererDeferredSetMaterials;
	rendererRoot._SetMaterialEffect = OPrendererDeferredSetMaterialEffect;
	rendererRoot._Begin = OPrendererDeferredBegin;
	rendererRoot._SubmitModel = OPrendererDeferredSubmitModel;
	rendererRoot._SubmitRendererEntity = OPrendererDeferredSubmitRendererEntity;
	rendererRoot._SubmitModelMaterial = OPrendererDeferredSubmitModelMaterial;
	rendererRoot._SubmitMeshMaterial = OPrendererDeferredSubmitMeshMaterial;
	rendererRoot._SubmitLight = OPrendererDeferredSubmitLight;
	rendererRoot._End = OPrendererDeferredEnd;
	rendererRoot._Present = OPrendererDeferredPresent;

	rendererRoot.internalPtr = this;

	return this;
}

OPrendererDeferred* OPrendererDeferred::Create() {
	OPrendererDeferred* result = OPNEW(OPrendererDeferred());
	return result->Setup();
}

void OPrendererDeferred::Destroy() {
	if (defaultGBufferEffect != NULL) {
		defaultGBufferEffect->Destroy();
		OPfree(defaultGBufferEffect);
	}
	if (defaultGBufferMaterial != NULL) {
		defaultGBufferMaterial->Destroy();
		OPfree(defaultGBufferMaterial);
	}
	if (defaultLightingEffect != NULL) {
		defaultLightingEffect->Destroy();
		OPfree(defaultLightingEffect);
	}
	if (defaultLightingMaterial != NULL) {
		defaultLightingMaterial->Destroy();
		OPfree(defaultLightingMaterial);
	}
	if (defaultLightingSpotEffect != NULL) {
		defaultLightingSpotEffect->Destroy();
		OPfree(defaultLightingSpotEffect);
	}
	if (defaultLightingSpotMaterial != NULL) {
		defaultLightingSpotMaterial->Destroy();
		OPfree(defaultLightingSpotMaterial);
	}

	if (sphereMesh != NULL) {
		sphereMesh->Destroy();
		OPfree(sphereMesh);
	}

	if (quadMesh != NULL) {
		quadMesh->Destroy();
		OPfree(quadMesh);
	}

	gBuffer.Destroy();

	renderBucket[0].Destroy();
	renderBucket[1].Destroy();
}

#include "./Math/include/OPtween.h"

void OPrendererDeferred2::Init() {
	defaultGBufferMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag"))));
	defaultLightingMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/DeferredLighting.vert", "Common/DeferredLighting.frag"))));
	defaultLightingMaterial->depth = false;
	defaultLightingMaterial->cull = false;

	passes = OPNEW(OPrendererPass, 2);

	passes[0].renderBucket.Init(1000, camera);
	passes[0].defaultMaterial = defaultGBufferMaterial;
	passes[1].renderBucket.Init(1000, shadowCamera);
	passes[1].defaultMaterial = defaultGBufferMaterial;

	OPtextureDesc gBufferDesc[3];
	// Position
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGBA16F, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Normal
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	gBuffer.Init(gBufferDesc, 3);


	OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	//OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBuffer.Init(ssaoDesc);
	//OPtextureDesc ssaoBlurDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBlurBuffer.Init(ssaoDesc);

	// Sample Kernel
	for (ui32 i = 0; i < 64; i++) {
		OPvec3 sample = OPvec3(
			OPrandom() * 2.0 - 1.0,
			OPrandom() * 2.0 - 1.0,
			OPrandom()
		);

		sample.Norm();
		sample *= OPrandom();

		OPfloat scale = i / 64.0f;
		scale = 0.1 + (scale * scale) * 0.9f; // linear lerp
		sample *= scale;
		ssaoKernel[i] = sample;
	}

	OPvec3 ssaoNoise[16];
	for (ui32 i = 0; i < 16; i++) {
		OPvec3 noise = OPvec3(
			OPrandom() * 2.0 - 1.0,
			OPrandom() * 2.0 - 1.0,
			0
		);

		ssaoNoise[i] = noise;
	}

	OPtextureDesc desc = OPtextureDesc(4,4, OPtextureFormat::RGB, OPtextureFormat::RGB16F, OPtextureWrap::REPEAT, OPtextureFilter::NEAREST, OPtextureType::FLOAT);	
	//OPRENDERER_ACTIVE->Texture.Init(&noiseTexture, desc);
	noiseTexture = OPRENDERER_ACTIVE->Texture.Create(desc, &ssaoNoise[0]);



	OPtextureDesc lightBufferDec = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	lightBuffer.Init(lightBufferDec, &gBuffer.depthTexture);

	defaultCombineMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/DeferredCombine.vert", "Common/DeferredCombine.frag"))));
	//defaultCombineMaterial->AddParam("uGbufferPosition", &gBuffer.texture[0], 0);
	//defaultCombineMaterial->AddParam("uGbufferNormal", &gBuffer.texture[1], 1);
	//defaultCombineMaterial->AddParam("uGbufferAlbedoSpec", &gBuffer.texture[2], 2);
	//defaultCombineMaterial->AddParam("uLightBuffer", lightBuffer.texture, 3);
	defaultCombineMaterial->depth = false;
	defaultCombineMaterial->cull = false;

	quadMesh = OPquadCreate(1.0f, 1.0f, OPvec2(0,1), OPvec2(1,0));
	sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");



	defaultSSAOMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao.frag"))));
	//defaultSSAOMaterial->AddParam("uGbufferPosition", &gBuffer.texture[0], 0);
	//defaultSSAOMaterial->AddParam("uGbufferNormal", &gBuffer.texture[1], 1);
	//defaultSSAOMaterial->AddParam("uNoise", &noiseTexture, 2);
	//defaultSSAOMaterial->AddParam("uGbufferDepth", &gBuffer.depthTexture, 3);
	//defaultSSAOMaterial->AddParam("samples[0]", &ssaoKernel[0]);
	//defaultSSAOMaterial->AddParam(OPmaterialParamType::VECTOR3, "samples", &ssaoKernel, 64);
	defaultSSAOBlurMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao_blur.frag"))));
	//defaultSSAOBlurMaterial->AddParam("uSSAOBuffer", ssaoBuffer.texture, 0);

	char buffer[64];
	for (ui32 i = 0; i < 64; i++) {
		sprintf(buffer, "samples[%d]", i);
		OPRENDERER_ACTIVE->Effect.AddUniform(defaultSSAOMaterial->effect, buffer);
	}
}

void OPrendererDeferred2::Begin() {
	gBuffer.Bind();
	OPrenderClear(0, 0, 0);
}

void OPrendererDeferred2::Submit(OPrendererEntity* rendererEntity) {
	passes[0].renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
}

OPmaterial* OPrendererDeferred2::GetMaterial(ui32 pass) {
	return passes[0].defaultMaterial;
}

void OPrendererDeferred2::End() {
	// DRAW SCENE
	{
		OPrenderBlend(false);
		// Geometry pass
		passes[0].renderBucket.Sort();
		passes[0].renderBucket.Flush(false);

		gBuffer.Unbind();




		// SSAO pass
		ssaoBuffer.Bind();
		OPrenderCullMode(OPcullFace::FRONT);
		OPrenderDepth(false);
		OPrenderDepthWrite(false);
		OPrenderClear(0, 0, 0, 1);
		defaultSSAOMaterial->Bind();	
		
		OPeffectSet("uGbufferPosition", &gBuffer.texture[0], 0);
		OPeffectSet("uGbufferNormal", &gBuffer.texture[1], 1);
		OPeffectSet("uNoise", noiseTexture, 2);
		char buffer[64];
		for (ui32 i = 0; i < 64; i++) {
			sprintf(buffer, "samples[%d]", i);
			OPeffectSet(buffer, &ssaoKernel[i]);
		}

		OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uProj"), &(*camera)->proj);
		OPrenderDepth(0);
		OPrenderCull(false);
		quadMesh->Bind();
		OPrenderDrawBufferIndexed(0);
		ssaoBuffer.Unbind();


		// Blur SSAO pass
		ssaoBlurBuffer.Bind();
		OPrenderCullMode(OPcullFace::FRONT);
		OPrenderDepth(false);
		OPrenderDepthWrite(false);
		OPrenderClear(0,0,0,1);
		defaultSSAOBlurMaterial->Bind();

		OPeffectSet("uSSAOBuffer", ssaoBuffer.texture, 0);

		OPrenderDepth(0);
		OPrenderCull(false);
		quadMesh->Bind();
		OPrenderDrawBufferIndexed(0);
		ssaoBlurBuffer.Unbind();
	}

	// DRAW LIGHTS
	{
		lightBuffer.Bind();
		f32 ambient = 0.0;
		OPrenderClearColor(ambient, ambient, ambient, 1.0);
		lightBuffer.Unbind();

	}
}

void OPrendererDeferred2::Present() {
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);

	defaultCombineMaterial->Bind();
	quadMesh->Bind();
	OPrenderDepth(0);
	OPrenderCull(false);

	OPmat4 world = OPMAT4_IDENTITY;

	//OPeffectSet("uGbufferPosition", &gBuffer.texture[0], 0);
	//OPeffectSet("uGbufferNormal", &gBuffer.texture[1], 1);
	OPeffectSet("uGbufferAlbedoSpec", &gBuffer.texture[2], 2);
	OPeffectSet("uLightBuffer", lightBuffer.texture, 3);
	OPeffectSet("uSSAOBuffer", ssaoBlurBuffer.texture, 3);
	OPeffectSet("uWorld", 1, &world);

	OPrenderDrawBufferIndexed(0);

	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(true);
	OPrenderDepthWrite(true);
}

void OPrendererDeferred2::Destroy() {
	defaultGBufferMaterial->effect->Free();
	defaultGBufferMaterial->Destroy();
	OPfree(defaultGBufferMaterial);
	defaultLightingMaterial->effect->Free();
	defaultLightingMaterial->Destroy();
	OPfree(defaultLightingMaterial);
	defaultLightingSpotMaterial->effect->Free();
	defaultLightingSpotMaterial->Destroy();
	OPfree(defaultLightingSpotMaterial);
	defaultCombineMaterial->effect->Free();
	defaultCombineMaterial->Destroy();
	OPfree(defaultCombineMaterial);

}

void OPrendererDeferred2::SetCamera(OPcam** camera) {
	SetCamera(camera, 0);
	SetCamera(camera, 1);
}

void OPrendererDeferred2::SetCamera(OPcam** camera, ui32 pass) {
	this->camera = camera;
	passes[pass].renderBucket.camera = camera;
}