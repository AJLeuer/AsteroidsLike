//
//  Navigator.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/8/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Navigator__
#define __GameWorld__Navigator__

#include <iostream>

#include "Util.hpp"
#include "Position.hpp"

class Navigator {
	
public:
	
	Direction dir ;
	
	const Position<long> * start ;
	Position<long> current ;
	
	long x_travelled() {
		Position<long> temp{ current - (*start) } ;
		long x_trav = setUnsigned(temp.getX()) ;
		return x_trav ;
	}
	
	long y_travelled() {
		Position<long> temp = current - (*start) ;
		long y_trav = setUnsigned(temp.getY()) ;
		return y_trav ;
	}
	
	Navigator(int) ;
	
	Navigator(Direction d, const Position<long> * s, Position<long> c) ;
	
	Navigator(const Navigator & other) ;
	
	Navigator(Navigator && other) ;
	
	~Navigator() ;
	
	Navigator & operator=(const Navigator & other) ;
	
	Navigator & operator=(Navigator && rhs) ;
	
} ;


#endif /* defined(__GameWorld__Navigator__) */
