// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPcman.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPfontManagerSetup(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value font(args[0]->ToString());
    OPfontManager* manager = OPfontManager::Create(*font, NULL, 0);

    Handle<Object> result = JS_NEW_OBJECT();
    OPfontManagerWrapper(result, manager);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfontManagerCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfont* font = JS_GET_ARG_PTR(args, 0, OPfont);

    OPfontManager* manager = OPfontManager::Create(font);

    Handle<Object> result = JS_NEW_OBJECT();
    OPfontManagerWrapper(result, manager);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfontManagerSetScale(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_ARG_PTR(args, 0, OPfontManager);

    ptr->scale = (f32)args[1]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerSetScaleSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_PTR(args.This(), OPfontManager);
    ptr->scale = (f32)args[0]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerSetAlign(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    if(args.Length() > 1) {
        OPfontManager* ptr = JS_GET_ARG_PTR(args, 0, OPfontManager);
		ptr->SetAlign((OPfontAlign)args[1]->IntegerValue());
    } else {
		OPFONTMANAGER_ACTIVE->SetAlign((OPfontAlign)args[1]->IntegerValue());
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerSetAlignSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_PTR(args.This(), OPfontManager);
	ptr->SetAlign((OPfontAlign)args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_ARG_PTR(args, 0, OPfontManager);

	ptr->Destroy();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerDestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_PTR(args.This(), OPfontManager);
	ptr->Destroy();

    JS_RETURN_NULL;
}

Handle<Object> OPfontManagerWrapper(Handle<Object> result, OPfontManager* ptr) {
    SCOPE_AND_ISOLATE;

    JS_SET_PTR(result, ptr);
    JS_SET_METHOD(result, "Destroy", _OPfontManagerDestroySelf);
    JS_SET_METHOD(result, "SetScale", _OPfontManagerSetScaleSelf);
    JS_SET_METHOD(result, "SetAlign", _OPfontManagerSetAlignSelf);

	return result;
}

void OPfontManagerWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Setup", _OPfontManagerSetup);
    JS_SET_METHOD(result, "Create", _OPfontManagerCreate);
    JS_SET_METHOD(result, "SetScale", _OPfontManagerSetScale);
    JS_SET_METHOD(result, "SetAlign", _OPfontManagerSetAlign);
    JS_SET_METHOD(result, "Destroy", _OPfontManagerDestroy);
    JS_SET_OBJECT(exports, "fontManager", result);

    Handle<Object> align = JS_NEW_OBJECT();
    JS_SET_NUMBER(align, "LEFT", (ui32)OPfontAlign::LEFT);
    JS_SET_NUMBER(align, "CENTER", (ui32)OPfontAlign::CENTER);
    JS_SET_NUMBER(align, "RIGHT", (ui32)OPfontAlign::RIGHT);
    JS_SET_OBJECT(exports, "FONTALIGN", align);

}

#endif
