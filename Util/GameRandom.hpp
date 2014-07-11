//
//  GameRandom.hpp
//  SpriteFight
//
//  Created by Adam James Leuer on 6/28/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef SpriteFight_GameRandom_hpp
#define SpriteFight_GameRandom_hpp

#include <iostream>
#include <limits>
#include <random>
#include <cmath>

#include "../Control/Configuration.h"

using namespace std ;

template<typename N>
class FastRand {
	
private:
	
	std::random_device dev ;
	std::uniform_int_distribution<N> dist ;
	std::default_random_engine rndm{dev()} ;
	
	N minimum ;
	N maximum ;
	
	static FastRand * initRandPosSetter() ;
	
	/* A conveniance random object that won't require initialization.
	 * Especially useful in constructors and initializing static values.
	 * A superior replacement for calling rand() 
	 */
	
	
public:
	
	static FastRand defaultRandom ;
	static FastRand * randPositionSetter ;
	
	FastRand(N _min, N _max) ;
	FastRand(const FastRand<N> & other) ;
	FastRand<N> & operator=(const FastRand<N> & rhs) ;
	~FastRand() ;
	N nextValue() ;
	N nextValue(N minimum, N maximum) ;
	template<typename R> R nextValue(R _min, R _max) ;
	N operator()() ;
	N operator()(N minimum, N maximum) ;
	
} ;


template<typename T>
FastRand<T>::FastRand(T _min, T _max) :
	minimum(_min),
	maximum(_max)
{
	//dev ;
	dist = uniform_int_distribution<T>(minimum, maximum) ;
	rndm.seed(dev()) ;
}

template<typename T>
FastRand<T>::FastRand(const FastRand<T> & other)
{
	std::random_device dev2 ;
	dist = uniform_int_distribution<T>(other.minimum, other.maximum) ;
	rndm.seed(dev2()) ;
}

template<typename T>
FastRand<T>::~FastRand(){}

template<typename T>
FastRand<T> & FastRand<T>::operator=(const FastRand<T> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_int_distribution<T>(rhs.minimum, rhs.maximum) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}

template<typename T>
T FastRand<T>::nextValue() {
	return dist(rndm) ;
}

template<typename T>
T FastRand<T>::nextValue(T minimum, T maximum) {
	std::uniform_int_distribution<T> dif_dist{minimum, maximum} ;
	return dif_dist(rndm) ;
}

template<typename T>
T FastRand<T>::operator()() {
	return nextValue() ;
}

template<typename T>
T FastRand<T>::operator()(T minimum, T maximum) {
	return nextValue(minimum, maximum) ;
}

template<typename T>
template<typename R>
R FastRand<T>::nextValue(R _min, R _max) {
	std::uniform_int_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rndm) ;
}

template<typename N>
FastRand<N> * FastRand<N>::randPositionSetter = initRandPosSetter() ;

template<typename N>
FastRand<N> * FastRand<N>::initRandPosSetter() {
    return new FastRand<N>(0, findLargest<N>({static_cast<N>(globalMaxX()), static_cast<N>(globalMaxY())})) ;
}

template<typename N>
FastRand<N> FastRand<N>::defaultRandom(std::numeric_limits<N>::min(), std::numeric_limits<N>::max()) ;
/* more useful random functions: */

template<typename N>
N randSignFlip(N n) {
	
	FastRand<unsigned short> rand(0, 1) ;
	bool pos = rand.nextValue() % 2 ;
	if (pos) {
		return n ;
	}
	else {
		return (n * -1) ;
	}
	
}

template <typename T>
T chooseAtRand(T n1, T n2) {
	FastRand<unsigned long> rand(0, 1) ;
	bool first = rand.nextValue() ;
	if (first) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}

/**
 * Returns a random enumeration of enum type
 * SomeEnum. SomeEnum should ideally use integer values starting
 * at zero as the underlying value for its enumerations.
 *
 * @param SomeEnum An enumeration type
 * @param N Some integer or floating point type
 * @param maximum The numerical value of the maximum enum of type SomeEnum
 */
template<typename SomeEnum, typename N>
SomeEnum randomEnumeration(N maximum) {
	
	FastRand<N> randm(0, maximum) ;
	N num = randm() ;
	return SomeEnum(num) ;
}


#endif
