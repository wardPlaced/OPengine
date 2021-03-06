
#include "./Human/include/Platform/opengl/OPframeBufferAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Platform/opengl/OPrenderBufferAPIGL.h"
#include "./Human/include/Rendering/OPwindow.h"



OPframeBuffer* _OPframeBufferAPIGLInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc) {
	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = 1;
	framebuffer->desc = textureDesc;


	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	framebuffer->renderBuffer = OPRENDERER_ACTIVE->RenderBuffer.Create();
	framebuffer->renderBuffer->Bind();
	

	//framebuffer->texture = OPRENDERER_ACTIVE->Texture.Create(textureDesc);
	//OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture->internalPtr;

	// Texture Buffer
	textureDesc.mipmap = false;


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* _OPframeBufferAPIGLInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = 1;
	framebuffer->desc = textureDesc;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	framebuffer->texture = OPRENDERER_ACTIVE->Texture.Create(textureDesc);
	OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture->internalPtr;

	framebuffer->renderBuffer = OPRENDERER_ACTIVE->RenderBuffer.Create();
	framebuffer->renderBuffer->Bind();

	// Texture Buffer
	textureDesc.mipmap = false;

	OPRENDERER_ACTIVE->Texture.Init(&framebuffer->depthTexture, depthDesc);
	

	if (!textureDesc.multisampled) {
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGL->Handle, 0));
		
		// Depth Buffer
		OPtextureGL* depthTextureGL = (OPtextureGL*)framebuffer->depthTexture.internalPtr;
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));
	}
	else {

		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureGL->Handle, 0));

		// Depth Buffer
		OPGLFN(glGenRenderbuffers(1, &frameBufferGL->DepthHandle));
		OPGLFN(glBindRenderbuffer(GL_RENDERBUFFER, frameBufferGL->DepthHandle));
		OPGLFN(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, textureDesc.width, textureDesc.height));
		OPGLFN(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufferGL->DepthHandle));
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLInitialize(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, bool depth = true) {

	if (depth) {
		OPtextureDesc depthDesc = OPtextureDesc();
		depthDesc.minfilter = OPtextureFilter::NEAREST;
		depthDesc.magfilter = OPtextureFilter::NEAREST;
		depthDesc.format = OPtextureFormat::DEPTH;
		depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
		depthDesc.width = textureDesc.width;
		depthDesc.height = textureDesc.height;
		depthDesc.textureType = OPtextureType::BYTE;
		depthDesc.multisampled = textureDesc.multisampled;
		depthDesc.wrap = OPtextureWrap::CLAMP_TO_BORDER;
		depthDesc.mipmap = false;

		return _OPframeBufferAPIGLInit(framebuffer, textureDesc, depthDesc);
	}
	else {
		return _OPframeBufferAPIGLInit(framebuffer, textureDesc);
	}
}

OPframeBuffer* OPframeBufferAPIGLInitializeDepth(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
	return _OPframeBufferAPIGLInit(framebuffer, textureDesc, depthDesc);
}

OPframeBuffer* OPframeBufferAPIGLInitMultiDepth(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {

	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = count;
	framebuffer->texture = OPALLOC(OPtexture, count);

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	for (ui32 i = 0; i < count; i++) {
		//textureDesc[i].multisampled = true;
		OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture[i], textureDesc[i]);
		OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture[i].internalPtr;
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureGL->Handle, 0));
		//OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, textureGL->Handle, 0));
	}

	GLuint attachments[10] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9 };
	OPGLFN(glDrawBuffers(count, attachments));


	framebuffer->depthTexture = *depthTexture;
	OPtextureGL* depthTextureGL = (OPtextureGL*)framebuffer->depthTexture.internalPtr;
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLInitMulti(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count) {
	OPtextureDesc depthDesc = OPtextureDesc();
	depthDesc.minfilter = OPtextureFilter::NEAREST;
	depthDesc.magfilter = OPtextureFilter::NEAREST;
	depthDesc.format = OPtextureFormat::DEPTH;
	depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
	depthDesc.width = textureDesc[0].width;
	depthDesc.height = textureDesc[0].height;
	depthDesc.textureType = OPtextureType::BYTE;
	depthDesc.multisampled = false;
    depthDesc.wrap = OPtextureWrap::CLAMP_TO_BORDER;
	depthDesc.mipmap = false;

	OPtexture depthTexture;
	OPRENDERER_ACTIVE->Texture.Init(&depthTexture, depthDesc);

	return OPframeBufferAPIGLInitMultiDepth(framebuffer, textureDesc, count, &depthTexture);
}

OPframeBuffer* OPframeBufferAPIGLCreate(OPtextureDesc textureDesc) {
	OPframeBuffer* frameBuffer = OPNEW(OPframeBuffer());
	return OPframeBufferAPIGLInitialize(frameBuffer, textureDesc);
}

ui32 OPframeBufferModeToGL(OPframeBufferMode::Enum mode) {
	switch (mode)
	{
	case OPframeBufferMode::BOTH: return GL_FRAMEBUFFER;
	case OPframeBufferMode::READ: return GL_READ_FRAMEBUFFER;
	case OPframeBufferMode::DRAW: return GL_DRAW_FRAMEBUFFER;
	}
	return 0;
}

void OPframeBufferAPIGLUnbind() {
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = NULL;
}

