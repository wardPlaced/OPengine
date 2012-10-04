#pragma once
#ifndef OPEngine_Core_MathHelpers
#define OPEngine_Core_MathHelpers

#include "Target.h"
#include "Types.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux math libs
#include <math.h>
#include <stdlib.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows math libs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#elif defined(OPIFEX_ANDROID)	// android math libs

#endif

#define OPpi 3.14159265359
#define OPe  2.71828182846

OPint OPceil(OPfloat f);
OPint OPfloor(OPfloat f);

OPfloat OPabsf(OPfloat f);
OPfloat OPabs(OPfloat f);
OPint	OPabsi(OPint i);

OPfloat OPsin(OPfloat f);
OPfloat OPcos(OPfloat f);
OPfloat OPtan(OPfloat f);

OPfloat OPasin(OPfloat f);
OPfloat OPacos(OPfloat f);
OPfloat OPatan(OPfloat f);

OPfloat OPpow(OPfloat b, OPfloat exp);
OPfloat OPsqrt(OPfloat f);
OPfloat OPlog10(OPfloat f);
OPfloat OPlog2(OPfloat f);
OPfloat OPln(OPfloat f);

OPfloat OPrandom();
OPfloat OPrandRange(OPfloat min, OPfloat max);
#endif
