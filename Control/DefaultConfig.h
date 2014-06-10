//
//  DefaultConfig.h
//  GameWorld
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_DefaultConfig_h
#define GameWorld_DefaultConfig_h

#include <chrono>


/**
 * This header holds important data and definitions used throughout the program
 */
#define MAX_X 1600
#define MIN_X 0
#define MAX_Y 900
#define MIN_Y 0

/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE

/* comment this out (or #undef) when not running in high-dpi mode */
#define HIGH_DPI

#ifdef  HIGH_DPI
#define WINDOW_MAX_X (MAX_X / 2)
#define WINDOW_MIN_X (MIN_X / 2)
#define WINDOW_MAX_Y (MAX_Y / 2)
#define WINDOW_MIN_Y (MIN_Y / 2)
#define WINDOW_ARGS (SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN)

#else
#define WINDOW_MAX_X MAX_X
#define WINDOW_MIN_X MIN_X
#define WINDOW_MAX_Y MAX_Y
#define WINDOW_MIN_Y MIN_Y
#define WINDOW_ARGS (SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN)
#endif

#define WINDOW_SIZE_X WINDOW_MAX_X
#define WINDOW_SIZE_Y WINDOW_MAX_Y

#define eight_milliseconds std::chrono::nanoseconds(8000000) /* Change if we decide to use microsecond precision,
													         etc., instead of nano. */


constexpr float worldToWindowRatio = 1.5 ;

constexpr int GLOBAL_MAX_X = (WINDOW_MAX_X * worldToWindowRatio) ; /* To give us buffer space outside the window margins */
constexpr int GLOBAL_MIN_X = (WINDOW_MIN_X) ;
constexpr int GLOBAL_MAX_Y = (WINDOW_MAX_Y * worldToWindowRatio) ;
constexpr int GLOBAL_MIN_Y = (WINDOW_MIN_Y) ;

constexpr auto refreshTime = eight_milliseconds ;

extern char * MOVE_UP ;
extern char * MOVE_DOWN ;
extern char * MOVE_LEFT ;
extern char * MOVE_RIGHT ;

#endif
