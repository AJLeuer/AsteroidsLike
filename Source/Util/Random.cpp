//
//  Random.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/22/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Random.hpp"

/**
 * A convenience global for creating random positions
 */
Randm<float> * Randm<float>::randPositionSetter = initRandPosSetter() ;


Randm<float> * Randm<float>::initRandPosSetter() {
	return new Randm<float>(0, findLargest<float>({static_cast<float>(globalMaxX()), static_cast<float>(globalMaxY())})) ;
}


Randm<float> Randm<float>::defaultRandom(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()) ;


Randm<float>::Randm(float _min, float _max) :
	minimum(_min),
	maximum(_max)
{
	//dev ;
	dist = uniform_real_distribution<float>(minimum, maximum) ;
	rndm.seed(dev()) ;
}


Randm<float>::Randm(const Randm<float> & other)
{
	std::random_device dev2 ;
	dist = uniform_real_distribution<float>(other.minimum, other.maximum) ;
	rndm.seed(dev2()) ;
}


Randm<float>::~Randm(){}


Randm<float> & Randm<float>::operator=(const Randm<float> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_real_distribution<float>(rhs.minimum, rhs.maximum) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}


float Randm<float>::nextValue() {
	return dist(rndm) ;
}


float Randm<float>::nextValue(float minimum, float maximum) {
	std::uniform_real_distribution<float> dif_dist{minimum, maximum} ;
	return dif_dist(rndm) ;
}


float Randm<float>::operator()() {
	return nextValue() ;
}


float Randm<float>::operator()(float minimum, float maximum) {
	return nextValue(minimum, maximum) ;
}


template<typename R>
R Randm<float>::nextValue(R _min, R _max) {
	std::uniform_real_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rndm) ;
}

/**
 * A convenience global for creating random positions
 */
Randm<double> * Randm<double>::randPositionSetter = initRandPosSetter() ;


Randm<double> * Randm<double>::initRandPosSetter() {
	return new Randm<double>(0, findLargest<double>({static_cast<double>(globalMaxX()), static_cast<double>(globalMaxY())})) ;
}


Randm<double> Randm<double>::defaultRandom(std::numeric_limits<double>::min(), std::numeric_limits<double>::max()) ;


Randm<double>::Randm(double _min, double _max) :
minimum(_min),
maximum(_max)
{
	//dev ;
	dist = uniform_real_distribution<double>(minimum, maximum) ;
	rndm.seed(dev()) ;
}


Randm<double>::Randm(const Randm<double> & other)
{
	std::random_device dev2 ;
	dist = uniform_real_distribution<double>(other.minimum, other.maximum) ;
	rndm.seed(dev2()) ;
}


Randm<double>::~Randm(){}


Randm<double> & Randm<double>::operator=(const Randm<double> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_real_distribution<double>(rhs.minimum, rhs.maximum) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}


double Randm<double>::nextValue() {
	return dist(rndm) ;
}


double Randm<double>::nextValue(double minimum, double maximum) {
	std::uniform_real_distribution<double> dif_dist{minimum, maximum} ;
	return dif_dist(rndm) ;
}


double Randm<double>::operator()() {
	return nextValue() ;
}


double Randm<double>::operator()(double minimum, double maximum) {
	return nextValue(minimum, maximum) ;
}


template<typename R>
R Randm<double>::nextValue(R _min, R _max) {
	std::uniform_real_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rndm) ;
}
