//
//  Util2.h
//  GameWorld
//
//  Created by Adam James Leuer on 5/23/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Util2__
#define __GameWorld__Util2__

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>

#include "Util.hpp"
#include "Position.hpp"
#include "Size.hpp"

#include "../Control/DefaultConfig.h"

/* Creating a second header for utility functions since we ran into some trouble 
   with includes */
template<typename M, typename N>
Size<N> & operator*(M m, Size<N> & rhs) {
	rhs.setW((m * rhs.getW())) ;
	rhs.setH((m * rhs.getH())) ;
	return rhs ;
}

template<typename M, typename N>
Position<N> * convert(const Position<M> * converted) {
    return new Position<N>(roundF<M, N>(converted->getX()), roundF<M, N>(converted->getY()), roundF<M, N>(converted->getZ())) ;
}

/**
 * This function translates between coordinate systems.
 */
template<typename N>
Position<N> translateToWindowCoords(const Position<N> & coords) {
    float x_ratio = (coords.getX() / GLOBAL_MAX_X) ;
    float y_ratio = (coords.getY() / GLOBAL_MAX_Y) ;
    
    N x_pos = WINDOW_SIZE_X * x_ratio ;
    N y_pos = WINDOW_SIZE_Y * y_ratio ;
    
    return Position<N>(x_pos, y_pos, 0) ;
}


#endif /* defined(__GameWorld__Util2__) */
