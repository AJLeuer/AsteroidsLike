//
//  DefaultConfig.h
//  GameWorld
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_DefaultConfig_h
#define GameWorld_DefaultConfig_h

#include "../Util/BoundsCheck.hpp"

#include <chrono>

using namespace std ;


/**
 * This header holds important data and definitions used throughout the program
 */

/* Default value for max X and Y size, may be overridden */
#define MAX_X 960
#define MAX_Y 540

/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE

#define eight_milliseconds std::chrono::nanoseconds(8000000) /* Change if we decide to use microsecond precision,
													         etc., instead of nano. */

extern bool HIGH_DPI ;

extern unsigned  * GLOBAL_MAX_X = 10000 ;
extern unsigned  * GLOBAL_MAX_Y = 4000 ;

extern unsigned * WINDOW_SIZE_X ;
extern unsigned * WINDOW_SIZE_Y ;

extern int WINDOW_ARGS ;

template<typename N>
N defaultOffset = 4 ;

template<typename N>
FastRand<N> * randPosSetter = new FastRand<N>(0, findLargest<N>({*GLOBAL_MAX_X, *GLOBAL_MAX_Y})) ;

template<typename N>
BoundsCheck<N> * defaultCheck = new BoundsCheck<N>(0, *GLOBAL_MAX_X, 0, *GLOBAL_MAX_Y) ;

extern chrono::nanoseconds refreshTime ;

extern char * MOVE_UP ;
extern char * MOVE_DOWN ;
extern char * MOVE_LEFT ;
extern char * MOVE_RIGHT ;

#endif
