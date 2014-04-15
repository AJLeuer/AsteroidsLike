//
//  AdapterUtil.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__AdapterUtil__
#define __GameWorld__AdapterUtil__

#include <iostream>

#include "../Util/Location.hpp"
#include "../GameWorld/GameData.h"
#include "../GameWorld/GameObject.h"


class AdapterUtil {
	
public:
	static unsigned termWidth() ;
	static unsigned termHeight() ;
	
	/**
	 * Translates a location from within the GameWorld to a location
	 * equivelent within the boundaries of the current screen.
	 * Gets the current GLOBAL_MAX_LOCATION (and MIN) from World
	 * to calculate the ratio
	 *
	 * @param inGameWorld The Location from within the GameWorld
	 */
	template<typename T>
	static Location<T> transLocation(const Location<T> & inGameWorld) {
		auto worldXSize = GLOBAL_MAX_X /*- GameObject::GLOBAL_MIN_X*/ ;
		auto worldYSize = GLOBAL_MAX_Y /*- GameObject::GLOBAL_MIN_Y*/ ;
		
		T tempX = inGameWorld.getX() /*+ (worldXSize - GameObject::GLOBAL_MAX_X)*/ ;
		T tempY = inGameWorld.getY() /*+ (worldYSize - GameObject::GLOBAL_MAX_Y)*/ ;
		
		unsigned tw = termWidth() ;
		unsigned th = termHeight() ;
		
		T x = (tw * tempX)/worldXSize ;
		T y = (th * tempY)/worldYSize ;
		
		return Location<T>(x, y, 0) ;
	}
	
} ;

#endif /* defined(__GameWorld__AdapterUtil__) */
