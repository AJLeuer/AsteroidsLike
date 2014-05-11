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
	const N min_X ;
	const N max_X ;
	const N min_Y ;
	const N max_Y ;
	
	BoundsCheck<N>(N min_X_, N max_X_, N min_Y_, N max_Y_) :
	max_X(max_X_), min_X(min_X_), max_Y(max_Y_), min_Y(min_Y_) {}
	
	void checkAgainst(N * x) {
		if (*x > this->max_X) {
			*x = this->max_X ;
		}
		else if (*x < this->min_X) {
			*x = this->min_X ;
		}
	}
	
	void checkAgainst(N * x, N * y) {
		if (*x > this->max_X) {
			*x = this->max_X ;
		}
		else if (*x < this->min_X) {
			*x = this->min_X ;
		}
		if (*y > this->max_Y) {
			*y = this->max_Y ;
		}
		else if (*y < this->min_Y) {
			*y = this->min_Y ;
		}
	}
} ;

#endif /* defined(__GameWorld__BoundsCheck__) */
