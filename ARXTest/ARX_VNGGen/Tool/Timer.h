#pragma once

namespace Timer
{
	typedef unsigned __int64 tsc_type;
	
	//---------------------------------------------------------------
	// Sample all three primary timers
	struct Sample
	{
		tsc_type		tsc;
		double			qpc; // seconds
		unsigned int	millis;
	};
	
	void GetSample(Sample * ptr);
	
	// delta(early,late)
	double DeltaSamples(const Sample & s1,const Sample & s2);
	
	// GetSeconds is seconds from startup using the Sample routines
	double GetSeconds();
	
	//---------------------------------------------------------------
	//	GetMillis uses the windows GetTickCount
	//	it's reliable, but only measures millisecond accuracy
	//	millis wraps 32 bits ever 49 days
	
	unsigned int GetMillis();

	//---------------------------------------------------------------
	// GetQPC uses QPC and converts to seconds
	//	it's a timer based on the system bus, usually around 50 MHz
	//	it can jump forward or back badly, the Sample stuff corrects for that
	double GetQPCSeconds();
	
	//---------------------------------------------------------------
	// tsc counts the number of clocks passed
	//	it's fast and always reliable when measured in clocks
	//	the tsc conversion to seconds does weird things on laptops with speedstep
		
	tsc_type	rdtsc();
	
	int			GetMHZ();
	double		GetSecondsPerTick();
	double		rdtscSeconds();
	
	//---------------------------------------------------------------
};