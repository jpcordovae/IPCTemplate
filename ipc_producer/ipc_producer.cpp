// ipc_producer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "..\HaptionDLL\HaptionDLL.h"
#include <chrono>
#include <thread>
#include <string>
#include <Windows.h>
#include "ipc_producer.h"

#define RETURN_SUCCESS EXIT_SUCCESS
#define RETURN_FAILURE EXIT_FAILURE

HINSTANCE hGetProcIDLL(nullptr);

typedef void(*hptn_ipc_init)();
typedef void(*hptn_ipc_write)(float *,float *, float *, float *);
typedef void(*hptn_ipc_read)(float *, float *, float *, float *);
typedef void(*hptn_ipc_close)();
typedef int(*hptn_ipc_start_read_thread)();
typedef int(*hptn_ipc_start_write_thread)();
typedef void(*hptn_ipc_stop_thread)();

hptn_ipc_init				ipc_init_ptr;
hptn_ipc_write				ipc_write_ptr;
hptn_ipc_read				ipc_read_ptr;
hptn_ipc_close				ipc_close_ptr;
hptn_ipc_start_read_thread	ipc_start_read_thread_ptr;
hptn_ipc_start_write_thread ipc_start_write_thread_ptr;
hptn_ipc_stop_thread		ipc_thread_stop_ptr;

int LoadDll(std::string filename)
{
	HINSTANCE hGetProcIDDLL = (HINSTANCE) LoadLibrary(filename.c_str());
	if (!hGetProcIDDLL)
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return RETURN_FAILURE;
	}

	ipc_init_ptr				= (hptn_ipc_init)				GetProcAddress(hGetProcIDDLL, "ipc_init");
	ipc_write_ptr				= (hptn_ipc_write)				GetProcAddress(hGetProcIDDLL, "ipc_write");
	ipc_read_ptr				= (hptn_ipc_read)				GetProcAddress(hGetProcIDDLL, "ipc_read");
	ipc_close_ptr				= (hptn_ipc_close)				GetProcAddress(hGetProcIDDLL, "ipc_close");
	ipc_start_read_thread_ptr	= (hptn_ipc_start_read_thread)	GetProcAddress(hGetProcIDDLL, "ipc_start_read_thread");
	ipc_start_write_thread_ptr	= (hptn_ipc_start_read_thread)	GetProcAddress(hGetProcIDDLL, "ipc_start_write_thread");
	ipc_thread_stop_ptr 		= (hptn_ipc_stop_thread)		GetProcAddress(hGetProcIDDLL, "ipc_stop_thread");

	return RETURN_SUCCESS;
}

std::thread main_thread;
std::chrono::milliseconds interval(5);

void threaded_function()
{
	static int i = 0;
	while (1)
	{
		std::cout << "send data to consumers" << std::endl;
		float p_A[] = { 1.0f + (float)i, 2.0f + (float)i, 3.0f + (float)i, 4.0f + (float)i };
		float r_A[] = { 2.0f + (float)i, 2.0f + (float)i, 3.0f + (float)i, 4.0f + (float)i };
		float p_B[] = { 3.0f + (float)i, 2.0f + (float)i, 3.0f + (float)i, 4.0f + (float)i };
		float r_B[] = { 4.0f + (float)i, 2.0f + (float)i, 3.0f + (float)i, 4.0f + (float)i };
		i++;
		ipc_write_ptr(p_A, r_A, p_B, r_B);
		std::cout << std::to_string(p_A[0]) << ", " << std::to_string(p_A[1]) << ", " << std::to_string(p_A[2]) << ", " << std::to_string(p_A[3]) << std::endl;
		std::cout << std::to_string(r_A[0]) << ", " << std::to_string(r_A[1]) << ", " << std::to_string(r_A[2]) << ", " << std::to_string(r_A[3]) << std::endl;
		std::cout << std::to_string(p_B[0]) << ", " << std::to_string(p_B[1]) << ", " << std::to_string(p_B[2]) << ", " << std::to_string(p_B[3]) << std::endl;
		std::cout << std::to_string(r_B[0]) << ", " << std::to_string(r_B[1]) << ", " << std::to_string(r_B[2]) << ", " << std::to_string(r_B[3]) << std::endl;
		std::this_thread::sleep_for(interval);
	}
}

int main()
{
	LoadDll("HaptionDLL.dll");
	ipc_start_write_thread_ptr();
	main_thread = std::thread(threaded_function);
	main_thread.join();

    std::cout << "Hello World!\n"; 
}
