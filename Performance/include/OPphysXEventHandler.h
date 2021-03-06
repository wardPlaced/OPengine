#pragma once

#include "./Performance/include/OPphysX.h"

#ifdef OPIFEX_OPTION_PHYSX

// TYPES

struct OPphysXTrigger {
	PxTriggerPair* triggerPairs;
	ui32 count;
};

struct OPphysXContact {
	PxContactPairHeader header;
	const PxContactPair* contactPairs;
	ui32 count;
};

class OPphysXEventHandler : public PxSimulationEventCallback
{
public:
	OPphysXEventHandler(void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact)) {
		_onTrigger = onTrigger;
		_onContact = onContact;
	}
private:
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
		if(_onContact == NULL) return;
		OPphysXContact contact = {
			pairHeader,
			pairs,
			nbPairs
		};
		_onContact(contact);
	}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {
		if(_onTrigger == NULL) return;
		OPphysXTrigger trigger = {
			pairs,
			count
		};
		_onTrigger(trigger);
	}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ){}

	void(*_onTrigger)(OPphysXTrigger);
	void(*_onContact)(OPphysXContact);
};

#endif
