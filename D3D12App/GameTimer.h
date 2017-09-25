//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.
	void SetSimulationSpeed(float fSpeed) { mSimulationSpeed = fSpeed; }

private:
	double mSecondsPerCount;
	double mDeltaTime;
	float mSimulationSpeed = 0.0f;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	static const unsigned long MAX_SAMPLE_COUNT = 15; // Maximum frame time sample count
	double          mFrameTime[MAX_SAMPLE_COUNT];
	unsigned long   mSampleCount = 0;

	bool mStopped;
};

#endif // GAMETIMER_H