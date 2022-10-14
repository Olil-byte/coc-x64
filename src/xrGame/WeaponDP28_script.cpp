#include "pch_script.h"
#include "WeaponDP28.h"

using namespace luabind;

#pragma optimize("s",on)
void CWeaponDP28::script_register(lua_State* L)
{
	module(L)
	[
		class_<CWeaponDP28, CGameObject>("CWeaponDP28")
			.def(constructor<>())
	];
}