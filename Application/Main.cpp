//////////////////////////////// OPEngine MAIN ////////////////////////////////
#define WIN32_LEAN_AND_MEAN 0
#include <stdio.h>
#include <stdlib.h>

#include "./Core/include/Core.h"
#include "./Data/include/OPgameStates.h"

#include "./GameManager.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Input/GamePadSystem.h"

#include "./Core/include/Log.h"

#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPheap.h"
#include "./Data/include/OPlist.h"

#include "./Performance/include/OPthread.h"

#if defined(OPIFEX_ANDROID)
#include <jni.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <direct.h>
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#endif

#include "./Data/include/OPfile.h"
#include "./Human/include/Audio/OPaudio2.h"
#include "./Human/include/Audio/OPaudioEmitter.h"
#include "./Human/include/Audio/OPaudioPlayer.h"
#include "./Human/include/Utilities/OPMloader.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Data/include/OPentHeap.h"

GameManager* GM;
OPaudioSource *Sound, *Sound1, *Sound2;
OPaudioEmitter *Emitter;
OPaudioPlayer player, player1;
OPentHeap* ents;
void* entData;

OPfloat vol = 0.05f;

void KeyDown(int key, int action){
	OPLog("Pizza %d", key);

	switch(key){
		case 65:
			OPaudSetPlayer(&player);
			OPaudPlayerPlay();
			break;
		case 83:
			OPaudSetPlayer(&player1);
			OPaudPlayerPlay();
			break;
	}

}

// Initialize
void Init(){
	#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	_chdir("assets\\");
	#else
	if(chdir("./assets")){
		OPLog("Directory changed!\n");
	}
	else
		OPLog("Directory change failed!!!\n");
	#endif


#ifndef OPIFEX_ANDROID
	i32 width = 640;
	i32 height = 480;
#endif

	OPassetLoader loaders[] ={
		{
			".wav",
			"Audio/",
			sizeof(OPaudioSource),
			(OPint (*)(const OPchar*, void**))OPaudOpenWave,
			(OPint (*)(void*))OPaudCloseWave
		},
		{
			".ogg",
			"Audio/",
			sizeof(OPaudioSource),
			(OPint (*)(const OPchar*, void**))OPaudOpenOgg,
			(OPint (*)(void*))OPaudCloseOgg
		},
		{
			".png",
			"Textures/",
			sizeof(Texture2D),
			(OPint (*)(const OPchar*, void**))OPimagePNGLoad,
			(OPint (*)(void*))OPimagePNGUnload
		},
		{
			".vert",
			"Shaders/",
			sizeof(OPshader),
			(OPint (*)(const OPchar*, void**))OPrenderLoadVertexShader,
			(OPint (*)(void*))OPrenderUnloadShader
		},
		{
			".frag",
			"Shaders/",
			sizeof(OPshader),
			(OPint (*)(const OPchar*, void**))OPrenderLoadFragmentShader,
			(OPint (*)(void*))OPrenderUnloadShader
		},
		{
			".opm",
			"Models/",
			sizeof(OPmesh),
			(OPint (*)(const OPchar*, void**))OPMload,
			(OPint (*)(void*))OPMUnload
		}
	};

	OPcmanInit(loaders, 6);
	
	OPaudInit();
	OPaudInitThread(10);

#ifndef OPIFEX_ANDROID
	GM = new GameManager(width, height);
#else
	GM = new GameManager(JNIWidth(), JNIHeight());
#endif


#ifndef OPIFEX_ANDROID//defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	  	glfwSetKeyCallback(KeyDown);
#endif

        OPLog("Main: Song loading...");
        OPchar songPath[] = {"Audio/background.ogg"};
        //Song = OPAudio::ReadOgg(songPath);

        OPLog("Main: Song loaded");

		Sound1 = (OPaudioSource*)OPcmanGet("impact.wav");
		Sound2 = (OPaudioSource*)OPcmanGet("boom.wav");
		Sound = (OPaudioSource*)OPcmanGet("background.ogg");

        OPLog("Reading done!\n");
        player = OPaudPlayerCreate(Sound1, 5, 0);
        player1 = OPaudPlayerCreate(Sound2, 4, 0);
		Emitter = OPaudCreateEmitter(Sound, EMITTER_THREADED | EMITTER_LOOPING);
        OPLog("Emitter created\n");

		OPaudSetEmitter(Emitter);
        OPaudVolume(0.05f);
        OPLog("Emitter set\n");
        OPLog("Emitter proc'd\n");
        OPaudPlay();

	return;
}

void Update( OPtimer* timer){


	bool result = GM->Update( timer );

	GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	OPgamePadUpdate(gamePad);

	if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_A)){
		OPLog("Playing Audio");
	  	OPaudSetPlayer(&player);
	  	OPaudPlayerPlay();
	}

	//SoundEmitter->Update();
	GM->Draw();
	OPrenderPresent();
	
	if(!result)
		exit(0);
	return;
}

void Destroy()
{
	delete GM;
	return;
}

void UpdateState(OPtimer* timer){
	ActiveState->Update(timer);
}


#ifdef OPIFEX_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
#else
	int main() {
#endif

	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;

	OPint a = 0;
	OPLog("Test %d %d %d", a, a, a);

	ActiveState = OPgameStateCreate(NULL, Update, NULL);

#ifdef OPIFEX_ANDROID
	return;
#else
	OPstart();
	OPend();
	return 0;
#endif
}
