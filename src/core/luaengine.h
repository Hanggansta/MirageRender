#ifndef LUAENGINE_H
#define LUAENGINE_H

// std includes
#include <iostream>
#include <string>

// mirage includes
#include "../macros.h"
#include "scene.h"
#include "material.h"

// lua includes
extern "C" {
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
}

struct DisplayInitInfo
{
	unsigned width;
	unsigned height;
	unsigned scale;
};

struct MultiThreadInitInfo
{
	unsigned rThreadCount;
};

namespace mirage
{

	namespace lua
	{

		extern lua_State *g_state;
		extern Scene *g_scene;
		extern DisplayInitInfo g_dispInitInfo;
		extern MultiThreadInitInfo g_mThreadInitInfo;

		extern void init(Scene *scene);
		extern void kill();
		extern void rerr(int status);
		extern void load(const std::string filename);
		extern void exec(const std::string filename, const std::string funcname);

		// Utility functions
		extern int lua_print_func(lua_State *L);

		// C++ Object constructors
		extern int lua_NewVector3_func(lua_State *L);
		extern int lua_NewVector4_func(lua_State *L);
		extern int lua_NewQuaternion_func(lua_State *L);
		extern int lua_NewQuaternionLookAt_func(lua_State *L);
		extern int lua_NewTransform_func(lua_State *L);
		extern int lua_NewCameraOrtho_func(lua_State *L);
		extern int lua_NewCameraPersp_func(lua_State *L);
		extern int lua_NewLightDir_func(lua_State *L);
		extern int lua_NewLightPoint_func(lua_State *L);
		extern int lua_NewLightSpot_func(lua_State *L);
		extern int lua_NewMesh_func(lua_State *L);
		extern int lua_NewSphere_func(lua_State *L);
		extern int lua_NewDiffMaterial_func(lua_State *L);
		extern int lua_NewEmisMaterial_func(lua_State *L);
		extern int lua_NewGlassMaterial_func(lua_State *L);
		extern int lua_NewSpecMaterial_func(lua_State *L);
		extern int lua_NewGlossyMaterial_func(lua_State *L);

		// C++ Scene object adders
		extern int lua_AddCamera_func(lua_State *L);
		extern int lua_AddLight_func(lua_State *L);
		extern int lua_AddMesh_func(lua_State *L);
		extern int lua_AddShape_func(lua_State *L);
		extern int lua_AddRayAccelerator_func(lua_State *L);

		// C++ Scene setting setters
		extern int lua_SetDisplayInitInfo_func(lua_State *L);
		extern int lua_SetMThreadInitInfo_func(lua_State *L);
		extern int lua_SetRadianceClamping_func(lua_State *L);
		extern int lua_SetMaxRecursion_func(lua_State *L);
		extern int lua_SetSceneSkyColor_func(lua_State * L);

	}

}

#endif // LUAENGINE_H
