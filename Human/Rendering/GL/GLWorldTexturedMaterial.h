#pragma once

#include "GLWorldProjMaterial.h"
#include "./Human/Rendering/GL/GLTexture.h"

class GLWorldTexturedMaterial : public GLWorldProjMaterial {
public:
	GLWorldTexturedMaterial();
	GLWorldTexturedMaterial(ShaderPtr vertex, ShaderPtr fragment);

	ui32 GetTextureLocation();
	
	void SetUVData(ui32 stride, void* data);
	virtual void SetData(Mesh* mesh);

	void SetTexture(GLTexture* texture, ui32 slot);
	
	void EnableAttributes();
	void DisableAttributes();
private:
	ui32 _uvLoc;
	ui32 _textureLoc;
};