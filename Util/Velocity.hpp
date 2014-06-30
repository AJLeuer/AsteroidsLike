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
	 * for this Velocity object. By default it is 64 milliseconds.
	 */
	chrono::nanoseconds baseTimeUnit = sixfour_milliseconds ;
	
	double lastVelocity = 0 ;
	
	bool * cont = new bool(true) ;
	
	void calculateVelocity() ;
	
public:
	
	/**
	 * Constructs a Velocity object. Distance is a pointer
	 * that should increase in value as the caller moves. When
	 * Velocity detects that distance has changed from its initial value,
	 * it will begin monitoring speed.
	 * 
	 * @param distance A pointer to the distance covered by the client class, which Velocity expects to change over time
	 */
	Velocity(const Distance * distance) :
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex())
	{
		timer->startTimer() ;
	}
	
	Velocity(const Distance * distance, chrono::nanoseconds baseTimeUnitOverride) :
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex()),
		baseTimeUnit(baseTimeUnitOverride)
	{
		timer->startTimer() ;
	}
	
	Velocity(const Velocity & other) :
		distance(other.distance),
		timer(new Timer),
		velMutex(new mutex),
		cont(new bool(other.cont)),
		baseTimeUnit(other.baseTimeUnit)
	{
		timer->startTimer() ;
	}

	Velocity(Velocity && other) :
		distance(other.distance),
		timer(other.timer),
		velMutex(other.velMutex),
		cont(other.cont),
		baseTimeUnit(other.baseTimeUnit)
	{
		other.distance = nullptr ;
		other.timer = nullptr ;
		other.velMutex = nullptr ;
		other.cont = nullptr ;
	}

	~Velocity() {
	
		if (cont != nullptr) {
			
			*cont = false ;
		
			velMutex->lock() ;
		
			distance = nullptr ;
			
			if (timer != nullptr) {
				delete timer ;
				timer = nullptr ;
			}
			
			delete cont ;
		
			velMutex->unlock() ;
			delete velMutex ;
		}
	}
	

	Velocity & operator=(const Velocity & rhs) {
		
		if (this != &rhs) {
			delete timer ;
			delete velMutex ;
			delete cont ;
			
			this->distance = rhs.distance ;
			this->timer = new Timer ;
			this->velMutex = new mutex ;
			this->cont = new bool(rhs.cont) ;
			baseTimeUnit = rhs.baseTimeUnit ;
			
			timer->startTimer() ;
		}
		return *this ;
	}

	

	Velocity & operator=(Velocity && rhs) {
		
		if (this != &rhs) {
			delete timer ;
			delete velMutex ;
			
			this->distance = rhs.distance ;
			this->timer = rhs.timer ;
			this->velMutex = rhs.velMutex ;
			this->cont = rhs.cont ;
			baseTimeUnit = rhs.baseTimeUnit ;
			
			rhs.distance = nullptr ;
			rhs.timer = nullptr ;
			rhs.velMutex = nullptr ;
			rhs.cont = nullptr ;
		}
		return *this ;
	}

	const double * getValue() {
		return &lastVelocity ;
	}
	 
	
	void monitorVelocity() {
		
		auto velocityMonitorLambda = [this] () -> void {
			while((cont != nullptr) && (timer != nullptr) && (*cont)) {
				velMutex->lock() ;
				this->calculateVelocity() ;
				velMutex->unlock() ;
			}
		} ;
		
		std::thread thr(velocityMonitorLambda) ;
		thr.detach() ;
	}
	
} ;

 
/* Should only be called by monitorVelocity(),
	and only on its own thread */
template<typename N>
void Velocity<N>::calculateVelocity() {
	
	const N dist0 = *distance ;
	const auto time0 = timer->checkTimeElapsed() ;
	
	this_thread::sleep_for(baseTimeUnit) ;
	
	const N dist1 = *distance ;
	auto time1 = timer->checkTimeElapsed() ;
	
	const N totalDistance = dist1 - dist0 ;
	auto totalTime = time1 - time0 ;

	double velocity = (totalDistance / totalTime.count()) ;
	
	/* debug */
	if (velocity > 0) {
		;
	}
	/* end debug */
	
	lastVelocity = velocity ;
}

#endif
