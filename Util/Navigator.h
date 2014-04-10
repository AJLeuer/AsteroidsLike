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

#include "Util.h"
#include "Location.h"

class Navigator {
	
public:
	
	Direction dir ;
	
	const Location<long> * start ;
	Location<long> current ;
	
	long x_travelled() {
		Location<long> temp = current - (*start) ;
		long x_trav = setUnsigned(temp.x) ;
		return x_trav ;
	}
	
	long y_travelled() {
		Location<long> temp = current - (*start) ;
		long y_trav = setUnsigned(temp.y) ;
		return y_trav ;
	}
	
	Navigator(int) ;
	
	Navigator(Direction d, const Location<long> * s, Location<long> c) ;
	
	Navigator(const Navigator & other) ;
	
	Navigator & operator=(const Navigator & other) ;
	
} ;


#endif /* defined(__GameWorld__Navigator__) */
