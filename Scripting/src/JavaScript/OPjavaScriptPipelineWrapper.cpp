#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

void OPscriptNodeWrapperPipeline(Handle<Object> exports) {
    OPvoxelGeneratorWrapper(exports);
    OPtexture2DWrapper(exports);
    OPloaderWrapper(exports);
    OPloaderVoxelsWrapper(exports);
    OPsprite2DWrapper(exports);
    OPspriteSystemWrapper(exports);
    OPfmodWrapper(exports);
}

#endif
