#include <windows.h>
#include <cstdio>
#include "Detours.h"

static LONG dwSlept = 0;

// Target pointer for the uninstrumented Sleep API.
//
static VOID(WINAPI * TrueSleep)(DWORD dwMilliseconds) = Sleep;

// Detour function that replaces the Sleep API.
//
VOID WINAPI TimedSleep(DWORD dwMilliseconds)
{
	// Save the before and after times around calling the Sleep API.
	DWORD dwBeg = GetTickCount();
	TrueSleep(dwMilliseconds);
	DWORD dwEnd = GetTickCount();

	InterlockedExchangeAdd(&dwSlept, dwEnd - dwBeg);

	printf("Duration: %ld\n", dwSlept);
}

// DllMain function attaches and detaches the TimedSleep detour to the
// Sleep target function.  The Sleep target function is referred to
// through the TrueSleep target pointer.
//
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (DetourIsHelperProcess()) {
		return TRUE;
	}

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueSleep, TimedSleep);
		DetourTransactionCommit();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueSleep, TimedSleep);
		DetourTransactionCommit();
		break;
	}

	return TRUE;
}
