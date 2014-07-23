//
//  GameRandom.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/22/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameRandom.hpp"

FastRand<float> * FastRand<float>::randPositionSetter = initRandPosSetter() ;


FastRand<float> * FastRand<float>::initRandPosSetter() {
	return new FastRand<float>(0, findLargest<float>({static_cast<float>(globalMaxX()), static_cast<float>(globalMaxY())})) ;
}


FastRand<float> FastRand<float>::defaultRandom(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()) ;


FastRand<float>::FastRand(float _min, float _max) :
	minimum(_min),
	maximum(_max)
{
	//dev ;
	dist = uniform_real_distribution<float>(minimum, maximum) ;
	rndm.seed(dev()) ;
}


FastRand<float>::FastRand(const FastRand<float> & other)
{
	std::random_device dev2 ;
	dist = uniform_real_distribution<float>(other.minimum, other.maximum) ;
	rndm.seed(dev2()) ;
}


FastRand<float>::~FastRand(){}


FastRand<float> & FastRand<float>::operator=(const FastRand<float> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2 ;
		dist = uniform_real_distribution<float>(rhs.minimum, rhs.maximum) ;
		rndm.seed(dev2()) ;
	}
	return *this ;
}


float FastRand<float>::nextValue() {
	return dist(rndm) ;
}


float FastRand<float>::nextValue(float minimum, float maximum) {
	std::uniform_real_distribution<float> dif_dist{minimum, maximum} ;
	return dif_dist(rndm) ;
}


float FastRand<float>::operator()() {
	return nextValue() ;
}


float FastRand<float>::operator()(float minimum, float maximum) {
	return nextValue(minimum, maximum) ;
}


template<typename R>
R FastRand<float>::nextValue(R _min, R _max) {
	std::uniform_real_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rndm) ;
}

