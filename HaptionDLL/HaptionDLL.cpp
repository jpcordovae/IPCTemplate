// HaptionDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "HaptionDLL.h"
#include <iostream>
#include <chrono>
#include <thread>

#define HAPTION_SHM_NAME "haption_ipc"

using namespace boost::interprocess;

// IPC variables
float *position_A(nullptr);
float *rotation_A(nullptr);
float *position_B(nullptr);
float *rotation_B(nullptr);
named_mutex nMutex(open_or_create,"nMutex");

// local variables and controls
std::mutex localMutex;
float local_position_A[4];
float local_rotation_A[4];
float local_position_B[4];
float local_rotation_B[4];

/* for internal thread timing only*/
std::chrono::microseconds interval(1000);
std::thread thread_read;
std::thread thread_write;
bool keep_reading;
bool keep_writing;
bool mshm(false);

// shared memory
managed_shared_memory managed_shm(open_or_create, HAPTION_SHM_NAME, 64 * 1024/*64Kb of memory*/);
shared_memory_object shdmem{ open_or_create, "Boost", read_write };

void ipc_init()
{
	memset(local_position_A, 0x00, sizeof(float) * 4);
	memset(local_rotation_A, 0x00, sizeof(float) * 4);
	memset(local_position_B, 0x00, sizeof(float) * 4);
	memset(local_rotation_B, 0x00, sizeof(float) * 4);

	keep_reading = true;
	keep_writing = true;
	
	try {
		//std::cout << "loading managed memory" << std::endl;
		std::cout << "IPC init..." << std::endl;
	position_A = managed_shm.find_or_construct<float>("position_A")[4]();
	rotation_A = managed_shm.find_or_construct<float>("rotation_A")[4]();
	position_B = managed_shm.find_or_construct<float>("position_B")[4]();
	rotation_B = managed_shm.find_or_construct<float>("rotation_B")[4]();
	}
	catch (boost::interprocess::interprocess_exception &ex) 
	{
		std::cout << ex.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	localMutex.unlock();
	nMutex.unlock();
}

void ipc_internal_read()
{
	{
		scoped_lock<named_mutex>(nMutex);
		{
			scoped_lock<std::mutex>(localMutex);
			memcpy(local_position_A, position_A, 4 * sizeof(float));
			memcpy(local_rotation_A, rotation_A, 4 * sizeof(float));
			memcpy(local_position_B, position_B, 4 * sizeof(float));
			memcpy(local_rotation_B, rotation_B, 4 * sizeof(float));
		}
	}
}

void ipc_internal_write()
{
	{
		scoped_lock<named_mutex>(nMutex);
		{
			scoped_lock<std::mutex>(local_mutex);
			memcpy(position_A, local_position_A, 4 * sizeof(float));
			memcpy(rotation_A, local_rotation_A, 4 * sizeof(float));
			memcpy(position_B, local_position_B, 4 * sizeof(float));
			memcpy(rotation_B, local_rotation_B, 4 * sizeof(float));
		}
	}
}

void ipc_write(float *new_position_A, float *new_rotation_A, float *new_position_B, float *new_rotation_B)
{
	{
		scoped_lock<std::mutex>(local_mutex);
		memcpy(local_position_A, new_position_A, 4 * sizeof(float));
		memcpy(local_rotation_A, new_rotation_A, 4 * sizeof(float));
		memcpy(local_position_B, new_position_B, 4 * sizeof(float));
		memcpy(local_rotation_B, new_rotation_B, 4 * sizeof(float));
	}
}

void ipc_read(float *dest_position_A, float *dest_rotation_A, float *dest_position_B, float *dest_rotation_B)
{
	{
		scoped_lock<std::mutex>(localMutex);
		memcpy(dest_position_A, local_position_A, 4 * sizeof(float));
		memcpy(dest_rotation_A, local_rotation_A, 4 * sizeof(float));
		memcpy(dest_position_B, local_position_B, 4 * sizeof(float));
		memcpy(dest_rotation_B, local_rotation_B, 4 * sizeof(float));
	}
}

void internal_loop_read() 
{
	while (keep_reading)
	{
		ipc_internal_read();
		std::this_thread::sleep_for(interval);
	}
}

void internal_loop_write()
{
	while (keep_writing)
	{
		ipc_internal_write();
		std::this_thread::sleep_for(interval);
	}
}

int ipc_start_read_thread()
{
	thread_read = std::thread(internal_loop_read);
	thread_read.detach();
	return 0;
}

int ipc_start_write_thread()
{
	thread_write = std::thread(internal_loop_write);
	thread_write.detach();
	return 0;
}

void ipc_stop_thread()
{
	keep_reading = false;
	keep_writing = false;
}

void ipc_close()
{
	ipc_stop_thread();
}
