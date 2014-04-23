//
//  Util.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/20/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_Util_h
#define GameWorld_Util_h

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>

using namespace std ;

template<typename T>
class fastRand {
	
private:
	std::random_device dev{};
	std::uniform_int_distribution<T> dist ;
	std::default_random_engine rnd{dev()};
	
	T min ;
	T max ;
	
public:
	fastRand(T _min, T _max) ;
	fastRand(const fastRand<T> & other) ;
	fastRand<T> & operator=(const fastRand<T> & rhs) ;
	~fastRand() ;
	T nextValue() ;
	T nextValue(T min, T max) ;
	template<typename R> R nextValue(R _min, R _max) ;
	T operator()() ;
	T operator()(T min, T max) ;
	
} ;


template<typename T>
fastRand<T>::fastRand(T _min, T _max) :
	min(_min),
	max(_max)
{
	//dev ;
	dist = uniform_int_distribution<T>(min, max) ;
	rnd.seed(dev()) ;
}

template<typename T>
fastRand<T>::fastRand(const fastRand<T> & other)
{
	std::random_device dev2{} ;
	dist = uniform_int_distribution<T>(other.min, other.max) ;
	rnd.seed(dev2()) ;
}

template<typename T>
fastRand<T>::~fastRand(){}

template<typename T>
fastRand<T> & fastRand<T>::operator=(const fastRand<T> & rhs)
{
	if (this != &rhs) {
		std::random_device dev2{} ;
		dist(rhs.min, rhs.max) ;
		rnd.seed(dev2()) ;
	}
	return *this ;
}

template<typename T>
T fastRand<T>::nextValue() {
	return dist(rnd) ;
}

template<typename T>
T fastRand<T>::nextValue(T min, T max) {
	std::uniform_int_distribution<T> dif_dist{min, max} ;
	return dif_dist(rnd) ;
}

template<typename T>
T fastRand<T>::operator()() {
	return nextValue() ;
}

template<typename T>
T fastRand<T>::operator()(T min, T max) {
	return nextValue(min, max) ;
}

template<typename T>
template<typename R>
R fastRand<T>::nextValue(R _min, R _max) {
	std::uniform_int_distribution<R> dif_dist{_min, _max} ;
	return dif_dist(rnd) ;
}


/* more useful functions: */

template<typename T>
T randSignFlip(T n) ;

template<typename T>
T setUnsigned(T n) ;

template<typename T>
T chooseAtRand(T n1, T n2) ;

template<typename T>
T pythag(T a, T b) ;

template<typename T>
T * findSmallest(vector<T*> cont) ;

template<typename T>
T * findSmallest_helper(unsigned long currSmallest, vector<T*> cont) ;

template<typename T>
T randSignFlip(T n) {
	fastRand<unsigned long> rand(0, 1) ;
	bool pos = rand.nextValue() % 2 ;
	if (pos) {
		return n ;
	}
	else {
		return (n * -1) ;
	}
}

template<typename T>
T setUnsigned(T n) {
	if (n < 0) {
		n = (n * -1) ;
	}
	return n ;
}

template <typename T>
T chooseAtRand(T n1, T n2) {
	fastRand<unsigned long> rand(0, 1) ;
	bool first = rand.nextValue() ;
	if (first) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}



template<typename T>
T pythag(T a, T b) {
	return sqrt((pow(a,2)) + (pow(b,2))) ;
}

template<typename T>
T * findSmallest(vector<T*> cont) {
	return findSmallest_helper(0, cont) ;
}

template<typename T>
T * findSmallest_helper(unsigned long currSmallest, vector<T*> cont) {
	vector<T*> smallerElem = vector<T*>() ;
	unsigned long size = cont.size() ;
	
	for (unsigned i = 0 ; i != cont.size() ; i++) {
		
		if (size == 1) {
			return cont.at(currSmallest) ;
		}
		if ((cont.at(i) != nullptr) && (cont.at(currSmallest) != nullptr)) {
			if (cont.at(i) < cont.at(currSmallest)) {
				currSmallest = i ;
				size-- ;
			}
		}
	}
	return cont.at(currSmallest) ;
}


class Drawing {
public:
	/*
	 * Draws a grid representing every object in the container. Draws an icon at every
	 * index an object is found. It calls the object's getIcon(), returning a std::string,
	 * and prints that at each index. It is recommended that icon consist of only a single char (though
	 * stored inside a string) in order to maintain proportions.
	 *
	 * Note: If the class stored in the container does not implement getIcon() returning a std::string,
	 * the code will not compile.
	 *
	 * @param whiteSpace The character to be printed in spaces where there are no objects
	 * @param writeTo The outputstream (std::ostream) that will receive output
	 */
	template <typename Container>
	void draw2DRepresentation(ostream & writeTo, const Container * container, char whitespace) const ;
} ;



template<class Container>
void Drawing::draw2DRepresentation (ostream & writeTo, const Container * container, char whitespace) const {
	
	for (auto i = 0 ; i < container->at(0)->size() ; i++) {
		for (auto j = 0 ; j < container->size() ; j++) {
			if (container->at(j)->at(i) != nullptr) {
				auto c = container->at(j)->at(i)->getIcon() ;
				writeTo << c ; /*->getIcon()*/  //remove *(     )
			}
			else {
				writeTo << whitespace ;
			}
		}
		writeTo << endl ;
	}
}

template<typename N>
N returnLarger(N n1, N n2) {
	if (n1 >= n2) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}

template<typename N>
N returnSmaller(N n1, N n2) {
	if (n1 <= n2) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}

/* misc. other */
extern char * basicAlphabet ;

const string generateName(unsigned int length) ;






#endif
