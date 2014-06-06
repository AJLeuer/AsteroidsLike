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



class Time {
	
private:
	
	bool timerStarted ;
	
	chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start ;

	
public:
	
	inline Time() : timerStarted(false) {}
	
	inline void startTimer() {
		if (timerStarted) {
			cerr << "stopTimer() must be called before startTimer() can be called again for this Time object \n" ;
			throw exception() ;
		}
		timerStarted = true ;
		start = chrono::steady_clock::now() ;
	}
	
	/**
	 * 1 millisecond = 1000000 nanoseconds
	 *
	 * @return The time elapsed in nanoseconds.
	 */
	inline std::chrono::nanoseconds checkTimeElapsed() {
		//returns in micro or nanoseconds
		if (!timerStarted) {
			cerr << "stopTimer() can only be called after startTimer() has been called once \n" ;
			throw exception() ;
		}
		auto duration = chrono::steady_clock::now() - start ;
		return duration ;
	}
	
	/**
	 * Stops timer and returns the time elapsed.
	 *
	 * 1 millisecond = 1000000 nanoseconds
	 *
	 * @return The time elapsed in nanoseconds.
	 */
	inline std::chrono::nanoseconds stopTimer() {
		
		if (!timerStarted) {
			cerr << "stopTimer() can only be called after startTimer() has been called once \n" ;
			throw exception() ;
		}
		auto duration = chrono::steady_clock::now() - start ;
		timerStarted = false ;
		return duration ;
	}
} ;







#endif /* defined(__Intro_to_C____ClockTicks__) */
