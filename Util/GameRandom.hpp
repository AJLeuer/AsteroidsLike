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
#include <random>
#include <cmath>

#include "../Control/Configuration.h"

using namespace std ;

template<typename T>
class FastRand {
	
private:
	
	std::random_device dev ;
	std::uniform_int_distribution<T> dist ;
	std::default_random_engine rndm{dev()} ;
	
	T min ;
	T max ;
	
	static FastRand * initRandPosSetter() ;
	
public:
	
	static FastRand * randPositionSetter ;
	
	FastRand(T _min, T _max) ;
	FastRand(const FastRand<T> & other) ;
	FastRand<T> & operator=(const FastRand<T> & rhs) ;
	~FastRand() ;
	T nextValue() ;
	T nextValue(T min, T max) ;
	template<typename R> R nextValue(R _min, R _max) ;
	T operator()() ;
	T operator()(T min, T max) ;
	
} ;


template<typename T>
FastRand<T>::FastRand(T _min, T _max) :
	min(_min),
	max(_max)
{
	//dev ;
	dist = uniform_int_distribution<T>(min, max) ;
	rndm.seed(dev()) ;
}

template<typename T>
FastRand<T>::FastRand(const FastRand<T> & other)
{
	std::random_device dev2 ;
	dist = uniform_int_distribution<T>(other.min, other.max) ;
	rndm.seed(dev2()) ;
}

template<typename T>
FastRand<T>::~FastRand(){}

template<typename T>
FastRand<T> & FastRand<T>::operator=(const FastRand<T> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_int_distribution<T>(rhs.min, rhs.max) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}

template<typename T>
T FastRand<T>::nextValue() {
	return dist(rndm) ;
}

template<typename T>
T FastRand<T>::nextValue(T min, T max) {
	std::uniform_int_distribution<T> dif_dist{min, max} ;
	return dif_dist(rndm) ;
}

template<typename T>
T FastRand<T>::operator()() {
	return nextValue() ;
}

template<typename T>
T FastRand<T>::operator()(T min, T max) {
	return nextValue(min, max) ;
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
 * @param max The numerical value of the maximum enum of type SomeEnum
 */
template<typename SomeEnum, typename N>
SomeEnum randomEnumeration(N max) {
	
	FastRand<N> randm(0, max) ;
	N num = randm() ;
	return SomeEnum(num) ;
}


#endif
