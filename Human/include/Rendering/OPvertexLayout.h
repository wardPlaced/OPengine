#pragma once

struct OPvertexLayout;
struct OPvertexLayoutBuilder;

typedef struct OPvertexLayout OPvertexLayout;
typedef struct OPvertexLayoutBuilder OPvertexLayoutBuilder;

// Assuming we'll never need more than 32 attributes in a vertex layout, if you do, make it larger
#define OPVERTEX_LAYOUT_BUILDER_MAX 32

#include "./Human/include/Rendering/OPshader.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Human/include/Rendering/Enums/OPattributes.h"
struct OPeffect;

struct OPvertexLayout {
	ui16 count;
	OPshaderAttribute attributes[OPVERTEX_LAYOUT_BUILDER_MAX];
	ui32 stride;

	void Init(ui16 attributeCount, const OPchar** names, OPattributeTypes::Enum * types, ui8* counts);
	//void Init(OPshaderAttribute* attributes, ui8 count);
	void SetOffsets(OPeffect* effect);
	void Log();
	void Destroy();
};


struct OPvertexLayoutBuilder {
    ui32 index;
    const OPchar* names[OPVERTEX_LAYOUT_BUILDER_MAX];
	OPattributeTypes::Enum types[OPVERTEX_LAYOUT_BUILDER_MAX];
	ui8 counts[OPVERTEX_LAYOUT_BUILDER_MAX];

	OPvertexLayoutBuilder() {

	}
	OPvertexLayoutBuilder(ui32 features) {
		Init(features);
	}

	OPvertexLayoutBuilder* Init();
	OPvertexLayoutBuilder* Init(ui32 features);
	OPvertexLayoutBuilder* Add(const OPchar* name, OPattributeTypes::Enum attrType, ui32 count);
	OPvertexLayoutBuilder* Add(OPattributes::Enum attribute);
	OPvertexLayout Build();
};