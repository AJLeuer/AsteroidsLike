//
//  ClockTicks.h
//  Intro to C++
//
//  Created by Adam J. Leuer on 7/27/13.
//  Copyright (c) 2013 Adam J. Leuer. All rights reserved.
//

#ifndef __BasicTime__
#define __BasicTime__

#include <iostream>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>

using namespace std ;

//use clock() to get time since program start
class BasicTime {
	
private:
	bool timerStarted ;
	chrono::time_point<chrono::system_clock> start ;
	
	
public:
	BasicTime() : timerStarted(false) {}
	
	void startTimer() {
		if (timerStarted) {
			cout << "stopTimer() must be called before startTimer() can be called again for this BasicTime object" << endl ;
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
			cout << "stopTimer() can only be called after startTimer() has been called once" << endl ;
			throw new exception() ;
		}
		auto duration = chrono::system_clock::now() - start ;
		auto d = duration.count() ;
		return d ;
	}
	
	const long stopTimer() {
		//returns in microseconds (= 1000 milliseconds) - may need to convert
		if (!timerStarted) {
			cout << "stopTimer() can only be called after startTimer() has been called once" << endl ;
			throw new exception() ;
		}
		auto duration = chrono::system_clock::now() - start ;
		timerStarted = false ;
		return duration.count() ;
	}
};






#endif /* defined(__Intro_to_C____ClockTicks__) */
