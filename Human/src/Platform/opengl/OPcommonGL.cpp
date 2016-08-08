#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPlog.h"

bool glewInitialized = 0;

i8 OPglewInit() {
	if (glewInitialized) return 0;
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		OPlogErr("Failed to init glew: %d", result);
		return -1;
	}
	glewInitialized = true;
	return 0;
}

GLenum OPcommonGLCheckError() {
	return glGetError();
}

bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line) {
	GLenum error = OPcommonGLCheckError();
	if (error != GL_NO_ERROR)
	{
		OPlogErr("OpenGL Error: [%d] %s, %s, %d", error, function, file, line);
		return false;
	}
	return true;
}