/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once
#define NOMINMAX
#include <Windows.h>

#include "script.h"
#include "keyboard.h"

#include <vector>
#include <string>

using namespace std;

class ScriptInput
{
public:
	static bool ScriptSwitchPressed(DWORD key)
	{
		return IsKeyJustUp(key);
	}
	static void ScriptInputBeep()
	{
		AUDIO::_STOP_SOUND_WITH_NAME("SELECT", "HUD_SHOP_SOUNDSET");
		AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", 1, 0);
	}
};
