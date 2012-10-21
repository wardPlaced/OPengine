#pragma once
#ifndef OPEngine_Data_Stream
#define OPEngine_Data_Stream
#include "./../Core/include/Core.h"

typedef struct{
	OPuint Length;   // bytes in this stream
	OPuint _pointer; // current location to write to
	ui8* Data;
} OPstream;

OPuint OPwrite(OPstream* stream, void* data, OPuint size){
	int len, ptr, i;
	ui8* D;

	if(!stream || !data) return 0;

	len = stream->Length;
	ptr = stream->_pointer;
	D   = stream->Data;

	// if we are out of space for this new element
	// double the size of the array
	if(ptr + size < len){
		D = (ui8*)OPrealloc(
			D,
			sizeof(ui8) * len * 2
		);
		stream->Length = len * 2;
	}

	// copy new data into the stream
	for(i = 0; i < size; D[ptr + i] = data[i++]);
	stream->_pointer += size;
}

#endif
