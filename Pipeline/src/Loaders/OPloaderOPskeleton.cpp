#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

void OPskeletonAddLoader() {
	OPassetLoader loader = {
		".skel",
		"Models/",
		sizeof(OPskeleton),
		(OPint(*)(OPstream*, void**))OPloaderOPskeletonLoad,
		(OPint(*)(void*))OPloaderOPskeletonUnload,
		NULL
	};
	OPCMAN.AddLoader(&loader);
}

OPint OPloaderOPskeletonLoad(OPstream* str, OPskeleton** skeleton) {
	//OPstream* str = OPreadFile(filename);

	i32 boneCount = str->I16();

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Skeleton cannot contain more than OPSKELETON_MAX_BONE_COUNT");



	i16 hierarchy[OPSKELETON_MAX_BONE_COUNT];
	OPchar* jointNames[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 pose[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 offsets[OPSKELETON_MAX_BONE_COUNT];


	OPmat4 globalInverseBindPose = OPMAT4_IDENTITY;
	for (ui32 j = 0; j < 4; j++) {
		for (ui32 k = 0; k < 4; k++) {
			globalInverseBindPose[k][j] = str->F32();
		}
	}

	for (i32 i = 0; i < boneCount; i++) {
		hierarchy[i] = str->I16();
		jointNames[i] = str->String();
		OPlog(jointNames[i]);

		for (ui32 j = 0; j < 4; j++) {
			for (ui32 k = 0; k < 4; k++) {
				pose[i][k][j] = str->F32();
			}
		}
		// pose[i] = OPmat4Transpose(pose[i]);

		for (ui32 j = 0; j < 4; j++) {
			for (ui32 k = 0; k < 4; k++) {
				offsets[i][k][j] = str->F32();
			}
		}

	}

	*skeleton = OPskeletonCreate(hierarchy, pose, offsets, globalInverseBindPose, boneCount, jointNames);

	//OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPskeletonUnload(OPskeleton* skeleton) {
	OPskeletonDestroy(skeleton);
	return 1;
}