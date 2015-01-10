#pragma once

#include "./Engine.h"
#include "OPfbxSdk.h"
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"
#include "OPfbxSkin.h"
#include "OPfbxAnimation.h"
#include "Helpers.h"

typedef struct {
	OPfbxSdk SDK;
	OPfbxScene SCENE;

	OPfbxSkeleton Skeleton;
	OPfbxMeshData MeshData;

	OPfbxMeshPoly* Polys;
	OPfbxSkinBlendWeight* BlendWeights;
	OPfbxAnimation* Animation;

} OPfbxMesh;

OPint OPfbxMeshCreate(OPfbxMesh* mesh, const OPchar* filename) {

	mesh->SDK = OPfbxSdkCreate();
	mesh->SCENE;
	if(OPfbxSdkLoadScene(&mesh->SCENE, &mesh->SDK, filename) == 0) {

		OPlog("Step: FBX Scene Loaded");

		OPfbxSkeletonGet(&mesh->Skeleton, &mesh->SCENE);

		OPlog("Step: Skeleton");

		OPfbxMeshDataGet(&mesh->MeshData, &mesh->SCENE);

		OPlog("Step: MeshData");

		mesh->Polys = OPfbxMeshDataGetPolygons(&mesh->MeshData);

		mesh->BlendWeights = _skinBlendWeights(&mesh->MeshData, &mesh->Skeleton, &mesh->SCENE);
		OPlog("Step: skin");

		mesh->Animation = OPfbxAnimationGet(&mesh->MeshData, &mesh->Skeleton, &mesh->SCENE);

		OPlog("End FBX SDK");
		//OPfbxSdkDestroy(&SDK);

		return 0; // Success
	}

	OPfbxSdkDestroy(&mesh->SDK);

	OPlog("Failed to create the mesh from the given input file.");
	return 1;
}