void OPframeBufferAPIGLUnbindReadOnly() {
	OPGLFN(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = NULL;
}

void OPframeBufferAPIGLUnbindWriteOnly() {
	OPGLFN(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = NULL;
}

void _OPframeBufferAPIGLBind(ui32 mode, OPframeBuffer* ptr, ui32 mip) {
	if (ptr == NULL) {
		return OPframeBufferAPIGLUnbind();
	}

	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)ptr->internalPtr;

	OPGLFN(glBindFramebuffer(mode, frameBufferGL->Handle));
	if (ptr->renderBuffer != NULL) {
		ptr->renderBuffer->Bind();
	}

	if (mip == 0) {
		OPGLFN(glViewport(0, 0, ptr->desc.width, ptr->desc.height));
	}
	else {
		if (ptr->renderBuffer != NULL) {
			// Adjust depth buffer & viewport to mip map size
			ui32 mipWidth = ptr->desc.width * OPpow(0.5, mip);
			ui32 mipHeight = ptr->desc.height * OPpow(0.5, mip);
			ptr->renderBuffer->SetSize(mipWidth, mipHeight);
			OPGLFN(glViewport(0, 0, mipWidth, mipHeight));
		}
	}

	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = ptr;
}

void OPframeBufferAPIGLBind(OPframeBuffer* ptr, ui32 mip) {
	_OPframeBufferAPIGLBind(GL_FRAMEBUFFER, ptr, mip);
}

void OPframeBufferAPIGLBindReadOnly(OPframeBuffer* ptr, ui32 mip) {
	_OPframeBufferAPIGLBind(GL_READ_FRAMEBUFFER, ptr, mip);
}

void OPframeBufferAPIGLBindWriteOnly(OPframeBuffer* ptr, ui32 mip) {
	_OPframeBufferAPIGLBind(GL_DRAW_FRAMEBUFFER, ptr, mip);
}

void OPframeBufferAPIGLSetAttachmentDepthTexture(OPframeBuffer* frameBuffer, OPtexture* texture) {
	OPtextureGL* textureGL = (OPtextureGL*)texture->internalPtr;
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)frameBuffer->internalPtr;

	OPframeBufferAPIGLBind(frameBuffer, 0);

	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureGL->Handle, 0));
}

void OPframeBufferAPIGLSetAttachmentTexture(OPframeBuffer* frameBuffer, ui32 ind, OPtexture* texture) {
	OPtextureGL* textureGL = (OPtextureGL*)texture->internalPtr;
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)frameBuffer->internalPtr;
	
	OPframeBufferAPIGLBind(frameBuffer, 0);
	frameBuffer->renderBuffer->Bind();
	frameBuffer->renderBuffer->SetSize(texture->textureDesc.width, texture->textureDesc.height);

	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ind, GL_TEXTURE_2D, textureGL->Handle, 0));
}

void OPframeBufferAPIGLSetAttachmentRenderBuffer(OPframeBuffer* frameBuffer, OPrenderBuffer* renderBuffer) {
	OPframeBufferAPIGLBind(frameBuffer, 0);
	OPrenderBufferGL* renderBufferGL = (OPrenderBufferGL*)renderBuffer->internalPtr;

	OPGLFN(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferGL->Handle));
}

void OPframeBufferAPIGLDestroy(OPframeBuffer* ptr) {
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)ptr->internalPtr;
	if (frameBufferGL->DepthHandle > 0) {
		ptr->depthTexture.Destroy();
	}
	if (ptr->renderBuffer != NULL) {
		ptr->renderBuffer->Destroy();
		OPfree(ptr->renderBuffer);
	}
	//for (ui32 i = 0; i < ptr->count; i++) {
	//	ptr->texture[i].Destroy();
	//}

	OPGLFN(glDeleteFramebuffers(1, &frameBufferGL->Handle));
	OPfree(frameBufferGL);
	ptr->internalPtr = NULL;
}

void OPframeBufferAPIGLInit(OPframeBufferAPI* frameBuffer) {
	frameBuffer->_Bind = OPframeBufferAPIGLBind;
	frameBuffer->BindReadOnly = OPframeBufferAPIGLBindReadOnly;
	frameBuffer->BindWriteOnly = OPframeBufferAPIGLBindWriteOnly;
	frameBuffer->SetAttachmentDepthTexture = OPframeBufferAPIGLSetAttachmentDepthTexture;
	frameBuffer->SetAttachmentTexture = OPframeBufferAPIGLSetAttachmentTexture;
	frameBuffer->SetAttachmentRenderBuffer = OPframeBufferAPIGLSetAttachmentRenderBuffer;
	frameBuffer->Destroy = OPframeBufferAPIGLDestroy;
	frameBuffer->Unbind = OPframeBufferAPIGLUnbind;
	frameBuffer->UnbindReadOnly = OPframeBufferAPIGLUnbindReadOnly;
	frameBuffer->UnbindWriteOnly = OPframeBufferAPIGLUnbindWriteOnly;
	frameBuffer->_Create = OPframeBufferAPIGLCreate;
	frameBuffer->_Init = OPframeBufferAPIGLInitialize;
	frameBuffer->_InitMulti = OPframeBufferAPIGLInitMulti;
	frameBuffer->_InitDepth = OPframeBufferAPIGLInitializeDepth;
	frameBuffer->_InitMultiDepth = OPframeBufferAPIGLInitMultiDepth;
}

#endif