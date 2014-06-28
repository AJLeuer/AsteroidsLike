//
//  DefaultConfig.h
//  World
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_DefaultConfig_h
#define GameWorld_DefaultConfig_h

#include "../Util/Util.hpp"
#include "../Util/BoundsCheck.hpp"

#include <chrono>

using namespace std ;


/**
 * This header holds important data and definitions used throughout the program
 */


/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE

#define eight_milliseconds std::chrono::nanoseconds(8000000) /* Change if we decide to use microsecond precision,
													         etc., instead of nano. */

/* Default value for window height and width, will almost always be overridden */
constexpr unsigned GLOBAL_MAX_X_BASE_VALUE = 4000 ;
constexpr unsigned GLOBAL_MAX_Y_BASE_VALUE = 2500 ;

constexpr unsigned DEFAULT_W_MAX_X = 960 ;
constexpr unsigned DEFAULT_W_MAX_Y = 540 ;

extern bool hiDPI() ;
extern float displayScalingFactor() ;

extern unsigned globalMaxX() ; /* To give us buffer space outside the window margins */
extern unsigned globalMaxY() ;

/**
 * Refer to LOGICAL_WINDOW_SIZE_X and LOGICAL_WINDOW_SIZE_Y only for telling the OS
 * or renderer how large our window should be. Everything else should refer
 * to windowSizeX() and windowSizeY().
 * LOGICAL_WINDOW_SIZE_X and LOGICAL_WINDOW_SIZE_Y give only the number of LOGICAL
 * pixels, which can be represented by a varying number of physical pixels, depending
 * on DPI settings.
 */
extern unsigned LOGICAL_WINDOW_SIZE_X ;

/**
 * Refer to LOGICAL_WINDOW_SIZE_X and LOGICAL_WINDOW_SIZE_Y only for telling the OS
 * or renderer how large our window should be. Everything else should refer
 * to windowSizeX() and windowSizeY().
 * LOGICAL_WINDOW_SIZE_X and LOGICAL_WINDOW_SIZE_Y give only the number of LOGICAL
 * pixels, which can be represented by a varying number of physical pixels, depending
 * on DPI settings.
 */
extern unsigned LOGICAL_WINDOW_SIZE_Y ;

/**
 * Any function that needs information about the size of the window should
 * refer to windowSizeX() or windowSizeY(). Unlike LOGICAL_WINDOW_SIZE_X and
 * LOGICAL_WINDOW_SIZE_Y, these values will vary depending on our DPI settings.
 * windowSizeX() and windowSizeY() always give the ACTUAL number of pixels,
 * regardless of DPI settings.
 */
extern unsigned windowSizeX() ;

/**
 * Any function that needs information about the size of the window should
 * refer to windowSizeX() or windowSizeY(). Unlike LOGICAL_WINDOW_SIZE_X and
 * LOGICAL_WINDOW_SIZE_Y, these values will vary depending on our DPI settings.
 * windowSizeX() and windowSizeY() always give the ACTUAL number of pixels,
 * regardless of DPI settings.
 */
extern unsigned windowSizeY() ;

extern int WINDOW_ARGS ;

template<typename N>
N defaultOffset = 4 ;

/**
 * Useful for setting random positions. Call Position(FastRand<N>) with this.
 */
template<typename N>
FastRand<N> randPositionSetter() {
	return FastRand<N>(0, findLargest<N>({static_cast<N>(globalMaxX()), static_cast<N>(globalMaxY())})) ;
}

template<typename N>
BoundsCheck<N> defaultCheck() {
	return BoundsCheck<N>(0, static_cast<N>(globalMaxX()), 0, static_cast<N>(globalMaxY())) ;
}

extern chrono::nanoseconds refreshTime ;

extern char * MOVE_UP ;
extern char * MOVE_DOWN ;
extern char * MOVE_LEFT ;
extern char * MOVE_RIGHT ;



#endif
