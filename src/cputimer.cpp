
#include <Windows.h>
#include "include/cputimer.hpp"

// CPU Timer

CpuTimer::CpuTimer() {
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&t);

	Frequency = t.QuadPart;

	Reset();
}

void CpuTimer::Reset() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);

	StartTime = t.QuadPart;
	CurrentCallToUpdate = t.QuadPart;
	LastCallToUpdate = t.QuadPart;
}

void CpuTimer::Update() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);

	LastCallToUpdate = CurrentCallToUpdate;
	CurrentCallToUpdate = t.QuadPart;
}

float CpuTimer::GetTimeTotal() {
	// Return the total lifetime of the timer

	float diff = CurrentCallToUpdate - StartTime;

	return diff / Frequency;
}

float CpuTimer::GetTimeDelta() {
	// Return difference between updates

	float diff = CurrentCallToUpdate - LastCallToUpdate;

	return diff / Frequency;
}

// Stopwatch

Stopwatch::Stopwatch() {
	Elapsed = CLOCK::now();
}

void Stopwatch::Restart() {
	Elapsed = CLOCK::now();
}

MILLISECONDS Stopwatch::GetElapsed() {
	// Return how long the time between now and the last start / restart

	TIME_POINT now = CLOCK::now();
	return std::chrono::duration_cast<MILLISECONDS>(now - Elapsed);
}

