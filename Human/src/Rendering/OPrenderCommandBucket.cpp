#include "./Human/include/Rendering/OPrenderCommandBucket.h"


OPrenderCommandDrawIndexed* OPrenderCommandBucket::CreateDrawIndexed() {
	void* result = allocator->alloc(allocator, sizeof(OPrenderCommandDrawIndexed));
	return (OPrenderCommandDrawIndexed*)result;
}

void OPrenderCommandBucket::Init(OPuint bucketSizeMax, OPcam** cameraPtr) {
	internalAllocator = OPallocatorLinear::Create(KB(bucketSizeMax));
	Init(bucketSizeMax, cameraPtr, internalAllocator->_rootAlloc);
}

void OPrenderCommandBucket::Init(OPuint bucketSizeMax, OPcam** cameraPtr, OPallocator* allocatorPtr) {

	bucketSize = bucketSizeMax;
	camera = cameraPtr;

	keys = (OPrenderCommandBucketKey*)OPalloc(sizeof(OPrenderCommandBucketKey) * bucketSize);
	copykeys = (OPrenderCommandBucketKey*)OPalloc(sizeof(OPrenderCommandBucketKey) * bucketSize);

	commands = (OPrenderCommand*)OPalloc(sizeof(OPrenderCommand) * bucketSize);
	keyIndex = 0;

	allocator = allocatorPtr;
}

void OPrenderCommandBucket::Destroy() {
	if (internalAllocator != NULL) {
		internalAllocator->Destroy();
		OPfree(internalAllocator);
	}
	if (keys != NULL) {
		OPfree(keys);
	}
	if (copykeys != NULL) {
		OPfree(copykeys);
	}
	if (commands != NULL) {
		OPfree(commands);
	}
}

OPrenderCommandBucket* OPrenderCommandBucket::Create(OPuint bucketSizeMax, OPcam** cameraPtr, OPallocator* allocatorPtr) {
	OPrenderCommandBucket* result = (OPrenderCommandBucket*)OPalloc(sizeof(OPrenderCommandBucket));
	result->Init(bucketSizeMax, cameraPtr, allocatorPtr);
	return result;
}

OPrenderCommandBucket* OPrenderCommandBucket::Create(OPuint bucketSizeMax, OPcam** cameraPtr) {
	OPrenderCommandBucket* result = (OPrenderCommandBucket*)OPalloc(sizeof(OPrenderCommandBucket));
	result->Init(bucketSizeMax, cameraPtr);
	return result;
}


i64 OPrenderCommandBucketSortGetKey(void* data, i64 index) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	return keys[index].key;
}

void* OPrenderCommandBucketSortGetEnitity(void* data, i64 index) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	return &keys[index];
}

void OPrenderCommandBucketSortSetEntity(void* data, i64 index, void* entity) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	OPrenderCommandBucketKey* key = (OPrenderCommandBucketKey*)entity;

	keys[index].key = key->key;
	keys[index].command = key->command;
}

void OPrenderCommandBucket::Sort() {
	// Sort the keys from lower order to higher order
	OPradixSort(
		keys,
		copykeys,
		keyIndex,
		OPrenderCommandBucketSortGetKey,
		OPrenderCommandBucketSortGetEnitity,
		OPrenderCommandBucketSortSetEntity);
}

void OPrenderCommandBucket::Flush(bool keep) {

	for (ui64 i = 0; i < keyIndex; i++) {
		keys[i].command->dispatch(keys[i].command->data, *camera);
	}


	keyIndex = 0;
	if (keep) return;

	allocator->clear(allocator);
}

void OPrenderCommandBucket::Render() {
	Sort();
	Flush();
}

void OPrenderCommandBucket::Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next) {
	//OPlogErr("OPrenderCommandBucket::Submit %d", keyIndex);
	ASSERT(keyIndex < bucketSize, "OPrenderCommandBucket is full");
	commands[keyIndex].data = data;
	commands[keyIndex].dispatch = dispatch;
	commands[keyIndex].next = next;
	keys[keyIndex].key = key;
	keys[keyIndex].command = &commands[keyIndex];

	keyIndex++;
}

void OPrenderCommandBucket::Submit(OPmodel* model, OPmat4* world, OPmaterial* material) {

	OPrenderCommandDrawIndexed::Submit(this, model, world, material);
}

void OPrenderCommandBucket::Submit(OPmesh* mesh, OPmat4* world, OPmaterial* material) {

	OPrenderCommandDrawIndexed::Submit(this, mesh, world, material);
}

void OPrenderCommandBucket::Submit(OPmodel* model, OPmat4* world, OPmaterial* material, bool materialPerMesh) {

	OPrenderCommandDrawIndexed::Submit(this, model, world, material, materialPerMesh);
}



// ui64 OPCOMMAND_BUCKET_DEFAULT_KEY_GEN(OPcommandBucket* commandBucket, OPmodel* model, OPtexture* texture, OPmaterial* material) {
//     ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
//     ui64 textureId = texture->Handle << 6;  // 07 - 12 bits
//     ui64 materialId = material->id << 12;   // 13 - 19 bits
//     ui64 renderTarget = 0 << 18;            // 20 - 26 bits
//     return meshId | textureId | materialId | renderTarget;
// }
