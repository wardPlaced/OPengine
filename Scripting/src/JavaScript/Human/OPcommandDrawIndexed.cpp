// void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material);
// void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture);
// void OPcommandDrawIndexedSubmit(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material, OPtexture* texture);
// inline void OPcommandDrawIndexedSubmit(OPcommandBucket* commandBucket, OPmodelTextured* model, OPmaterial* material) {
// 	OPcommandDrawIndexedSubmit(commandBucket, &model->model, material, model->texture);
// }

#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPcman.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPcommandDrawIndexedWrapperCreate(Handle<Object> result, OPrenderCommandBucket* cb);

JS_RETURN_VAL _OPcommandDrawIndexedSubmit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* commandBucket = JS_GET_ARG_PTR(args, 0, OPrenderCommandBucket);
	OPmodel* model = JS_GET_ARG_PTR(args, 1, OPmodel);
	OPmat4* world = JS_GET_ARG_PTR(args, 2, OPmat4);
    OPmaterial* material = JS_GET_ARG_PTR(args, 3, OPmaterial);
	commandBucket->Submit(model, world, material);

    JS_RETURN_NULL;
}

void OPcommandDrawIndexedWrapperCreate(Handle<Object> result, OPrenderCommandBucket* cb) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, cb);
}

void OPcommandDrawIndexedWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Submit", _OPcommandDrawIndexedSubmit);
    JS_SET_NUMBER(result, "size", sizeof(OPrenderCommandDrawIndexed));
    JS_SET_OBJECT(exports, "commandDrawIndexed", result);

}

#endif