OPint OPfbxMeshWriteToFile(OPfbxMesh* mesh, const OPchar* filename, OPint* features, OPint animOnly) {
	OPchar* output = OPstringCreateMerged(filename, ".opm");
	ofstream myFile(output, ios::binary);
	OPlog("Begin Writing File...");

	if (!animOnly) {
		// OPM File Format Version
		writeU16(&myFile, 1);

		OPlog("Feature[Model_Positions]: %d", features[Model_Positions]);
		OPlog("Feature[Model_Normals]: %d", features[Model_Normals]);
		OPlog("Feature[Model_UVs]: %d", features[Model_UVs]);
		OPlog("Feature[Model_Colors]: %d", features[Model_Colors]);
		OPlog("Feature[Model_Indices]: %d", features[Model_Indices]);
		OPlog("Feature[Model_Bones]: %d", features[Model_Bones]);
		OPlog("Feature[Model_Skinning]: %d", features[Model_Skinning]);
		OPlog("Feature[Model_Animations]: %d", features[Model_Animations]);

		ui32 feature = 0;
		if (features[Model_Positions]) feature += 0x01;
		if (features[Model_Normals]) feature += 0x02;
		if (features[Model_UVs]) feature += 0x04;
		if (features[Model_Colors]) feature += 0x100;
		if (features[Model_Indices]) feature += 0x10;
		if (features[Model_Bones]) feature += 0x20;
		if (features[Model_Skinning]) feature += 0x40;
		if (features[Model_Animations]) feature += 0x80;


		OPlog("Feature: %d", feature);
		// OPM File Features
		writeU32(&myFile, feature);

		OPint verticeCount = 0;
		OPint indCount = 0;
		for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
			verticeCount += mesh->Polys[i].Size;
			indCount += mesh->Polys[i].Size == 3 ? 3 : 6;
		}

		OPlog("Vertex Count: %d", verticeCount);
		// Vertex Count
		writeU32(&myFile, verticeCount);

		for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
			for (OPint j = 0; j < mesh->Polys[i].Size; j++) {

				if (features[Model_Positions]) {
					OPlg(".");
					OPvec3 pos = mesh->Polys[i].Position[j];
					writeF32(&myFile, pos.x);
					writeF32(&myFile, pos.y);
					writeF32(&myFile, pos.z);
				}

				if (features[Model_Normals]) {
					OPlg("-");
					OPvec3 norm = mesh->Polys[i].Normal[j];
					writeF32(&myFile, norm.x);
					writeF32(&myFile, norm.y);
					writeF32(&myFile, norm.z);
				}

				if (features[Model_Tangents]) {
					OPlg("%");
					OPvec4 tan = mesh->Polys[i].Tangent[j];
					writeF32(&myFile, tan.x);
					writeF32(&myFile, tan.y);
					writeF32(&myFile, tan.z);
				}

				if (features[Model_UVs]) {
					OPlg("=");
					OPvec2 uv = mesh->Polys[i].UV[j];
					writeF32(&myFile, uv.x);
					writeF32(&myFile, uv.y);
				}

				if (features[Model_Skinning]) {
					OPlg("+");
					OPint cp = mesh->Polys[i].ControlPointIndex[j];
					OPfbxSkinBlendWeight bw = mesh->BlendWeights[cp];
					OPint boneIndex;

					if (bw.c > 0) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b1);
						writeI16(&myFile, boneIndex);
					}
					if (bw.c > 1) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b2);
						writeI16(&myFile, boneIndex);
					}
					if (bw.c > 2) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b3);
						writeI16(&myFile, boneIndex);
					}
					if (bw.c > 3) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b4);
						writeI16(&myFile, boneIndex);
					}

					for (OPint k = 0; k < 4 - bw.c; k++) {
						writeI16(&myFile, -1);
					}


					if (bw.c > 0) {
						writeF32(&myFile, mesh->BlendWeights[cp].w1);
					}
					if (bw.c > 1) {
						writeF32(&myFile, mesh->BlendWeights[cp].w2);
					}
					if (bw.c > 2) {
						writeF32(&myFile, mesh->BlendWeights[cp].w3);
					}
					if (bw.c > 3) {
						writeF32(&myFile, mesh->BlendWeights[cp].w4);
					}

					for (OPint k = 0; k < 4 - bw.c; k++) {
						writeF32(&myFile, 0.0f);
					}

				}
			}
		}

		OPint offset = 0;
		if (features[Model_Indices]) {
			writeU32(&myFile, indCount / 3);

			for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
				if (mesh->Polys[i].Size == 3) {
					writeU16(&myFile, offset + 0);
					writeU16(&myFile, offset + 1);
					writeU16(&myFile, offset + 2);
					offset += 3;
				}
				else {
					writeU16(&myFile, offset + 0);
					writeU16(&myFile, offset + 1);
					writeU16(&myFile, offset + 2);

					writeU16(&myFile, offset + 0);
					writeU16(&myFile, offset + 2);
					writeU16(&myFile, offset + 3);
					offset += 4;
				}
			}
		}

		if (features[Model_Bones]) {
			OPchar* skel = OPstringCreateMerged(output, ".skel");
			ofstream mySkelFile(skel, ios::binary);
			writeI16(&mySkelFile, mesh->Skeleton.BoneCount);
			for (OPint i = 0; i < mesh->Skeleton.BoneCount; i++) {
				writeI16(&mySkelFile, mesh->Skeleton.Bones[i].Parent); // TODO: Bone Parent

				ui32 len = strlen(mesh->Skeleton.Bones[i].Name);
				writeU32(&mySkelFile, len);
				write(&mySkelFile, mesh->Skeleton.Bones[i].Name, len);
				for (i32 c = 0; c < 4; c++) {
					writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].x);
					writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].y);
					writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].z);
					writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].w);
				}
			}
		}
	}

	if (features[Model_Animations]) {
		OPlog("Track Count: %d", mesh->Animation->AnimationTrackCount);
		//writeI16(&myAnimFile, mesh->Animation->AnimationTrackCount);
		for (OPint i = 0; i < mesh->Animation->AnimationTrackCount; i++) {
			OPlog("Track Name: %s", mesh->Animation->Animations[i].Name);
			OPchar* prefix = OPstringCreateMerged(".", mesh->Animation->Animations[i].Name);
			OPchar* animName = OPstringCreateMerged(prefix, ".anim");
			OPchar* anim = OPstringCreateMerged(output, animName);			
			ofstream myAnimFile(anim, ios::binary);

			ui32 len = strlen(mesh->Animation->Animations[i].Name);
			writeU32(&myAnimFile, len);
			write(&myAnimFile, mesh->Animation->Animations[i].Name, len);

			writeU32(&myAnimFile, mesh->Animation->Animations[i].TotalFrames);
			for (OPint j = 0; j < mesh->Animation->Animations[i].TotalFrames * mesh->Skeleton.BoneCount; j++) {
				OPlg("%d ", j);
				OPmat4Log("Frame", mesh->Animation->Animations[i].JointTransform[j]);
				for (i32 c = 0; c < 4; c++) {
					writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].x);
					writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].y);
					writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].z);
					writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].w);
				}
			}

		}
	}

	return 0;
}

