#pragma once

struct OPtextureDesc;
typedef struct OPtextureDesc OPtextureDesc;

#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Human/include/Rendering/Enums/OPtextureFormat.h"
#include "./Human/include/Rendering/Enums/OPtextureWrap.h"
#include "./Human/include/Rendering/Enums/OPtextureType.h"
#include "./Core/include/OPtypes.h"

struct OPtextureDesc {
	ui32 width;
	ui32 height;
	OPtextureFormat::Enum format;
	OPtextureFormat::Enum internalFormat;
	OPtextureWrap::Enum wrap;
	OPtextureFilter::Enum minfilter = OPtextureFilter::LINEAR;
	OPtextureFilter::Enum magfilter = OPtextureFilter::LINEAR;
	OPtextureType::Enum textureType;
	bool multisampled = false;
	bool mipmap = false;

	OPtextureDesc() {
		width = 0;
		height = 0;
		this->format = OPtextureFormat::RGBA;
		this->internalFormat = OPtextureFormat::RGBA;
		this->wrap = OPtextureWrap::CLAMP;
		this->minfilter = OPtextureFilter::LINEAR;
		this->magfilter = OPtextureFilter::LINEAR;
		this->textureType = OPtextureType::BYTE;
		multisampled = false;
		mipmap = false;
	}
	OPtextureDesc(ui32 width, ui32 height) {
		this->width = width;
		this->height = height;
		this->format = OPtextureFormat::RGBA;
		this->internalFormat = OPtextureFormat::RGBA;
		this->wrap = OPtextureWrap::CLAMP;
		this->minfilter = OPtextureFilter::LINEAR;
		this->magfilter = OPtextureFilter::LINEAR;
		this->textureType = OPtextureType::BYTE;
		multisampled = false;
		mipmap = false;
	}
	OPtextureDesc(ui32 width, ui32 height, OPtextureFormat::Enum format, OPtextureWrap::Enum wrap, OPtextureFilter::Enum filter) {
		this->width = width;
		this->height = height;
		this->format = format;
		this->internalFormat = format;
		this->wrap = wrap;
		this->minfilter = filter;
		this->magfilter = filter;
		this->textureType = OPtextureType::BYTE;
		multisampled = false;
		mipmap = false;
	}
	OPtextureDesc(ui32 width, ui32 height, OPtextureFormat::Enum format, OPtextureWrap::Enum wrap, OPtextureFilter::Enum filter, OPtextureType::Enum textureType) {
		this->width = width;
		this->height = height;
		this->format = format;
		this->internalFormat = format;
		this->wrap = wrap;
		this->minfilter = filter;
		this->magfilter = filter;
		this->textureType = textureType;
		multisampled = false;
		mipmap = false;
	}
	OPtextureDesc(ui32 width, ui32 height, OPtextureFormat::Enum format, OPtextureFormat::Enum internalFormat, OPtextureWrap::Enum wrap, OPtextureFilter::Enum filter, OPtextureType::Enum textureType) {
		this->width = width;
		this->height = height;
		this->format = format;
		this->internalFormat = internalFormat;
		this->wrap = wrap;
		this->minfilter = filter;
		this->magfilter = filter;
		this->textureType = textureType;
		multisampled = false;
		mipmap = false;
	}
};