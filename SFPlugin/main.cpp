#include "main.h"
SAMPFUNCS *SF = new SAMPFUNCS();
bool isEnabled = false;

bool __stdcall onOutcomingPackets(stRakNetHookParams* params)
{
	if (params->packetId == 251 && isEnabled)
	{
		int action;
		unsigned short value;
		params->bitStream->ResetReadPointer();
		params->bitStream->IgnoreBits(8);
		params->bitStream->Read(action);

		if (action == 8)
		{
			params->bitStream->Read(value);
			if (value == 256)
				return false;
		}
	}
	return true;
}

void __stdcall commandHandler(std::string params)
{
	if (params.empty())
	{
		isEnabled ^= true;
		SF->getSAMP()->getChat()->AddChatMessage(0xEB2E4A, "Evolve-Client: {FFFFFF}%s", isEnabled ? "Работа в свернутом режиме включена" : "Работа в свернутом режиме выключена");
	}
}

void __stdcall mainloop()
{
	static bool initialized = false;

	if (!initialized)
	{
		if (GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized())
		{
			initialized = true;
			SF->getRakNet()->registerRakNetCallback(RakNetScriptHookType::RAKHOOK_TYPE_OUTCOMING_PACKET, onOutcomingPackets);
			SF->getSAMP()->registerChatCommand("aafk", commandHandler);
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	if (dwReasonForCall == DLL_PROCESS_ATTACH)
		SF->initPlugin(mainloop, hModule);

	return TRUE;
}
