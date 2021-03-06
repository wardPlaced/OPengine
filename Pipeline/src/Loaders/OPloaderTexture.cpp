#include "./Pipeline/include/Loaders/OPloaderTexture.h"
#include "./Core/include/Assert.h"
#include "./Core/include/OPdebug.h"
#include "./Data/include/OPstring.h"

#include "./Pipeline/include/Loaders/stb_image.h"


OPint OPloaderTextureFromStreamOffset(OPstream* str, OPuint offset, OPtexture** image, OPtextureFilter::Enum filter) {
	TIMED_BLOCK;

	ASSERT(str != NULL, "Image not found.");

	ui8* data;
	i32 width, height, components;

	data = stbi_load_from_memory(str->Data + offset, str->Size - offset, &width, &height, &components, 0);
    if(data == NULL) {
        OPlogErr("STBI Error %d", str->Size);
    	return 0;
    }

	OPtexture* tex = OPNEW(OPtexture());

	ui16 w = width, h = height;
	OPtextureDesc desc;
	desc.width = w;
	desc.height = h;

	if (components == 1) {
		desc.format = desc.internalFormat = OPtextureFormat::RED;
	}
	else if (components == 2) {
		desc.format = desc.internalFormat = OPtextureFormat::RG;
	}
	else if (components == 3) {
		desc.format = desc.internalFormat = OPtextureFormat::RGB;
	}
	else if (components == 4) {
		desc.format = desc.internalFormat = OPtextureFormat::RGBA;
	}
	desc.wrap = OPtextureWrap::REPEAT;
	desc.minfilter = filter;
	desc.magfilter = filter;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc);
	tex->SetData(data);

#ifdef _DEBUG
	tex->source = OPstringCopy(str->Source);
#endif

	// clean up
	free(data); // Clean up load png

	*image = tex;

	return 1;
}

OPint OPloaderTextureFromFile(const OPchar* filename, OPtexture** image) {
	OPstream* str = OPfile::ReadFromFile(filename);
    OPint result = OPloaderTextureFromStreamOffset(str, 0, image, OPtextureFilter::LINEAR);
    str->Destroy();
    return result;
}

OPint OPloaderTextureFromStream(OPstream* str, OPtexture** image) {
    return OPloaderTextureFromStreamOffset(str, 0, image, OPtextureFilter::LINEAR);
}

OPint OPloaderTextureReload(OPstream* str, OPtexture** image) {
	OPlog("Reload Image Texture");
	//OPstream* str = OPreadFile(filename);
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);
	i32 result = OPloaderTextureFromStreamOffset(str, 0, &resultTex, OPtextureFilter::LINEAR);
	if (result) {
		OPmemcpy(*image, resultTex, sizeof(OPtexture));
		OPfree(resultTex);
	}
	return result;
}

OPint OPloaderTextureUnload(OPtexture* image) {
	image->Destroy();
	OPfree(image);
	return 1;
}

OPassetLoader OPASSETLOADER_TEXTURE_JPG = {
	".jpg",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};

OPassetLoader OPASSETLOADER_TEXTURE_PNG = {
	".png",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};

OPassetLoader OPASSETLOADER_TEXTURE_JPEG = {
	".jpeg",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};


OPassetLoader OPASSETLOADER_TEXTURE_TGA = {
	".tga",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};

OPassetLoader OPASSETLOADER_TEXTURE_BMP = {
	".bmp",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};

OPassetLoader OPASSETLOADER_TEXTURE_PSD = {
	".psd",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};

OPassetLoader OPASSETLOADER_TEXTURE_GIF = {
	".gif",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPloaderTextureFromStream,
	(OPint(*)(void*))OPloaderTextureUnload,
	(OPint(*)(OPstream*, void**))OPloaderTextureReload
};
