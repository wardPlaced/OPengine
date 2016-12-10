#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Data/include/OPstring.h"

inline void OPskeletonUpdateGlobalPoses(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->activePose[0];
	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		skeleton->globalPoses[i] = skeleton->globalPoses[skeleton->hierarchy[i]] * skeleton->activePose[i];
	}
}

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* bindPose, OPmat4* boneOffsets, OPmat4 invGlobalPose, i32 count, OPchar** names) {
	OPint sizeOfMatricesArray = sizeof(OPmat4)* count;
	OPint sizeOfHierarchy = sizeof(i16)* count;
	OPint sizeOfSkeleton = sizeof(OPskeleton);
	void* memoryBlock = OPalloc(sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 3);

	OPskeleton* skeleton = (OPskeleton*)memoryBlock;
	skeleton->hierarchyCount = count;
	skeleton->hierarchy = (i16*)((OPint)memoryBlock + sizeOfSkeleton);
	OPmemcpy(skeleton->hierarchy, hierarchy, sizeOfHierarchy);


	skeleton->globalPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 0);
	skeleton->activePose = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 1);
	skeleton->skinned = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 2);
	skeleton->bindPose = OPALLOC(OPmat4, count);
	skeleton->boneOffsets = OPALLOC(OPmat4, count);

	OPmemcpy(skeleton->bindPose, bindPose, sizeOfMatricesArray);
	OPmemcpy(skeleton->activePose, bindPose, sizeOfMatricesArray);
	OPmemcpy(skeleton->boneOffsets, boneOffsets, sizeOfMatricesArray);

	skeleton->globalInverse = invGlobalPose;

	skeleton->jointNames = (OPchar**)OPalloc(sizeof(OPchar*) * count);
	for (OPint i = 0; i < count; i++) {
		skeleton->jointNames[i] = OPstringCopy(names[i]);
		OPlogInfo("Joint Name (%d): %s", i, skeleton->jointNames[i]);
	}

	
	OPskeletonUpdate(skeleton);

	return skeleton;
}

void OPskeleton::Reset() {
	for (OPint i = 0; i < hierarchyCount; i++) {
		activePose[i] = bindPose[i];
	}
}

i16 OPskeletonGet(OPskeleton* skeleton, const OPchar* name) {
	for (ui16 i = 0; i < skeleton->hierarchyCount; i++) {
		if (OPstringEquals(name, skeleton->jointNames[i])) return i;
	}
	return -1;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->skinned[i] = skeleton->globalInverse * skeleton->globalPoses[i] * skeleton->boneOffsets[i];
	}
}

void OPskeletonDestroy(OPskeleton* skeleton) {
	OPfree(skeleton);
}


i16 OPskeleton::JointIndex(const OPchar* joint) {
	return OPskeletonGet(this, joint);
}


OPmat4 OPskeleton::JointMatrix(i32 joint) {
	return globalPoses[joint];
}

OPvec3 OPskeleton::JointPosition(i32 joint) {
	return globalPoses[joint].GetTranslation();
}