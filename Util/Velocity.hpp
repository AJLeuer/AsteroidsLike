//
//  Velocity.hpp
//  SpriteFight
//
//  Created by Adam James Leuer on 6/29/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef SpriteFight_Speed_hpp
#define SpriteFight_Speed_hpp

#include <chrono>
#include <thread>
#include <mutex>

#include "Timer.hpp"
#include "../Control/Configuration.h"

using namespace std ;

template<typename N>
struct Velocity {

#define Distance N
	
protected:
	
	const Distance * distance ;
	Timer * timer ;
	mutex * velMutex ;
	
	/**
	 * The unit of time that will function as the denominator
	 * for this Velocity object. By default it is one millisecond.
	 */
	chrono::nanoseconds baseTimeUnit = sixfour_milliseconds ;
	
	double lastVelocity = 0 ;
	
	bool cont = true ;
	
	void calculateVelocity() ;
	
public:
	
	/**
	 * Constructs a Velocity object. Distance is a pointer
	 * that should increase in value as the caller moves. When
	 * Velocity detects that distance has changed from its initial value,
	 * it will begin monitoring speed.
	 * 
	 * @param distance A pointer to the distance covered by the client class, which should change over time
	 */
	Velocity(const Distance * distance) :
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex())
	{
		timer->startTimer() ;
	}
	
	Velocity(const Distance * distance, chrono::nanoseconds newbaseTimeUnit) :
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex()),
		baseTimeUnit(newbaseTimeUnit)
	{
		timer->startTimer() ;
	}
	
	
	Velocity(const Velocity & other) :
		distance(other.distance),
		timer(other.timer),
		velMutex(other.velMutex),
		baseTimeUnit(other.baseTimeUnit) {}
	 
	
	Velocity(Velocity && other) :
		distance(other.distance),
		timer(other.timer),
		velMutex(other.velMutex),
		baseTimeUnit(other.baseTimeUnit)
	{
		other.distance = nullptr ;
		other.timer = nullptr ;
		other.velMutex = nullptr ;
	}
	
	~Velocity() {
		distance = nullptr ;
		delete timer ;
		delete velMutex ;
	}
	
	
	Velocity & operator=(const Velocity & rhs) {
		if (this != &rhs) {
			this->distance = rhs.distance ;
			this->timer = rhs.timer ;
			this->velMutex = rhs.velMutex ;
			baseTimeUnit = rhs.baseTimeUnit ;
		}
		return *this ;
	}
	 
	
	Velocity & operator=(Velocity && rhs) {
		if (this != &rhs) {
			this->distance = rhs.distance ;
			this->timer = rhs.timer ;
			this->velMutex = rhs.velMutex ;
			baseTimeUnit = rhs.baseTimeUnit ;
			rhs.distance = nullptr ;
			rhs.timer = nullptr ;
			rhs.velMutex = nullptr ;
		}
		return *this ;
	}
	
	void selfDestruct() {
		cont = false ;
		delete this ;
	}
	
	const double * operator()() {
		return &lastVelocity ;
	}
	
	const double * getValue() {
		return &lastVelocity ;
	}
	 
	/* Should only be called on its own thread */
	void monitorVelocity() {
		
		auto velocityMonitor = [&] {
			while(cont) {
				this->calculateVelocity() ;
			}
		} ;
		
		std::thread thr(velocityMonitor) ;
		thr.detach() ;
	}
	
} ;

 
/* Should only be called by monitorVelocity(),
	and only on its own thread */
template<typename N>
void Velocity<N>::calculateVelocity() {
		
	const auto time0 = timer->checkTimeElapsed() ;
	const N dist0 = *distance ;
	
	long long check = 0 ;
	while (check < this->baseTimeUnit.count()) {
		/* wait */
		auto chTime0 = timer->checkTimeElapsed() ;
		check += (chTime0 - time0).count() ;
	}
	
	auto time1 = timer->checkTimeElapsed() ;
	const N dist1 = *distance ;
	
	auto timeDiff = time1 - time0 ;
	const N dist = dist1 - dist0 ;
	auto ttime = timeDiff.count() ;
	
	double vel = (dist / ttime) ;
	
	/* debug */
	if (vel > 0) {
		;
	}
	/* end debug */
	
	lastVelocity = vel ;
}

#endif
