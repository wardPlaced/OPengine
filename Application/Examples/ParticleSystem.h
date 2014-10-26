#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_PARTICLESYSTEM;

void ExampleParticleSystemEnter(OPgameState* last);
OPint ExampleParticleSystemUpdate(OPtimer* time);
void ExampleParticleSystemExit(OPgameState* next);