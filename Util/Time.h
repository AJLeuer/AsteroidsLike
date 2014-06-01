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

//use clock() to get time since program start
class Time {
	
private:
	
	bool timerStarted ;
	chrono::time_point<chrono::system_clock> start ;
	
	
public:
	
	Time() : timerStarted(false) {}
	
	void startTimer() {
		if (timerStarted) {
			*(Debug::debugOutput) << "stopTimer() must be called before startTimer() can be called again for this Time object" << endl ;
			throw new exception() ;
		}
		timerStarted = true ;
		start = chrono::system_clock::now() ;
	}
	
	/**
	 * @return The time elapsed in microseconds
	 */
	const long checkTimeElapsed() {
		//returns in microseconds (= 1000 milliseconds) - may need to convert
		if (!timerStarted) {
			*(Debug::debugOutput) << "stopTimer() can only be called after startTimer() has been called once" << endl ;
			throw new exception() ;
		}
		auto duration = chrono::system_clock::now() - start ;
		auto d = duration.count() ;
		return d ;
	}
	
	/**
	 * Stops timer and returns the time elapsed
	 * in microseconds (= 1000 milliseconds) - may need to convert
	 */
	const long stopTimer() {
		
		if (!timerStarted) {
			*(Debug::debugOutput) << "stopTimer() can only be called after startTimer() has been called once" << endl ;
			throw new exception() ;
		}
		auto duration = chrono::system_clock::now() - start ;
		timerStarted = false ;
		return duration.count() ;
	}
} ;







#endif /* defined(__Intro_to_C____ClockTicks__) */
