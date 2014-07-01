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
#include <vector>

#include <assert.h>

#include "Timer.hpp"
#include "../Control/Configuration.h"

using namespace std ;

extern bool velocityMonitorContinueSignal ;

template<typename N>
struct Velocity {

#define Distance N
	
protected:
	
	static bool velocityMonitorInit ;
	static unsigned IDs ;
	static vector<Velocity *> velocityStorage ;
	
	
	int id ;
	const Distance * distance ;
	Timer * timer ;
	mutex * velMutex ;
	
	Distance lastDistance = 0 ;
	
	/**
	 * The unit of time that will function as the denominator
	 * for this Velocity object. By default it is 64 milliseconds.
	 */
	chrono::nanoseconds baseTimeUnit = sixfour_milliseconds ;
	
	double lastVelocity = 0 ;
	
	static void calculateVelocity() ;
	
	bool * cont = new bool(true) ;
	

	
public:
	
	static const vector<Velocity *> * getVelocityStorage() { return & velocityStorage ; }
	
	static void monitorVelocity() ;
	
	/**
	 * Constructs a Velocity object. Distance is a pointer
	 * that should increase in value as the caller moves. When
	 * Velocity detects that distance has changed from its initial value,
	 * it will begin monitoring speed.
	 * 
	 * @param distance A pointer to the distance covered by the client class, which Velocity expects to change over time
	 */
	Velocity(const Distance * distance) :
		id(IDs),
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex())
	{
		IDs++ ;
		timer->startTimer() ;
		
		velocityStorage.push_back(this) ;
		assert(velocityStorage.at(id)->id == id) ;
		
		if (velocityMonitorInit == false) {
			monitorVelocity() ;
		}
	}
	
	Velocity(const Distance * distance, chrono::nanoseconds baseTimeUnitOverride) :
		id(IDs),
		distance(distance),
		timer(new Timer()),
		velMutex(new mutex()),
		baseTimeUnit(baseTimeUnitOverride)
	{
		IDs++ ;
		timer->startTimer() ;
		
		velocityStorage.push_back(this) ;
		assert(velocityStorage.at(id)->id == id) ;
		
		if (velocityMonitorInit == false) {
			monitorVelocity() ;
		}
	}
	
	Velocity(const Velocity & other) :
		id(IDs),
		distance(other.distance),
		timer(new Timer),
		velMutex(new mutex),
		cont(new bool(other.cont)),
		baseTimeUnit(other.baseTimeUnit)
	{
		IDs++ ;
		timer->startTimer() ;
		
		velocityStorage.push_back(this) ;
		assert(velocityStorage.at(id)->id == id) ;
		
		if (velocityMonitorInit == false) {
			monitorVelocity() ;
		}
	}

	Velocity(Velocity && other) :
		id(other.id),
		distance(other.distance),
		timer(other.timer),
		velMutex(other.velMutex),
		cont(other.cont),
		baseTimeUnit(other.baseTimeUnit)
	{
		other.id = -1 ;
		other.distance = nullptr ;
		other.timer = nullptr ;
		other.velMutex = nullptr ;
		other.cont = nullptr ;
		
        velocityStorage.at(id) = this ;
		assert(velocityStorage.at(id)->id == id) ;
        
		
		if (velocityMonitorInit == false) {
			monitorVelocity() ;
		}
	}

	~Velocity() {
	
		if (cont != nullptr) {
			
			if (id != -1) {
				velocityStorage.at(id) = nullptr ;
			}
			
			id = -1 ;
			
			*cont = false ;
		
			velMutex->lock() ;
			
			std::thread thrd ;
		
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
			
			velocityStorage.at(id) = nullptr ;
			
			delete timer ;
			delete velMutex ;
			delete cont ;
			
			this->id = IDs ;
			IDs++ ;
			
			this->distance = rhs.distance ;
			this->timer = new Timer ;
			this->velMutex = new mutex ;
			this->cont = new bool(rhs.cont) ;
			baseTimeUnit = rhs.baseTimeUnit ;
			
			velocityStorage.push_back(this) ;
			assert(velocityStorage.at(id)->id == id) ;
			
			timer->startTimer() ;
		}
		return *this ;
	}

	

	Velocity & operator=(Velocity && rhs) {
		
		if (this != &rhs) {
			
			delete timer ;
			delete velMutex ;
			delete cont ;
			
			this->id = rhs.id ;
			this->distance = rhs.distance ;
			this->timer = rhs.timer ;
			this->velMutex = rhs.velMutex ;
			this->cont = rhs.cont ;
			baseTimeUnit = rhs.baseTimeUnit ;
			
            velocityStorage.at(id) = this ;
			assert(velocityStorage.at(id)->id == id) ;
			
			rhs.id = -1 ;
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
	 
} ;


template<typename N>
bool Velocity<N>::velocityMonitorInit = false ;

template<typename N>
unsigned Velocity<N>::IDs = 0 ;

template<typename N>
vector<Velocity<N> *> Velocity<N>::velocityStorage = vector<Velocity<N> *>() ;

template<typename N>
void Velocity<N>::monitorVelocity() {
	
	auto velocityMonitorLambda = [&] () -> void {
		while(velocityMonitorContinueSignal) {
			calculateVelocity() ;
		}
	} ;
	
	std::thread thr(velocityMonitorLambda) ;
	velocityMonitorInit = true ;
	thr.detach() ;
	
}

template<typename N>
void Velocity<N>::calculateVelocity() {
	
	auto * vs = &velocityStorage ;
	
	for (auto i = 0 ; i < velocityStorage.size() ; i++) {
		
		if ((velocityStorage.at(i) != nullptr) && (velocityStorage.at(i)->id != -1)) {
			
			auto current = velocityStorage.at(i) ;
			
			if (current->lastDistance != *(current->distance)) {
				
				current->velMutex->lock() ;
				
				const N dist0 = *current->distance ;
				const auto time0 = current->timer->checkTimeElapsed() ;
				
				this_thread::sleep_for(current->baseTimeUnit) ;
				
				const N dist1 = *current->distance ;
				auto time1 = current->timer->checkTimeElapsed() ;
				
				const N totalDistance = dist1 - dist0 ;
				auto totalTime = time1 - time0 ;
				
				double velocity = (totalDistance / totalTime.count()) ;
				
				/* debug */
				if (velocity > 0) {
					;
				}
				/* end debug */
				
				current->lastVelocity = velocity ;
				current->lastDistance = *current->distance ;
				
				current->velMutex->unlock() ;
			}
		}
	}
}




#endif
