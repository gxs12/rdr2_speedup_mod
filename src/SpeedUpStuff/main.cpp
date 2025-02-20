

#include "../../inc/main.h"
#include "script.h"
#include "keyboard.h"


#include "speed_hack.h"

//template<class T>
BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		scriptRegisterAdditionalThread(hInstance, Speedhack::InintDLL);
		break;
	case DLL_PROCESS_DETACH:
		Speedhack::DetachDLL();
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	default:
		break;
	}
	return TRUE;
}