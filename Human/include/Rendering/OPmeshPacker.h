#pragma once

struct OPmeshPacker;
typedef struct OPmeshPacker OPmeshPacker;

extern OPmeshPacker* OPMESHPACKER_ACTIVE;

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Data/include/OPstream.h"

struct OPmeshPacker{
	OPvertexArray vertexArray;
	OPvertexBuffer VertexBuffer;
	OPindexBuffer IndexBuffer;
	OPuint vertexElementOffset;
	OPuint vertexOffset;
	OPuint indexOffset;
	OPstream* vertices;
	OPstream* indices;
	OPint built;

	static OPmeshPacker* Create();
	OPmeshPacker* Init(); 
	OPuint AddVertexBuffer(ui32 vertexSize, void* verticesData, OPuint vertexCount);
	OPuint AddIndexBuffer(OPindexSize::Enum indexSize, void* indicesData, OPuint indexCount);
	void Build();
	void Bind();
	OPint Destroy();
};