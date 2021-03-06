#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPwindowGL;
typedef struct OPwindowGL OPwindowGL;

#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPwindowAPIGLInit(OPwindowAPI* window);

struct OPwindowGL {
	GLFWwindow* Handle;

	void(*OPWINDOWDROPCALLBACK)(OPuint count, const OPchar**) = NULL;
};

#endif