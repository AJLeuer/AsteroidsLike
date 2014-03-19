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

#include "../GameWorld/Location.h"
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
	static Location transLocation(const Location & inGameWorld) ;
	
} ;

#endif /* defined(__GameWorld__AdapterUtil__) */
