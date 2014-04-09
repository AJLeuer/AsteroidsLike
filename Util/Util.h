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
#include <cmath>

using namespace std ;

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
	bool pos = rand() % 2 ;
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
	bool first = rand() % 2 ;
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

#endif
