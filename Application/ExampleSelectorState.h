#pragma once

#include "./OPengine.h"

#define ExampleCount 29

extern OPgameState GS_EXAMPLE_SELECTOR;

// Determines whether the Example State will display the example
// as available to select. They might not be available because
// an option was not selected to be built. Ex: Oculus support
extern OPint       GS_EXAMPLE_AUDIO_AVAILABLE,
                   GS_EXAMPLE_DEFERRED_AVAILABLE,
                   GS_EXAMPLE_MODEL_AVAILABLE,
                   GS_EXAMPLE_MYO_AVAILABLE,
                   GS_EXAMPLE_OCULUS_AVAILABLE,
                   GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE,
                   GS_EXAMPLE_PHYSICS_AVAILABLE,
                   GS_EXAMPLE_SKINNING_AVAILABLE,
                   GS_EXAMPLE_SPINE_AVAILABLE,
                   GS_EXAMPLE_TEXTURED_AVAILABLE,
                   GS_EXAMPLE_SCRIPTING_AVAILABLE,
                   GS_EXAMPLE_SPHERICALCUBE_AVAILABLE,
                   GS_EXAMPLE_SPRITE_AVAILABLE,
                   GS_EXAMPLE_IMGUI_AVAILABLE,
                   GS_EXAMPLE_SHADOWS_AVAILABLE,
                   GS_EXAMPLE_MESH_BUILDER_AVAILABLE,
                   GS_EXAMPLE_FREEFLIGHT_AVAILABLE,
                   GS_EXAMPLE_FMOD_AVAILABLE,
                   GS_EXAMPLE_SPRITESYSTEM_AVAILABLE,
                   GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE,
                   GS_EXAMPLE_MOUSE_INTERSECT_AVAILABLE,
                    GS_EXAMPLE_SERVER_CLIENT_AVAILABLE,
                    GS_EXAMPLE_CUBE_MAP_AVAILABLE,
                    GS_EXAMPLE_MATERIAL_AVAILABLE,
				GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE,
				GS_EXAMPLE_ASIO_AVAILABLE, 
				GS_EXAMPLE_RAKNET_AVAILABLE,
	GS_EXAMPLE_OCORNUT_IMGUI_AVAILABLE,
	GS_EXAMPLE_LUA_AVAILABLE,
	GS_EXAMPLE_OPENVR_AVAILABLE;

// All of the example game states
extern OPgameState GS_EXAMPLE_AUDIO,
                   GS_EXAMPLE_DEFERRED,
                   GS_EXAMPLE_MODEL,
                   GS_EXAMPLE_MYO,
                   GS_EXAMPLE_OCULUS,
                   GS_EXAMPLE_PARTICLESYSTEM,
                   GS_EXAMPLE_PHYSICS,
                   GS_EXAMPLE_SKINNING,
                   GS_EXAMPLE_SPINE,
                   GS_EXAMPLE_TEXTURED,
                   GS_EXAMPLE_SCRIPTING,
                   GS_EXAMPLE_SPHERICALCUBE,
                   GS_EXAMPLE_SPRITE,
                   GS_EXAMPLE_IMGUI,
                   GS_EXAMPLE_SHADOWS,
                   GS_EXAMPLE_MESH_BUILDER,
                   GS_EXAMPLE_FREEFLIGHT,
                   GS_EXAMPLE_FMOD,
                   GS_EXAMPLE_SPRITESYSTEM,
                   GS_EXAMPLE_PHYSICSCHARACTER,
                   GS_EXAMPLE_MOUSE_INTERSECT,
                    GS_EXAMPLE_SERVER_CLIENT,
                    GS_EXAMPLE_CUBE_MAP,
                    GS_EXAMPLE_MATERIAL,
	GS_EXAMPLE_COMMAND_BUCKET,
	GS_EXAMPLE_ASIO,
	GS_EXAMPLE_RAKNET,
	GS_EXAMPLE_OCORNUT_IMGUI,
	GS_EXAMPLE_LUA,
	GS_EXAMPLE_OPENVR;
