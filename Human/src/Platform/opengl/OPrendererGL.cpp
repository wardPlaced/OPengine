
#include "./Human/include/Platform/opengl/OPrendererGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"

// TODO: (garrett) CMake should be generating these variables and we'll check for just OPIFEX_OPENGL and use the OPIFEX_OPENGL_MAJOR and OPIFEX_OPENGL_MINOR
#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif


void glfwErrorCallback(int error, const char* desc) {
	OPlogErr("GLFW ERROR: %s", desc);
}

i8 OPrendererInitGL(OPwindow* window) {
	OPlogInfo("Initializing OpenGL Renderer");

	glfwSetErrorCallback(glfwErrorCallback);

	window->Bind();

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	if (!OPglewInit()) {
		OPlogErr("Failed to initialize GLEW");
		return -2;
	}

	OPRENDERER_ACTIVE->SetBlendModeAlpha();

	return 0;
}

void OPrendererClearGL(OPvec4 color) {
	OPGLFN(glClearColor(color.x, color.y, color.z, color.w));
	OPGLFN(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OPrendererClearColorGL(OPvec4 color) {
	OPGLFN(glClearColor(color.x, color.y, color.z, color.w));
	OPGLFN(glClear(GL_COLOR_BUFFER_BIT));
}

void OPrendererClearDepthGL() {
	OPGLFN(glClear(GL_DEPTH_BUFFER_BIT));
}

void OPrendererPresentGL() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGL* windowGL = (OPwindowGL*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;

	OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

ui32 OPdepthFunctionToGL(OPdepthFunction::Enum depthFunction) {
	switch (depthFunction)
	{
		case OPdepthFunction::LESS: return GL_LESS;
		case OPdepthFunction::NEVER: return GL_NEVER;
		case OPdepthFunction::EQUAL: return GL_EQUAL;
		case OPdepthFunction::LEQUAL: return GL_LEQUAL;
		case OPdepthFunction::GREATER: return GL_GREATER;
		case OPdepthFunction::NOTEQUAL: return GL_NOTEQUAL;
		case OPdepthFunction::GEQUAL: return GL_GEQUAL;
		case OPdepthFunction::ALWAYS: return GL_ALWAYS;
	}
	return 0;
}

void OPrendererSetDepthFuncGL(OPdepthFunction::Enum fn) {
	OPGLFN(glDepthFunc(OPdepthFunctionToGL(fn)));
}

void OPrendererSetDepthTestingGL(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_DEPTH_TEST));
	}
	else {
		OPGLFN(glDisable(GL_DEPTH_TEST));
	}
}

void OPrendererSetDepthWriteGL(bool state){
	if (state) {
		OPGLFN(glDepthMask(GL_TRUE));
	}
	else {
		OPGLFN(glDepthMask(GL_FALSE));
	}
}

void OPrendererSetCullGL(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_CULL_FACE));
	}
	else {
		OPGLFN(glDisable(GL_CULL_FACE));
	}
}

void OPrendererSetMultisampleGL(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_MULTISAMPLE));
	}
	else {
		OPGLFN(glDisable(GL_MULTISAMPLE));
	}
}

