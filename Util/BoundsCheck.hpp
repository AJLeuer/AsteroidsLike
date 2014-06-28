//
//  BoundsCheck.h
//  World
//
//  Created by Adam James Leuer on 4/23/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__BoundsCheck__
#define __GameWorld__BoundsCheck__

#include <iostream>

#include "../Control/Configuration.h"


/**
 * Helps with checking validity of Position objects
 * Used in Position's checkBounds()
 */
template<typename N>
struct BoundsCheck {
    
protected:
    static BoundsCheck * defaultCheck_value ;
    
public:
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
    
    /**
     * A useful default BoundsCheck, included for convenience
     */
    static BoundsCheck<N> * defaultCheck() {
        
        /* During initialization of static vars and globals, we have to call this
         * function instead of returning a reference to defaultCheck_value
         */
        if (Configuration::isInit == false) {
            return new BoundsCheck<N>(0, static_cast<N>(globalMaxX()), 0, static_cast<N>(globalMaxY())) ;
        }
        /* One time this is called config will have finished initialization and defaultCheck_value will still be null. We'll
         use that opportunity to init defaultCheck_value */
        else if ((Configuration::isInit) && (defaultCheck_value == nullptr)) {
            defaultCheck_value = new BoundsCheck<N>(0, static_cast<N>(globalMaxX()), 0, static_cast<N>(globalMaxY())) ;
            return defaultCheck_value ;
        }
        /* Here we know we already have an initialized defaultCheck_value that we can just pass to the caller */
        else {
            return defaultCheck_value ;
        }
    }

} ;

template<typename N>
BoundsCheck<N> * BoundsCheck<N>::defaultCheck_value = nullptr ;



#endif /* defined(__GameWorld__BoundsCheck__) */
