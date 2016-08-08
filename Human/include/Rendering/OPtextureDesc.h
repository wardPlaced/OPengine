#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_DESC
#define OPENGINE_HUMAN_RENDERING_TEXTURE_DESC

struct OPtextureDesc;
typedef struct OPtextureDesc OPtextureDesc;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Human/include/Rendering/Enums/OPtextureFormat.h"
#include "./Human/include/Rendering/Enums/OPtextureWrap.h"

struct OPtextureDesc {
	ui32 width;
	ui32 height;
	OPtextureFormat format;
	OPtextureWrap wrap;
	OPtextureFilter filter;
};

#endif