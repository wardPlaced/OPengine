#ifndef OP_MATH_BOUNDINGBOX3D
#define OP_MATH_BOUNDINGBOX3D

#include "./Math/include/OPvec3.h"

struct OPboundingBox3D {
	OPvec3 min;
	OPvec3 max;
};

inline OPboundingBox3D OPboundingBox3DCreate(OPvec3 min, OPvec3 max) {
	OPboundingBox3D tmp = {
		min,
		max
	};
	return tmp;
}

inline void OPboundingBox3DAddVec3(OPboundingBox3D* bb, OPvec3* point) {
	bb->min += (*point);
	bb->max += (*point);
}

inline OPint OPboundingBox3DContainsVec3(OPboundingBox3D* bb, OPvec3* point) {
	return
		point->x >= bb->min.x && point->x <= bb->max.x &&
		point->y >= bb->min.y && point->y <= bb->max.y &&
		point->z >= bb->min.z && point->z <= bb->max.z;
}

inline OPint OPboundingBox3DContainsBoundingBox3D(OPboundingBox3D* bb, OPboundingBox3D* bb2) {
	return
		bb2->min.x >= bb->min.x && bb2->max.x <= bb->max.x &&
		bb2->min.y >= bb->min.y && bb2->max.y <= bb->max.y &&
		bb2->min.z >= bb->min.z && bb2->max.z <= bb->max.z;
}

enum BoundingBox3DCollision {
	BoundingBox3DCollision_None = 0,
	BoundingBox3DCollision_Inside = 1,
	BoundingBox3DCollision_Partial = 2
};

inline OPint OPboundingBox3DCollisionBoundingBox3D(OPboundingBox3D modelBounds, OPboundingBox3D playerBounds) {
	// TODO: AABB testing in 3D

	if (modelBounds.min.x <= playerBounds.min.x && modelBounds.max.x >= playerBounds.max.x &&
		modelBounds.min.y <= playerBounds.min.y && modelBounds.max.y >= playerBounds.max.y &&
		modelBounds.min.z <= playerBounds.min.z && modelBounds.max.z >= playerBounds.max.z)
		return BoundingBox3DCollision_Inside;

	if (modelBounds.max.x > playerBounds.min.x &&
		modelBounds.min.x < playerBounds.max.x &&
		modelBounds.max.y > playerBounds.min.y &&
		modelBounds.min.y < playerBounds.max.y &&
		modelBounds.max.z > playerBounds.min.z &&
		modelBounds.min.z < playerBounds.max.z) {
		return BoundingBox3DCollision_Partial;
	}

	return BoundingBox3DCollision_None;
}

#include "./Math/include/OPray3D.h"
inline i8 OPboundingBox3DRay3D(OPboundingBox3D b, OPray3D ray) {
	double tmin = -INFINITY, tmax = INFINITY;

	for (int i = 0; i < 3; ++i) {
		if (ray.direction[i] != 0.0) {
			double t1 = (b.min[i] - ray.position[i])/ray.direction[i];
			double t2 = (b.max[i] - ray.position[i])/ray.direction[i];

			tmin = OPMAX(tmin, OPMIN(t1, t2));
			tmax = OPMIN(tmax, OPMAX(t1, t2));
		} else if (ray.position[i] <= b.min[i] || ray.position[i] >= b.max[i]) {
			return false;
		}
	}

	return tmax > tmin && tmax > 0.0;
}

#endif
