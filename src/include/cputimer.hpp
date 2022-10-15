
#pragma once

#include <chrono>

typedef std::chrono::steady_clock::time_point		TIME_POINT;
typedef std::chrono::high_resolution_clock			CLOCK;
typedef std::chrono::milliseconds					MILLISECONDS;

class CpuTimer {
public:
	long long StartTime;
	long long LastCallToUpdate;
	long long CurrentCallToUpdate;
	long long Frequency;

	CpuTimer();

	void Reset();
	void Update();

	float GetTimeTotal();
	float GetTimeDelta();
};

class Stopwatch {
private:
	TIME_POINT Elapsed;
public:
	Stopwatch();

	void Restart();
	MILLISECONDS GetElapsed();
};
