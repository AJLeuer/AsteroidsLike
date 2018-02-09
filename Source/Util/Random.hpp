//
//  Random.hpp
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
class Randm {
	
protected:
	
	std::random_device dev ;
	std::uniform_int_distribution<N> dist ;
    std::default_random_engine rndm = std::default_random_engine(dev()) ;
	
	N minimum ;
	N maximum ;
	
	static Randm * initRandPosSetter() ;
	
public:
	
	static Randm defaultRandom ;
	static Randm * randPositionSetter ;
	
	Randm(N _min, N _max) ;
    
	Randm(const Randm<N> & other) ;
    
    template<typename M>
    Randm(const Randm<M> & other) ;
    
	Randm & operator=(const Randm<N> & rhs) ;
	
	~Randm() ;
	
	N nextValue() ;
	
	N nextValue(N minimum, N maximum) ;
	
	template<typename R>
	R nextValue(R _min, R _max) ;
	
	N operator()() ;
	
	N operator()(N minimum, N maximum) ;
	
} ;

template<>
class Randm<float> {
	
protected:
	
	std::random_device dev ;
	std::uniform_real_distribution<float> dist ;
	std::default_random_engine rndm = std::default_random_engine(dev()) ;
	
	float minimum ;
	float maximum ;
	
	static Randm * initRandPosSetter() ;
	
public:
	
	static Randm defaultRandom ;
	static Randm * randPositionSetter ;
	
	Randm(float _min, float _max) ;
	Randm(const Randm<float> & other) ;
	Randm & operator=(const Randm<float> & rhs) ;
	~Randm() ;
	float nextValue() ;
	float nextValue(float minimum, float maximum) ;
	template<typename R> R nextValue(R _min, R _max) ;
	float operator()() ;
	float operator()(float minimum, float maximum) ;
	
	
} ;

template<>
class Randm<double> {
	
protected:
	
	std::random_device dev ;
	std::uniform_real_distribution<double> dist ;
	std::default_random_engine rndm = std::default_random_engine(dev()) ;
	
	double minimum ;
	double maximum ;
	
	static Randm * initRandPosSetter() ;
	
public:
	
	static Randm defaultRandom ;
	static Randm * randPositionSetter ;
	
	Randm(double _min, double _max) ;
	Randm(const Randm<double> & other) ;
	Randm & operator=(const Randm<double> & rhs) ;
	~Randm() ;
	double nextValue() ;
	double nextValue(double minimum, double maximum) ;
	template<typename R> R nextValue(R _min, R _max) ;
	double operator()() ;
	double operator()(double minimum, double maximum) ;
	
	
} ;

template<typename N>
Randm<N>::Randm(N _min, N _max) :
	minimum(_min),
	maximum(_max)
{
	//dev ;
	dist = uniform_int_distribution<N>(minimum, maximum) ;
	rndm.seed(dev()) ;
}

template<typename N>
Randm<N>::Randm(const Randm<N> & other)
{
	std::random_device dev2 ;
	dist = uniform_int_distribution<N>(other.minimum, other.maximum) ;
	rndm.seed(dev2()) ;
}

template<typename N>
template<typename M>
Randm<N>::Randm(const Randm<M> & other) {
    this->minimum(static_cast<N>(other.minimum)) ;
    this->maximum(static_cast<N>(other.maximum)) ;
    
    dist = uniform_int_distribution<N>(minimum, maximum) ;
	rndm.seed(dev()) ;
}

template<typename N>
Randm<N>::~Randm(){}

template<typename N>
Randm<N> & Randm<N>::operator=(const Randm<N> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_int_distribution<N>(rhs.minimum, rhs.maximum) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}

template<typename N>
N Randm<N>::nextValue() {
	return dist(rndm) ;
}

template<typename N>
N Randm<N>::nextValue(N minimum, N maximum) {
	std::uniform_int_distribution<N> dif_dist{minimum, maximum} ;
	return dif_dist(rndm) ;
}

template<typename N>
N Randm<N>::operator()() {
	return nextValue() ;
}

template<typename N>
N Randm<N>::operator()(N minimum, N maximum) {
	return nextValue(minimum, maximum) ;
}

template<typename N>
template<typename R>
R Randm<N>::nextValue(R _min, R _max) {
	std::uniform_int_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rndm) ;
}



template<typename N>
Randm<N> Randm<N>::defaultRandom(std::numeric_limits<N>::min(), std::numeric_limits<N>::max()) ;


template<typename N>
Randm<N> * Randm<N>::randPositionSetter = initRandPosSetter() ;
 
template<typename N>
Randm<N> * Randm<N>::initRandPosSetter() {
    return new Randm<N>(0, Util::findLargest<N>({static_cast<N>(globalMaxX()), static_cast<N>(globalMaxY())})) ;
}
 

/* more useful random functions: */

template<typename N>
N randSignFlip(N n) {
	
	Randm<unsigned short> rand(0, 1) ;
	bool pos = rand.nextValue() % 2 ;
	if (pos) {
		return n ;
	}
	else {
		return (n * -1) ;
	}
	
}

template <typename T>
T chooseAtRand(T t1, T t2) {
	Randm<unsigned long> rand(0, 1) ;
	bool first = rand.nextValue() ;
	if (first) {
		return t1 ;
	}
	else {
		return t2 ;
	}
}

/**
 * Returns a random enumeration of enum type
 * SomeEnum. SomeEnum should ideally use integer values starting
 * at zero as the underlying value for its enumerations.
 *
 * @tparam SomeEnum An enumeration type
 * @tparam N Some integer or floating point type
 * @param maximum The numerical value of the maximum enum of type SomeEnum
 */
template<typename SomeEnum, typename N>
SomeEnum randomEnumeration(N maximum) {
	
	Randm<N> randm(0, maximum) ;
	N num = randm() ;
	return SomeEnum(num) ;
}


#endif
