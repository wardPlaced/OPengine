#include "../include/OPaudio.h"

#define OPAUDIO_SWAP(s1, s2){\
	i16 sTemp = s1;\
	s1 = s2;\
	s2 = sTemp;\
}\

#ifdef OPIFEX_ANDROID
SLEngineItf OPAudio::EngineEngine = NULL;
SLObjectItf OPAudio::EngineObject = NULL;
SLObjectItf OPAudio::OutputMixObject = NULL;

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void *datasource)
{
   return fclose((FILE*)datasource);
}

long ov_tell_func(void *datasource)
{
	return ftell((FILE*)datasource);
}
#else
size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void *datasource)
{
   return fclose((FILE*)datasource);
}

long ov_tell_func(void *datasource)
{
	return ftell((FILE*)datasource);
}
#endif

OPint OPAudio::Init(){
	OPLog("Initializing OP audio...\n");
#ifdef OPIFEX_ANDROID // USE OpenSL ES for sound
    // create engine
	SLresult result;
	result = slCreateEngine(&OPAudio::EngineObject, 0, NULL, 0, NULL, NULL);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 1");
		return false;
	}
	
    // realize the engine
	result = (*OPAudio::EngineObject)->Realize(OPAudio::EngineObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 2");
		return false;
	}
	
    // get the engine interface, which is needed in order to create other objects
    result = (*OPAudio::EngineObject)->GetInterface(OPAudio::EngineObject, SL_IID_ENGINE, &OPAudio::EngineEngine);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 3");
		return false;
	}

	OPLog("OPaudio: engineEngine VVV");
	OPLog_i32((OPint)OPAudio::EngineEngine);

	// create output mix, with environmental reverb specified as a non-required interface    
	const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*OPAudio::EngineEngine)->CreateOutputMix(OPAudio::EngineEngine, &OPAudio::OutputMixObject, 1, ids, req);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 4");
		//return false;
	}
		
    // realize the output mix
    result = (*OPAudio::OutputMixObject)->Realize(OPAudio::OutputMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 5");
		return false;
	}
	OPLog("OPaudio::Initialized");
#else // USE OpenAL FOR SOUND
	// setup the device and stuff
	_OPaudioDevice = alcOpenDevice(NULL);
	if(!_OPaudioDevice) return 0;
	_OPaudioContext = alcCreateContext(_OPaudioDevice, NULL);
	alcMakeContextCurrent(_OPaudioContext);
	if(!_OPaudioContext) return -1;

	alDistanceModel(AL_LINEAR_DISTANCE);
	//SLresult result;
#endif

// USE OGG VORBIS FOR DESKTOP PLATFORMS
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	HINSTANCE _hVorbisFileDLL = LoadLibrary("vorbisfile.dll");
	if(_hVorbisFileDLL){
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(_hVorbisFileDLL, "ov_open_callbacks");

		if (!(fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)){
			return -2;
		}
	}
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	// copy the function pointers directly from
	// the linked SO file.
	fn_ov_clear = (LPOVCLEAR)ov_clear;
	fn_ov_read = (LPOVREAD)ov_read;
	fn_ov_pcm_total = (LPOVPCMTOTAL)ov_pcm_total;
	fn_ov_info = (LPOVINFO)ov_info;
	fn_ov_comment = (LPOVCOMMENT)ov_comment;
	fn_ov_open_callbacks = (LPOVOPENCALLBACKS)ov_open_callbacks;
#endif

	OPLog("OP audio Initialized!!!");
	return 1;
}
/*---------------------------------------------------------------------------*/
OPsound OPAudio::ReadWave(const OPchar* filename){
	//FILE* fp = NULL;
	OPsound ERR = {0};
	//fp = fopen(filename, "rb");
	OPstream* str = OPreadFile(filename);

	if(!str) printf("Error: couldn't open '%s'\n", filename);
	else{
		ui8* type;
		i32 size = 0, chunkSize = 0;
		i16 formatType = 0, channels = 0;
		i32 sampleRate = 0, avgBytesPerSec = 0;
		i16 bytesPerSample = 0, bitsPerSample = 0;
		i32 dataSize = 0;

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "RIFF", 4) != 0){
			OPLog("No RIFF\n");
		}

		OPmemcpy(&size, OPread(str, sizeof(i32)), sizeof(i32));
		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "WAVE", 4) != 0){
			OPLog((char const*)type);
			OPLog("Not WAVE\n");
		}

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "fmt ", 4) != 0){
			OPLog("Not fmt\n");
		}

		OPmemcpy(&chunkSize, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&formatType, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&channels, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&sampleRate, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&avgBytesPerSec, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&bytesPerSample, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&bitsPerSample, OPread(str, sizeof(i16)), sizeof(i16));

		printf("Chunk Size: %d\nChannels: %d\nSample Rate: %d\nBytes/Sec: %d\n",
			(OPint)chunkSize, (OPint)channels, (OPint)sampleRate, (OPint)avgBytesPerSec);

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "data", 4) != 0){
			OPLog("Missing data\n");
		}

		OPmemcpy(&dataSize, OPread(str, sizeof(i32)), sizeof(i32));

		ui8* data = (ui8*)OPalloc(sizeof(ui8) * dataSize);
		OPmemcpy(data, OPread(str, dataSize), dataSize);

		OPstreamDestroy(str);

