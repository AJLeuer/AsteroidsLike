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
constexpr unsigned DEFAULT_W_SZ_X = 960 ;
constexpr unsigned DEFAULT_W_SZ_Y = 540 ;

extern bool HIGH_DPI ;

extern unsigned GLOBAL_MAX_X ; /* To give us buffer space outside the window margins */
extern unsigned GLOBAL_MAX_Y ;

extern unsigned * WINDOW_SIZE_X ;
extern unsigned * WINDOW_SIZE_Y ;

extern int WINDOW_ARGS ;

template<typename N>
N defaultOffset = 4 ;

/**
 * Useful for setting random positions. Call Position(FastRand<N>) with this.
 */
template<typename N>
FastRand<N> * randPosSetter = new FastRand<N>(0, findLargest<N>({static_cast<N>(GLOBAL_MAX_X), static_cast<N>(GLOBAL_MAX_Y)})) ;

template<typename N>
BoundsCheck<N> * defaultCheck = new BoundsCheck<N>(0, static_cast<N>(GLOBAL_MAX_X), 0, static_cast<N>(GLOBAL_MAX_Y)) ;

extern chrono::nanoseconds refreshTime ;

extern char * MOVE_UP ;
extern char * MOVE_DOWN ;
extern char * MOVE_LEFT ;
extern char * MOVE_RIGHT ;



#endif
