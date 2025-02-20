#pragma once

#include "speed_hack.h"

namespace Speedhack
{
	TSimpleLock GTCLock;
	TSimpleLock QPCLock;

	LARGE_INTEGER initialtime64;
	LARGE_INTEGER initialoffset64;

	SpeedHackClass<LONGLONG> h_QueryPerformanceCounter;
	SpeedHackClass<DWORD> h_GetTickCount;
	SpeedHackClass<ULONGLONG> h_GetTickCount64;
	SpeedHackClass<DWORD> h_GetTime;

	void lock(TSimpleLock& d)
	{
		DWORD tid = GetCurrentThreadId();
		if (d.owner != tid)
		{
			do {
				Sleep(0);
			} while (InterlockedExchange(&d.count, 1) == 0);
			d.owner = tid;
		}
		else {
			InterlockedIncrement(&d.count);
		}
	}

	void unlock(TSimpleLock& d)
	{
		if (d.count == 1) {
			d.owner = 0;
		}
		InterlockedDecrement(&d.count);
	}


	BOOL WINAPI hackedQueryPerformanceCounter(LARGE_INTEGER* counter) {
		lock(QPCLock);
		LARGE_INTEGER falseLi;
		LARGE_INTEGER currentLi;
		BOOL result = realQueryPerformanceCounter(&currentLi);
		falseLi.QuadPart = h_QueryPerformanceCounter.get(currentLi.QuadPart);
		unlock(QPCLock);
		*counter = falseLi;
		return result;
	}

	DWORD WINAPI hackedGetTickCount() {
		lock(GTCLock);
		DWORD currentTime = realGetTickCount();
		DWORD res = h_GetTickCount.get(currentTime);
		unlock(GTCLock);
		return res;	
	}

	ULONGLONG WINAPI hackedGetTickCount64() {
		lock(GTCLock);
		ULONGLONG currentTime = realGetTickCount64();
		ULONGLONG res = h_GetTickCount64.get(currentTime);
		unlock(GTCLock);

		return res;
	}

	DWORD WINAPI hackedTimeGetTime() {
		DWORD currentTime = realTimeGetTime();
		return h_GetTime.get(currentTime);
	}


	void InitializeSpeedHack(double speed) {
		lock(QPCLock);
		lock(GTCLock);

		realQueryPerformanceCounter(&initialtime64);
		hackedQueryPerformanceCounter(&initialoffset64);

		h_QueryPerformanceCounter = SpeedHackClass<LONGLONG>(initialtime64.QuadPart, initialoffset64.QuadPart, speed);
		h_GetTickCount = SpeedHackClass<DWORD>(realGetTickCount(), hackedGetTickCount(), speed);
		h_GetTickCount64 = SpeedHackClass<ULONGLONG>(realGetTickCount64(), hackedGetTickCount64(), speed);
		h_GetTime = SpeedHackClass<DWORD>(realTimeGetTime(), hackedTimeGetTime(), speed);

		unlock(GTCLock);
		unlock(QPCLock);
	}

	void InintDLL()
	{
		realQueryPerformanceCounter(&initialtime64);
		initialoffset64 = initialtime64;

		h_QueryPerformanceCounter = SpeedHackClass<LONGLONG>(initialtime64.QuadPart, initialoffset64.QuadPart);
		h_GetTickCount = SpeedHackClass<DWORD>(realGetTickCount(), realGetTickCount());
		h_GetTickCount64 = SpeedHackClass<ULONGLONG>(realGetTickCount64(), realGetTickCount64());
		h_GetTime = SpeedHackClass<DWORD>(realTimeGetTime(), realTimeGetTime());

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)realQueryPerformanceCounter, &hackedQueryPerformanceCounter);
		DetourAttach(&(PVOID&)realGetTickCount, &hackedGetTickCount);
		DetourAttach(&(PVOID&)realGetTickCount64, &hackedGetTickCount64);
		DetourAttach(&(PVOID&)realTimeGetTime, &hackedTimeGetTime);
		DetourTransactionCommit();

		GTCLock = TSimpleLock();
		QPCLock = TSimpleLock();
	}

	void DetachDLL() {
		InitializeSpeedHack(1.0);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)realQueryPerformanceCounter, &hackedQueryPerformanceCounter);
		DetourDetach(&(PVOID&)realGetTickCount, &hackedGetTickCount);
		DetourDetach(&(PVOID&)realGetTickCount64, &hackedGetTickCount64);
		DetourDetach(&(PVOID&)realTimeGetTime, &hackedTimeGetTime);
		DetourTransactionCommit();	
	}
	
}