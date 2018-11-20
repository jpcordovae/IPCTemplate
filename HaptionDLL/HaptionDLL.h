#ifndef _HAPTION_DLL__
#define _HAPTION_DLL__

#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/sync/named_mutex.hpp"
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <windows.h>

#ifdef HAPTIONDLL_EXPORTS
#define HAPTION_API __declspec( dllexport )
#else
#define HAPTION_API __declspec( dllimport )
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

	HAPTION_API void ipc_init();
	HAPTION_API void ipc_read(float *,float *, float *, float *);
	HAPTION_API void ipc_write(float *, float *, float *, float *);
	HAPTION_API void ipc_close();
	HAPTION_API int ipc_start_read_thread();
	HAPTION_API int ipc_start_write_thread();
	HAPTION_API void ipc_stop_thread();

#ifdef __cplusplus
}
#endif

#endif
