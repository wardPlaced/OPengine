#ifndef OPIFEX_SCRIPTING
#define OPIFEX_SCRIPTING

#include "./Scripting/include/wrappers/V8Helper.h"
#include "./Human/include/Utilities/LoaderOPS.h"
#include "./Data/include/Stream.h"

#include "v8.h"
#include "./External/V8/include/libplatform/libplatform.h"
using namespace v8;

extern V8isolate* isolate;

void OPscriptInit();
//Handle<Script> OPscriptCompile(OPscript* script);
void OPscriptCompileAndRun(OPscript* script);
void OPscriptCompileAndRunStream(OPstream* stream);
void OPscriptLog(const char* data); 
void OPscriptRun(Handle<Script> script);
#endif