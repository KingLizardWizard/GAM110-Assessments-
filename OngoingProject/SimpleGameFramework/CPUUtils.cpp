#include "stdafx.h"
#include "CPUUtils.h"

LONGLONG CPUUtils::GetPerformanceCounterTicks(uint32_t onCore)
{
	DWORD_PTR previousMask = MoveThisThreadToCore(onCore);

	// Query the value of the performance counter (in ticks)
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);

	SetThisThreadsAffinityMask(previousMask);
	return count.QuadPart;
}

LONGLONG CPUUtils::GetPerformanceCounterFrequency(uint32_t onCore)
{
	DWORD_PTR previousMask = MoveThisThreadToCore(onCore);

	// Query the value of the performance frequency (ticks per second)
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	SetThisThreadsAffinityMask(previousMask);
	return freq.QuadPart;
}

DWORD_PTR CPUUtils::MoveThisThreadToCore(uint32_t core)
{
	return SetThisThreadsAffinityMask(1 << core);
}

DWORD_PTR CPUUtils::SetThisThreadsAffinityMask(uint32_t mask)
{
	HANDLE thread = GetCurrentThread();

	// Set the thread to run on the specified core (based on the mask)
	return SetThreadAffinityMask(thread, mask);
}
