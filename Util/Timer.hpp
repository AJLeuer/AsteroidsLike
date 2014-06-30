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


class Timer {
	
private:
	
	bool timerStarted ;
	
	chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start ;

	
public:
	
	Timer() ;
	
	Timer(Timer && other) ;
	
	~Timer() ;
	
	Timer & operator=(Timer && rhs) ;
	
	/**
	 * Starts the timer
	 */
	void startTimer() ;
	
	/**
	 * 1 millisecond = 1000000 nanoseconds
	 *
	 * @return The time elapsed in nanoseconds.
	 */
	std::chrono::nanoseconds checkTimeElapsed() ;
	
	/**
	 * Stops timer and returns the time elapsed.
	 *
	 * 1 millisecond = 1000000 nanoseconds
	 *
	 * @return The time elapsed in nanoseconds.
	 */
	std::chrono::nanoseconds stopTimer() ;
} ;







#endif /* defined(__Intro_to_C____ClockTicks__) */
