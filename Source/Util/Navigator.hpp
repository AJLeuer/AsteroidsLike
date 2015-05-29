//
//  Navigator.h
//  World
//
//  Created by Adam James Leuer on 4/8/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Navigator__
#define __SpriteFight__Navigator__

#include <iostream>

#include "Util.hpp"
#include "Vect.hpp"

template<typename N>
class Navigator {
	
public:
	
	Direction dir ;
	
	const Vect<N> * start ;
	Vect<N> current ;
	
	long x_travelled() {
		Vect<N> temp( current - (*start) ) ;
		long x_trav = setUnsigned(temp.getX()) ;
		return x_trav ;
	}
	
	long y_travelled() {
		Vect<N> temp = current - (*start) ;
		long y_trav = setUnsigned(temp.getY()) ;
		return y_trav ;
	}
	
	Navigator(N) ;
	
	Navigator(Direction d, const Vect<N> * s, Vect<N> c) ;
	
	Navigator(const Navigator & other) ;
	
	Navigator(Navigator && other) ;
	
	~Navigator() ;
	
	Navigator & operator=(const Navigator & other) ;
	
	Navigator & operator=(Navigator && rhs) ;
	
} ;

template<typename N>
Navigator<N>::Navigator(N i) {}

template<typename N>
Navigator<N>::Navigator(Direction d, const Vect<N> * s, Vect<N> c) :
	dir(d), start(s), current(c) {}

template<typename N>
Navigator<N>::Navigator(const Navigator & other) :
	dir(other.dir), start(other.start), current(other.current) {}

template<typename N>
Navigator<N>::Navigator(Navigator && other) :
	dir(other.dir), start(other.start), current(std::move(other.current))
{
	other.start = nullptr ;
}

template<typename N>
Navigator<N>::~Navigator() {

	//delete start ;
}

template<typename N>
Navigator<N> & Navigator<N>::operator=(Navigator<N> && rhs) {
	if (this != &rhs) {
		this->dir = rhs.dir ;
		this->start = rhs.start ;
		this->current = Vect<N>(rhs.current) ;
		rhs.start = nullptr ;
	}
	return *this ;

}

template<typename N>
Navigator<N> & Navigator<N>::operator=(const Navigator<N> & other) {
	if (this != &other) {
		dir = other.dir ;
		start = other.start ;
		current = other.current ;
	}
	return *this ;
}



#endif /* defined(__SpriteFight__Navigator__) */
