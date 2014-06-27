//
//  Util2.h
//  World
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

#include "../World/GameState.h"

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
Position<N> translateToWindowCoords(const Position<N> & worldCoords) {
	
	doNothing() ;
	
	Position<N> origin = getWindowOriginAsWorldCoord<N>() ;
	
	auto windowCoords = translateCoords(worldCoords, {0, 0, 0}, origin) ;
	
	return windowCoords ;
}

/**
 * Translates the Position coords from a source coordinate system to coordinates
 * in the destination coordinate system. To work properly, this function an additonal
 * pair of coordinates: first coordinates from the source coordinate system, followed by
 * equivalent coordinates from the destination coordinate system. "Equivalent" implies that if these two
 * coordinate systems were overlaid on top of each other, then these two coordinates would be
 * in the same spot. The simplest way to do this is to simply give the origin ({0, 0, 0}) from the source
 * coordinate system, and its equivalent from the destination coordinate system.
 *
 * @param coords The coordinate to translate
 * @param sourceCoords Sample coordinates from the source coordinate system
 * @param equivalentDestinationCoords Coordinates from the destination coordinate system that are equivalent to sourceCoords
 */
template<typename N>
Position<N> translateCoords(const Position<N> & coords, const Position<N> & sourceCoords, const Position<N> & equivalentDestinationCoords) {
	
	/* using a position to store, not an actual position, but the difference between a position from the source coordinate system and
	 a position from the destination coordinate system (i.e. a vector) */
	auto differential = sourceCoords - equivalentDestinationCoords ;
	
	auto coordsAsDestCoordSystemCoords = (coords + differential) ;
	
	return coordsAsDestCoordSystemCoords ;
}


#endif /* defined(__GameWorld__Util2__) */