#ifdef OPIFEX_ANDROID
		OPsound out = {  
			(SLuint32)sampleRate,
			(SLuint32)bitsPerSample,
			(SLuint32)channels, 
			{0},
			NULL,
			NULL,
			dataSize,
			data
		};

		OPLog("Channels:"); OPLog_i32(channels);
		OPLog("sampleRate:"); OPLog_i32(sampleRate);
		OPLog("bps:"); OPLog_i32(avgBytesPerSec);

		SLDataFormat_PCM slFormat = {
			SL_DATAFORMAT_PCM, // specifies the format
			(SLuint32)channels,   
			(SLuint32)sampleRate,
			(SLuint32)bitsPerSample,
			OPceil(bitsPerSample / 32) * 32,
			SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
			SL_BYTEORDER_LITTLEENDIAN
		};

		// keep the SL data format in the OPsound
		*((SLDataFormat_PCM*)out.SLdataFormat) = slFormat;
#else
		ALenum format = 0;

		switch (bitsPerSample)
		{
			case 8:
				format = (channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8);
				break;
			case 16:
				format = (channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16);
				break;
			default:
				break;
		}

		OPsound out = {
			sampleRate,
			bitsPerSample,
			channels,
			format,
			NULL,
			NULL,
			NULL,
			dataSize,
			data
		};
#endif

		return out;
	}


	return ERR;
}
/*---------------------------------------------------------------------------*/
static ui64 DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, OPchar *pDecodeBuffer, ui64 ulBufferSize, ui64 ulChannels){
	OPint current_section;
	i64 lDecodeSize;
	ui64 ulSamples;
	i16 *pSamples;

	ui64 ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = fn_ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	if (ulChannels == 6){		
		pSamples = (i16*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6){
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			OPAUDIO_SWAP(pSamples[ulSamples+1], pSamples[ulSamples+2]);
			OPAUDIO_SWAP(pSamples[ulSamples+3], pSamples[ulSamples+5]);
			OPAUDIO_SWAP(pSamples[ulSamples+4], pSamples[ulSamples+5]);
		}
	}

	return ulBytesDone;
}
/*---------------------------------------------------------------------------*/
static OPint fetchOggData(OPsound* sound, i64 pos, i64 len){
	OggVorbis_File* ogg = (OggVorbis_File*)sound->dataSource;
	i64 length = 0;

#ifdef OPIFEX_ANDROID
#else
	length = DecodeOggVorbis(ogg, (i8*)sound->Data, sound->DataSize, sound->Channels);
#endif
	printf("Fetched %d\n", (OPint)length);

	return length;
}

static void resetOggData(OPsound* sound){
	OPLog("OPAudio::Reset");
	ov_raw_seek((OggVorbis_File*)sound->dataSource, 0);
}
/*---------------------------------------------------------------------------*/
OPsound OPAudio::ReadOgg(const OPchar* filename){
	// Open Ogg Stream
	ov_callbacks	sCallbacks;
	OggVorbis_File	*sOggVorbisFile = new OggVorbis_File();
	vorbis_info		*psVorbisInfo;

#ifdef OPIFEX_ANDROID
	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;
#else
	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;
#endif

	FILE* song = fopen(filename, "rb");
	if(song){
		printf("Song loaded!\n");
		// Create an OggVorbis file stream
		if (fn_ov_open_callbacks(song, sOggVorbisFile, NULL, 0, sCallbacks) == 0){
			i64 ulFrequency = 0, ulBufferSize = 0;
			OPint ulChannels = 0, ulFormat = 0, bitsPerSample = 0;


			// Get some information about the file (Channels, Format, and Frequency)
			psVorbisInfo = fn_ov_info(sOggVorbisFile, -1);
			if (psVorbisInfo)
			{
				ulFrequency = psVorbisInfo->rate;
				ulChannels = psVorbisInfo->channels;
				if (psVorbisInfo->channels == 1)
				{
					ulFormat = AL_FORMAT_MONO16;
					printf("Format AL_FORMAT_MONO16\n");
					// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency >> 1;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 2);
					bitsPerSample = 16;
				}
				else if (psVorbisInfo->channels == 2)
				{
					ulFormat = AL_FORMAT_STEREO16;
					printf("Format AL_FORMAT_STEREO16\n");
					// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 4);
					bitsPerSample = 32;
				}
				else if (psVorbisInfo->channels == 4)
				{
					#ifdef OPIFEX_ANDROID
					ulFormat = 1;
					#else
					ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
					#endif
					// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 2;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 8);
				}
				else if (psVorbisInfo->channels == 6)
				{
					#ifdef OPIFEX_ANDROID
					ulFormat = 2;
					#else
					ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
					#endif

					// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 3;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 12);
				}

				printf("Buffer size: %d Channels: %d\n", (OPint)ulBufferSize, (OPint)ulChannels);

				i64 length = fn_ov_pcm_total(sOggVorbisFile, -1);
				printf("Song length: %d\n", (OPint)length);

				//length = ulFrequency * 2;
				ui8* buff = (ui8*)OPalloc(sizeof(ui8) * length);
				DecodeOggVorbis(sOggVorbisFile, (OPchar*)buff, length, ulChannels);

				OPsound songResult = {
					#ifndef OPIFEX_ANDROID
					ulFrequency,
					bitsPerSample,
					ulChannels,
					ulFormat,
					#else
					(SLuint32)ulChannels,   
					(SLuint32)ulFrequency,
					(SLuint32)bitsPerSample,
					{0},
					#endif
					(void*)sOggVorbisFile,
					fetchOggData,
					resetOggData,
					length,
					buff
				};

				#ifdef OPIFEX_ANDROID
				bzero(song.SLdataFormat, sizeof(ui8) * 32);
				#endif
				
				return songResult;
			}
		}
	}
}