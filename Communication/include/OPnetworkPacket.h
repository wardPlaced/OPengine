#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/OPnetworkPlatform.h"

#define MAX_PACKET_SIZE 1024

struct OPnetworkPacket {
	i8 buffer[MAX_PACKET_SIZE];
	ui16 size = 0;
	ui16 pos = 0;
	
	inline i8 I8() {
		return buffer[pos++];
	}

	inline void I8(i8 c) {
		size++;
		buffer[pos++] = c;
	}
	
	inline i16 I16() {
		i16 val = *(i16*)&buffer[pos];
		i16 result = ntohs(val);
		pos += 2;
		return result;
	}

	inline void I16(i16 s) {
		i16 val = htons(s);
		*(i16*)&buffer[pos] = val;
		pos += 2;
		size += 2;
	}
	
	inline i32 I32() {
		i32 val = *(i32*)&buffer[pos];
		i32 result = ntohl(val);
		pos += 4;
		return result;
	}

	inline void I32(i32 s) {
		i32 val = htonl(s);
		*((i32*)(&buffer[pos])) = val;
		pos += 4;
		size += 4;
	}

	void Str(const OPchar* str);
	OPchar* Str();
};