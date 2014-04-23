//
//  BoundsCheck.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/23/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__BoundsCheck__
#define __GameWorld__BoundsCheck__

#include <iostream>

/**
 * Helps with checking validity of Position objects
 * Used in Position's checkBounds()
 */
template<typename N>
struct BoundsCheck {
	const N MAX_X ;
	const N MIN_X ;
	const N MAX_Y ;
	const N MIN_Y ;
	
	BoundsCheck<N>(N MAX_X_, N MIN_X_, N MAX_Y_, N MIN_Y_) :
	MAX_X(MAX_X_), MIN_X(MIN_X_), MAX_Y(MAX_Y_), MIN_Y(MIN_Y_) {}
} ;

#endif /* defined(__GameWorld__BoundsCheck__) */