void OPrendererSetWireframeGL(bool state) {
	if (state) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void OPrendererSetCullModeGL(OPcullFace state) {
	OPGLFN(glCullFace(state == OPcullFace::FRONT ? GL_FRONT : GL_BACK));
}

void OPrendererSetBlendGL(bool state){
	if (state) {
		OPGLFN(glEnable(GL_BLEND));
	}
	else {
		OPGLFN(glDisable(GL_BLEND));
	}
}


ui32 OPblendFunctionToGL(OPblendFunction::Enum blendFunction) {
	switch (blendFunction)
	{
		case OPblendFunction::CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
		case OPblendFunction::CONSTANT_COLOR: return GL_CONSTANT_COLOR;
		case OPblendFunction::DST_ALPHA: return GL_DST_ALPHA;
		case OPblendFunction::DST_COLOR: return GL_DST_COLOR;
		case OPblendFunction::ONE: return GL_ONE;
		case OPblendFunction::ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
		case OPblendFunction::ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
		case OPblendFunction::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case OPblendFunction::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case OPblendFunction::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case OPblendFunction::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case OPblendFunction::SRC_ALPHA: return GL_SRC_ALPHA;
		case OPblendFunction::SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
		case OPblendFunction::SRC_COLOR: return GL_SRC_COLOR;
		case OPblendFunction::ZERO: return GL_ZERO;
	}
	return 0;
}

void OPrendererSetBlendModeGL(OPblendFunction::Enum src, OPblendFunction::Enum dst) {
	OPGLFN(glBlendFunc(OPblendFunctionToGL(src), OPblendFunctionToGL(dst)));
}

inline void OPrenderSetViewportGL(ui32 x, ui32 y, ui32 width, ui32 height) {
   // OPlogInfo("Set viewport %dx%d", width, height);
	OPGLFN(glViewport(x, y, width, height));
}

void OPrendererSwapBufferGL() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGL* windowGL = (OPwindowGL*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;
	OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

void OPrendererShutdownGL() {
	glfwTerminate();
}

void OPrendererSetDrawModeGL(OPdrawMode::Enum mode) {
	switch(mode) {
		case OPdrawMode::Polygons: {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
		case OPdrawMode::Lines: {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
		case OPdrawMode::Points: {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}
	}
}


#include "./Human/include/Platform/opengl/OPcontextGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPrenderBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPframeBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPframeBufferDepthAPIGL.h"
#include "./Human/include/Platform/opengl/OPindexBufferGL.h"
#include "./Human/include/Platform/opengl/OPmonitorAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAttributeAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderResourceGL.h"
#include "./Human/include/Platform/opengl/OPshaderUniformAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderUniformBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Platform/opengl/OPtextureCubeAPIGL.h"
#include "./Human/include/Platform/opengl/OPvertexArrayAPIGL.h"
#include "./Human/include/Platform/opengl/OPvertexBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"

OPrenderAPI OPRENDERERGL;
OPrenderAPI* OPrendererGL() {
	OPRENDERERGL.Init = OPrendererInitGL;
	OPRENDERERGL._Clear = OPrendererClearGL;
	OPRENDERERGL._ClearColor = OPrendererClearColorGL;
	OPRENDERERGL.ClearDepth = OPrendererClearDepthGL;
	OPRENDERERGL.Present = OPrendererPresentGL;
	OPRENDERERGL.SetDepthFunc = OPrendererSetDepthFuncGL;
	OPRENDERERGL.SetDepthTesting = OPrendererSetDepthTestingGL;
	OPRENDERERGL.SetDepthWrite = OPrendererSetDepthWriteGL;
	OPRENDERERGL.SetCull = OPrendererSetCullGL;
	OPRENDERERGL.SetCullMode = OPrendererSetCullModeGL;
	OPRENDERERGL.SetMultisample = OPrendererSetMultisampleGL;
	OPRENDERERGL.SetWireframe = OPrendererSetWireframeGL;
	OPRENDERERGL.SetBlend = OPrendererSetBlendGL;
	OPRENDERERGL.SetBlendMode = OPrendererSetBlendModeGL;
	OPRENDERERGL.SetViewport = OPrenderSetViewportGL;
	OPRENDERERGL.SwapBuffer = OPrendererSwapBufferGL;
	OPRENDERERGL.Shutdown = OPrendererShutdownGL;
	OPRENDERERGL.SetDrawMode = OPrendererSetDrawModeGL;

	OPcontextGLInit(&OPRENDERERGL.Context);
	OPeffectAPIGLInit(&OPRENDERERGL.Effect);
	OPrenderBufferAPIGLInit(&OPRENDERERGL.RenderBuffer);
	OPframeBufferAPIGLInit(&OPRENDERERGL.FrameBuffer);
	OPframeBufferDepthAPIGLInit(&OPRENDERERGL.FrameBufferDepth);
	OPindexBufferAPIGLInit(&OPRENDERERGL.IndexBuffer);
	OPmonitorAPIGLInit(&OPRENDERERGL.Monitor);
	OPshaderAPIGLInit(&OPRENDERERGL.Shader);
	OPshaderAttributeAPIGLInit(&OPRENDERERGL.ShaderAttribute);
	OPshaderResourceGLInit(&OPRENDERERGL.ShaderResource);
	OPshaderUniformAPIGLInit(&OPRENDERERGL.ShaderUniform);
	OPshaderUniformBufferAPIGLInit(&OPRENDERERGL.ShaderUniformBuffer);
	OPtextureAPIGLInit(&OPRENDERERGL.Texture);
	OPtextureCubeAPIGLInit(&OPRENDERERGL.TextureCube);
	OPvertexArrayAPIGLInit(&OPRENDERERGL.VertexArray);
	OPvertexBufferAPIGLInit(&OPRENDERERGL.VertexBuffer);
	OPwindowAPIGLInit(&OPRENDERERGL.Window);

	OPlogErr("Renderer Setup");

	return &OPRENDERERGL;
}

#endif