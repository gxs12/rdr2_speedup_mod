/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"
#include "scriptmenu.h"
#include "keyboard.h"
#include "speed_hack.h"
#include "scriptinfo.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class ScriptController
{
	float speed_hack_multiplier;
	bool isActive = false;
	bool speedHackOn = false;
	void setSpeedHack(float f) {
		Speedhack::InitializeSpeedHack(f);
	}
	void OnFrame() {

		if (isActive) {
			bool fnd = false;

			for (int i : cTaskIdSkips) {
				if (TASK::GET_IS_TASK_ACTIVE(PLAYER::PLAYER_PED_ID(), i)) {
					fnd = true;
				}
			}

			if (fnd && !speedHackOn) {
				WAIT(100);
				Speedhack::InitializeSpeedHack(25.0);
				speedHackOn = true;
			}
			if (!fnd) {
				reset();
				speedHackOn = false;
			}

		}
	};
public:
	ScriptController(float multiplier) : speed_hack_multiplier(multiplier) {}
	~ScriptController() {
		reset();
	}
	bool isActivated() { return isActive; }
	void activate() { isActive = true; }
	void deactivate() { isActive = false; }

	void reset() {
		setSpeedHack(1.0);
	}
	void Update()
	{
		OnFrame();
	}
	void OnSelect() {

	}
};

unsigned long get_ulong(string str) {
	boost::trim(str);
	return stoul(str.c_str());
}

float get_float(string str) {
	boost::trim(str);
	return stof(str.c_str());
}

void main()
{

	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("SpeedUpStuff.ini", pt);

	unsigned long activate_key;
	float speed_hack_multiplier;
	try {
		activate_key = get_ulong(pt.get<string>("activate_key"));
	}
	catch (exception e) {
		activate_key = 118;
	}
	try {
		speed_hack_multiplier = get_float(pt.get<string>("speed_hack_multiplier"));
	}
	catch (exception e) {
		speed_hack_multiplier = 25;
	}

	auto scriptController = new ScriptController(speed_hack_multiplier);

	while (true)
	{
		if (!scriptController->isActivated() && ScriptInput::ScriptSwitchPressed(activate_key))
		{
			ScriptInput::ScriptInputBeep();
			scriptController->activate();
			scriptController->OnSelect();
		}
		if (scriptController->isActivated() && ScriptInput::ScriptSwitchPressed(activate_key)) {
			ScriptInput::ScriptInputBeep();
			scriptController->reset();
			scriptController->deactivate();
			ScriptInput::ScriptInputBeep();
			ScriptInput::ScriptInputBeep();
		}
		scriptController->Update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}