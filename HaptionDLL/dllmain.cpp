// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "HaptionDLL.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//TODO: send message of attachment
		//std::cout << "IPC comms init..." << std::endl;
		//ipc_init();
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		//std::cout << "closing IPC comms..." << std::endl;
		//ipc_close();
        break;
    }
    return TRUE;
}

