#include "./Human/include/Rendering/FrameBuffer.h"
#include "./Core/include/Log.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

GLuint createDepthTexture(int w, int h) {

	GLuint tex;

	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
						w,
						h, 
						0, GL_DEPTH_COMPONENT, GL_FLOAT,
						NULL); 

	glBindTexture(GL_TEXTURE_2D, 0);

	return(tex);
}

OPframeBuffer OPframeBufferCreate(OPtextureDescription desc){
	OPframeBuffer fb = {
		desc,
		OPtextureCreate(desc),
		0
	};

	// generate and bind the fbo
	glGenFramebuffers(1, &fb.Handle);
	
	// setup color texture
	OPtextureBind(&fb.Texture);	
	OPtextureSetData(NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach the depth texture
	ui32 dt = createDepthTexture(desc.Width, desc.Height);

#ifndef OPIFEX_ANDROID
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.Handle);
	glBindRenderbuffer(GL_RENDERBUFFER, dt);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, desc.Width, desc.Height);


	// attach the color texture
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb.Texture.Handle, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dt, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, fb.Handle);
	glBindRenderbuffer(GL_RENDERBUFFER, dt);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, desc.Width, desc.Height);


	// attach the color texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.Texture.Handle, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dt);
#endif

	// check fbo creation status
#ifndef OPIFEX_ANDROID
	GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
#else
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
#endif
	switch (e) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
			OPlog("FBO Incomplete Attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
			OPlog("FBO Missing Attachment\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED :
			OPlog("FBO Unsupported\n");
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			OPlog("FBO OK\n");
			break;
#ifndef OPIFEX_ANDROID
		case GL_FRAMEBUFFER_UNDEFINED:
			OPlog("FBO Undefined\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
			OPlog("FBO Incomplete Draw Buffer\n");
			break;
#else
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			OPlog("FBO Incomplete Dimensions OK\n");
			break;
#endif
		default:
			OPlog("FBO Problem?\n");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
#ifndef OPIFEX_ANDROID
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	return fb;
}
//-----------------------------------------------------------------------------
void OPframeBufferDestroy(OPframeBuffer* fb){
	OPtextureDestroy(&fb->Texture);
	glDeleteFramebuffers(1, &fb->Handle);
}
//-----------------------------------------------------------------------------
void OPframeBufferBind(OPframeBuffer* fb){
	//GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	OPRENDER_CURR_FRAMEBUFFER = fb;
	glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle);
	//glDrawBuffers(1,  attachments);
	OPrenderSetViewport(0, 0, fb->Description.Width, fb->Description.Height);
}
//-----------------------------------------------------------------------------
void OPframeBufferBindTex(OPframeBuffer* fb){
	OPtextureBind(&fb->Texture);
}
//-----------------------------------------------------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
void OPframeBufferUnbind(){
	OPframeBuffer* fb = OPRENDER_CURR_FRAMEBUFFER;

	glBindTexture(GL_TEXTURE_2D, fb->Texture.Handle);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
/*	OPlog("FBO W: %d, H: %d", fb->Description.Width, fb->Description.Height);

	glBindTexture(GL_TEXTURE_2D, fb->Texture.Handle);
	glCopyTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		0,
		0,
		fb->Description.Width,
		fb->Description.Height,
		0	
	);
	glGenerateMipmap(fb->Texture.Handle);*/
}
