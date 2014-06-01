//
//  ClockTicks.h
//  Intro to C++
//
//  Created by Adam J. Leuer on 7/27/13.
//  Copyright (c) 2013 Adam J. Leuer. All rights reserved.
//

#ifndef __Time__
#define __Time__

#include <iostream>
#include <chrono>

#include "Debug.h"

using namespace std ;

typedef chrono::system_clock microsecPrecisionClock ;
typedef chrono::steady_clock nanosecPrecisionClock ;


template<typename CurrentClock = nanosecPrecisionClock>
class Time {
	
private:
	
	bool timerStarted ;
	chrono::time_point<CurrentClock> start ;
	typedef chrono::duration<typename CurrentClock::rep, typename CurrentClock::period> TimerDuration ;
	
public:
	
	Time() : timerStarted(false) {}
	
	void startTimer() {
		if (timerStarted) {
			*(Debug::debugOutput) << "stopTimer() must be called before startTimer() can be called again for this Time object \n" ;
			throw new exception() ;
		}
		timerStarted = true ;
		start = CurrentClock::now() ;
	}
	
	/**
	 * 1 microsecond = 1000 milliseconds
	 * @return The time elapsed in microseconds or nanoseconds, depending on chosen precision.
	 */
	 TimerDuration checkTimeElapsed() {
		//returns in micro or nanoseconds
		if (!timerStarted) {
			*(Debug::debugOutput) << "stopTimer() can only be called after startTimer() has been called once \n" ;
			throw new exception() ;
		}
		auto duration = CurrentClock::now() - start ;
		return duration ;
	}
	
	/**
	 * Stops timer and returns the time elapsed.
	 *
	 */
	TimerDuration stopTimer() {
		
		if (!timerStarted) {
			*(Debug::debugOutput) << "stopTimer() can only be called after startTimer() has been called once" << endl ;
			throw new exception() ;
		}
		auto duration = CurrentClock::now() - start ;
		timerStarted = false ;
		return duration ;
	}
} ;







#endif /* defined(__Intro_to_C____ClockTicks__) */
