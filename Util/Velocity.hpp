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
	bool * localContinueSignal ;
	
	Distance lastDistance = 0 ;
	
	/**
	 * The unit of time that will function as the denominator
	 * for this Velocity object. By default it is 64 milliseconds.
	 */
	chrono::nanoseconds baseTimeUnit = sixfour_milliseconds ;
	
	double lastVelocity = 0 ;
	
	static void calculateVelocity() ;
	
	

	
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
	 * @param mut A mutex shared with Velocity that should be locked when this velocity or distance is about to be deleted
	 */
	Velocity(const Distance * distance, mutex * mut, bool * localContinueSignal) :
		id(IDs),
		distance(distance),
		timer(new Timer()),
		velMutex(mut),
		localContinueSignal(localContinueSignal)
	{
		IDs++ ;
		timer->startTimer() ;
		
		velocityStorage.push_back(this) ;
		assert(velocityStorage.at(id)->id == id) ;
		
		if (velocityMonitorInit == false) {
			monitorVelocity() ;
		}
	}
	
	Velocity(const Distance * distance, mutex * mut, bool * localContinueSignal, chrono::nanoseconds baseTimeUnitOverride) :
		id(IDs),
		distance(distance),
		timer(new Timer()),
		velMutex(mut),
		localContinueSignal(localContinueSignal),
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
	
	Velocity(const Velocity & other) = delete ;
	Velocity(Velocity && other) = delete ;

	~Velocity() {
	
		if (id != -1) {
			velocityStorage.at(id) = nullptr ;
		}
		
		id = -1 ;
		
		distance = nullptr ;
			
		if (timer != nullptr) {
			delete timer ;
			timer = nullptr ;
		}
	}
	
	Velocity & operator=(const Velocity & rhs) = delete ;
	Velocity & operator=(Velocity && rhs)  = delete ;

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
	
    /* debug var */
	auto * vs = &velocityStorage ;
	/* end debug */
    
	for (auto i = 0 ; (i < velocityStorage.size()) && (velocityMonitorContinueSignal) ; i++) {
		
		/* Debug var */
		auto vs = Velocity::getVelocityStorage() ;
		
		if ((velocityStorage.at(i) != nullptr) && (velocityStorage.at(i)->id != -1) && (velocityStorage.at(i)->localContinueSignal)) {
			
			
			if (velocityStorage.at(i)->velMutex != nullptr) {
				if (velocityStorage.at(i)->velMutex->try_lock() == false) {
					continue ;
				}
			}
			else {
				continue ;
			}
			
			if ((velocityStorage.at(i) != nullptr) && (velocityStorage.at(i)->lastDistance != *velocityStorage.at(i)->distance)) {
				
				const N dist0 = *velocityStorage.at(i)->distance ;
				const auto time0 = velocityStorage.at(i)->timer->checkTimeElapsed() ;
				
				this_thread::sleep_for(velocityStorage.at(i)->baseTimeUnit) ;
				
				const N dist1 = *velocityStorage.at(i)->distance ;
				auto time1 = velocityStorage.at(i)->timer->checkTimeElapsed() ;
				
				const N totalDistance = dist1 - dist0 ;
				auto totalTime = time1 - time0 ;
				
				double velocity = (totalDistance / totalTime.count()) ;
				
				/* debug */
				if (velocity > 0) {
					;
				}
				/* end debug */
				
				velocityStorage.at(i)->lastVelocity = velocity ;
				velocityStorage.at(i)->lastDistance = *velocityStorage.at(i)->distance ;
			
			}
			if ((velocityStorage.at(i) != nullptr) && (velocityStorage.at(i)->velMutex != nullptr)) {
				velocityStorage.at(i)->velMutex->unlock() ;
			}
		}
	}
}




#endif
