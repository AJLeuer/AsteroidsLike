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

#include <SDL2/SDL_rect.h>

using namespace std ;

template<typename T>
class FastRand {
	
private:
	std::random_device dev ;
	std::uniform_int_distribution<T> dist ;
	std::default_random_engine rndm{dev()} ;
	
	T min ;
	T max ;
	
public:
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


/* more useful functions: */

template<typename T>
T randSignFlip(T n) {
	FastRand<unsigned long> rand(0, 1) ;
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
	FastRand<unsigned long> rand(0, 1) ;
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
T findSmallest_helper(unsigned long currSmallest, vector<T> cont) {
	//vector<T*> smallerElem = vector<T*>() ;
	auto size = cont.size() ;
	
	for (auto i = 0 ; i != cont.size() ; i++) {
		
		if (size == 1) {
			return cont.at(currSmallest) ;
		}
		if (cont.at(i) < cont.at(currSmallest)) {
			currSmallest = i ;
			size-- ;
		}
	}
	return cont.at(currSmallest) ;
}

template<typename T>
T findSmallest(vector<T> cont) {
	return findSmallest_helper(0, cont) ;
}

template<typename T>
T findLargest_helper(unsigned long currLargest, vector<T> cont) {
	auto size = cont.size() ;
	
	for (auto i = 0 ; i != cont.size() ; i++) {
		
		if (size == 1) {
			return cont.at(currLargest) ;
		}
		if (cont.at(i) > cont.at(currLargest)) {
			currLargest = i ;
			size-- ;
		}
	}
	return cont.at(currLargest) ;
}

template<typename T>
T findLargest(vector<T> cont) {
	return findLargest_helper(0, cont) ;
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
N ceilling(N n1, N n2) {
	if (n1 >= n2) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}

template<typename N>
N floor(N n1, N n2) {
	if (n1 <= n2) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}


/**
 * Rounds a float or double to an int or long
 */
template<typename F = float, typename I = int> // F = some float or double, I = some unsigned, int, long, etc
I roundF(F value) {
    if ((typeid(F) == typeid(unsigned)) || (typeid(F) == typeid(int)) || (typeid(F) == typeid(long)))  {
        return value ;
    }
    else {
        F temp = (value >= 0.0f) ? (floor(value + 0.5f)) : (ceil(value - 0.5f)) ;
        I round = static_cast<I>(temp) ;

		/* catch a weird problem where this was returning a negative value from a posititive input */
		if ((value >= 0) && (round < 0)) {
			round = (round * -1) ;
		}
		
        return round ;
    }
}

template<typename I = int, class FloatPosition>
FloatPosition roundF(FloatPosition & pos) {
    if ((typeid(pos.x) == typeid(unsigned)) || (typeid(pos.x) == typeid(int)) || (typeid(pos.x) == typeid(long)))  {
        return pos ;
    }
    else {
        I tempX = roundFI(pos.getX()) ;
        I tempY = roundFI(pos.getY()) ;
        I tempZ = roundFI(pos.getZ()) ;
        return FloatPosition(tempX, tempY, tempZ) ;
    }
}



template<typename I = int, class FloatPosition>
FloatPosition roundF(FloatPosition * pos) {
    if ((typeid(pos->x) == typeid(unsigned)) || (typeid(pos->x) == typeid(int)) || (typeid(pos->x) == typeid(long)))  {
        return pos ;
    }
    else {
        I tempX = roundFI(pos->getX()) ;
        I tempY = roundFI(pos->getY()) ;
        I tempZ = roundFI(pos->getZ()) ;
        return FloatPosition(tempX, tempY, tempZ) ;
    }
}

template<typename I = int, class FloatPosition>
FloatPosition * roundF(FloatPosition * pos) {
    if ((typeid(pos->x) == typeid(unsigned)) || (typeid(pos->x) == typeid(int)) || (typeid(pos->x) == typeid(long)))  {
        return pos ;
    }
    else {
        I tempX = roundFI(pos->getX()) ;
        I tempY = roundFI(pos->getY()) ;
        I tempZ = roundFI(pos->getZ()) ;
        return new FloatPosition(tempX, tempY, tempZ) ;
    }
}

template<typename N>
SDL_Rect & operator*(N n, SDL_Rect & rhs) {
	rhs.w = n * rhs.w ;
	rhs.h = n * rhs.h ;
	return rhs ;
}

template<class vec3, class vec2>
SDL_Rect convertToSDL_Rect(vec3 position, vec2 size) {

	auto shape = SDL_Rect() ;
	shape.x = roundF(position.getX()) ;
	shape.y = roundF(position.getY()) ;

	shape.w = roundF(size->getWidth()) ;
	shape.h = roundF(size->getHeight()) ;

	return std::move(shape) ;
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

inline unsigned termWidth() {
	unsigned r = atoi(getenv("COLUMNS")) ;
	return r ;
}

inline unsigned termHeight() {
	unsigned r = atoi(getenv("LINES")) ;
	return r ;
}


/* misc. other */
extern char * basicAlphabet ;

const string generateName(unsigned int length) ;






#endif
