// TestHaptionDLL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "..\HaptionDLL\HaptionDLL.h"
#include <windows.h>

HINSTANCE hGetProcIDLL(nullptr);
typedef void (*hptn_ipc_init);

int LoadDll(std::string filename)
{
	HINSTANCE hGetProcIDDLL = (HINSTANCE)LoadLibrary("HaptionDLL.dll");
	if (!hGetProcIDDLL) 
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	hptn_ipc_init kk	= (hptn_ipc_init) GetProcAddress(hGetProcIDDLL,"ipc_init");

	return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
