#ifndef OPAUD_EMITTER
#define OPAUD_EMITTER

#include "OPaudioSource.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_COUNT 10
//-----------------------------------------------------------------------------
// ______                           
// |  ____|                          
// | |__   _ __  _   _ _ __ ___  ___ 
// |  __| | '_ \| | | | '_ ` _ \/ __|
// | |____| | | | |_| | | | | | \__ \
// |______|_| |_|\__,_|_| |_| |_|___/
enum OPaudioEmitterState{
	Playing = 0,
	Stopped = 1,
	Paused = 2
};
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPint               CurrBuffer;           // index of the buffer currently being filled
	OPint				Looping;              // indicates weather the sounds restarts when finished or not
	OPaudioEmitterState State;                // current play state of the sound
	OPaudioSource*      Source;               // data source for the sound
	ui8                 Temp[4096];           // 4096 byte temp buffer for processing
	void*               Processor;            // Pointer to data for audio processing

#ifdef OPIFEX_ANDROID 
	SLObjectItf _outputMixObject, _playerObject;
	SLAndroidSimpleBufferQueueItf _bqPlayerBufferQueue;

	SLPlayItf _playerPlay;
	SLSeekItf _playerSeek;
	SLMuteSoloItf _playerMuteSolo;
	SLVolumeItf _playerVolume;

	ui8* Buffers[BUFFER_COUNT];// Buffers of processed data
#else // All other targets...
	ALuint al_src;
	ALuint Buffers[BUFFER_COUNT]; // AL buffers 
#endif
}OPaudioEmitter;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPaudioEmitter* OPAUD_CURR_EMITTER;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/                                                                            
#define OPaudSetEmitter(emitter){\
	OPAUD_CURR_EMITTER = emitter;\
}\
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//                    _ _       ______           _ _   _              ______                _   _                 
//     /\            | (_)     |  ____|         (_) | | |            |  ____|              | | (_)                
//    /  \  _   _  __| |_  ___ | |__   _ __ ___  _| |_| |_ ___ _ __  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   / /\ \| | | |/ _` | |/ _ \|  __| | '_ ` _ \| | __| __/ _ \ '__| |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  / ____ \ |_| | (_| | | (_) | |____| | | | | | | |_| ||  __/ |    | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// /_/    \_\__,_|\__,_|_|\___/|______|_| |_| |_|_|\__|\__\___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPaudioEmitter OPaudCreateEmitter(OPaudioSource* src, OPint looping);

void  OPaudEnqueueBuffer(ui8* buffer, OPint length);

void OPaudPlay ();
void OPaudPause();
void OPaudStop ();
OPint OPaudUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length));

OPint OPaudProc(OPaudioEmitter* emitter, void(*Proc)(OPaudioEmitter* emit, OPint length));

Vector3 OPaudPosition(OPaudioEmitter* emitter, Vector3 position);
Vector3 OPaudVelocity(Vector3* velocity);
OPfloat OPaudVolume  (OPfloat* gain);
OPfloat OPaudPitch   (OPfloat* pitch);
//-----------------------------------------------------------------------------
void OPaudProcess(OPaudioEmitter* emit, OPint length);
//-----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif
#endif