#pragma once

#define NOMINMAX
#include <Windows.h>
#include <winnt.h>

#include <detours.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "syelog.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Winmm.lib")



namespace Speedhack
{
	// native original functions
	extern "C" {
		static BOOL(WINAPI* realQueryPerformanceCounter)(LARGE_INTEGER* performanceCounter) = QueryPerformanceCounter;
		static DWORD(WINAPI* realGetTickCount)() = GetTickCount;
		static ULONGLONG(WINAPI* realGetTickCount64)() = GetTickCount64;
		static DWORD(WINAPI* realTimeGetTime)() = timeGetTime;
	}

	class TSimpleLock
	{
	public:
		TSimpleLock() { 
			owner = GetCurrentThreadId(); 
		}
		long count = 0;
		DWORD owner;
	};

	void lock(TSimpleLock& d);
	void unlock(TSimpleLock& d);

	template<class T>
	class SpeedHackClass
	{
	private:
		double speed = 0;
		T initialoffset = NULL;
		T initialtime = NULL;
	public:
		SpeedHackClass() { speed = 1.0; }
		SpeedHackClass(T _initialtime, T _initialoffset, double _speed = 1.0) { 
			speed = _speed;			
			initialoffset = _initialoffset;			
			initialtime = _initialtime; 
		}
		double get_speed() const { 
			return speed; 
		}
		T get(T currentTime) { 
			T false_val = (T)((currentTime - initialtime) * speed) + initialoffset;			
			return (T)false_val; 
		}
		void set_speed(double _speed) { 
			speed = _speed; 
		}
	};

	BOOL WINAPI hackedQueryPerformanceCounter(LARGE_INTEGER* counter);
	DWORD WINAPI hackedGetTickCount();
	ULONGLONG WINAPI hackedGetTickCount64();
	DWORD WINAPI hackedTimeGetTime();

	void InitializeSpeedHack(double speed);
	void InintDLL();
	void DetachDLL();
}