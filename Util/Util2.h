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


#endif /* defined(__GameWorld__Util2__) */
