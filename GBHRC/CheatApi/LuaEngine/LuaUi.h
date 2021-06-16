﻿#pragma once


typedef struct lua_State lua_State;
namespace LuaEngine
{
	class LuaExecution;

	class LuaUi
	{
	private:
		static int gui_add_element(lua_State*state);
		static int gui_new(lua_State* state);
		static int gui_destroy(lua_State* state);
	public:
		LuaUi(lua_State* state);

	};

}