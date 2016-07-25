#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <stdint.h>
#include <windows.h>

namespace CPUUtils
{
	/**
	 * Get performance counter ticks from the given core
	 * 
	 * To ensure this works as desired the calling thread is moved to the given core then the
	 * request is processed then the thread is returned to its 'previous core'.
	 *
	 * @param onCore The on core. [Default Value = 0]
	 *
	 * @return The performance counter ticks.
	 */
	LONGLONG GetPerformanceCounterTicks(uint32_t onCore = 0);

	/**
	 * Get performance counter frequency, the number of ticks per second.
	 * 
	 * MSDN states that this will not change over the lifetime of the program.
	 * 
	 * To ensure this works as desired the calling thread is moved to the given core then the
	 * request is processed then the thread is returned to its 'previous core'.
	 *
	 * @param onCore The on core. [Default Value = 0]
	 *
	 * @return The performance counter frequency (ticks per second).
	 */
	LONGLONG GetPerformanceCounterFrequency(uint32_t onCore);

	/**
	 * Move the calling thread to the given core, starts at 0.
	 * 
	 * This is done by changing the thread affinity mask, the return value is the current threads
	 * previous thread affinity mask.
	 *
	 * @param core The CPU core to move the current executing thread to.
	 *
	 * @return The affinity mask that the thread previously had.
	 */
	DWORD_PTR MoveThisThreadToCore(uint32_t core);

	/**
	 * Set thread affinity mask, returns previous mask.
	 *
	 * @param mask The mask indicating which CPU cores the thread can run on.
	 *
	 * @return The affinity mask that the thread previously had.
	 */
	DWORD_PTR SetThisThreadsAffinityMask(uint32_t mask);
}

#endif // CPU_UTILS_H