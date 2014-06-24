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

using namespace std ;


/**
 * This header holds important data and definitions used throughout the program
 */
#define MAX_X 1920
#define MAX_Y 1080


/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE

/* comment this out (or #undef) when not running in high-dpi mode */
#define HIGH_DPI

#ifdef  HIGH_DPI
#define W_SIZE_X (MAX_X / 2)
#define W_SIZE_Y (MAX_Y / 2)
#define WINDOW_ARGS (SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN)

#else
#define W_SIZE_X MAX_X
#define W_SIZE_Y MAX_Y
#define WINDOW_ARGS (SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN)
#endif

#define eight_milliseconds std::chrono::nanoseconds(8000000) /* Change if we decide to use microsecond precision,
													         etc., instead of nano. */

constexpr unsigned GLOBAL_MAX_X = MAX_X ; /* To give us buffer space outside the window margins */
constexpr unsigned GLOBAL_MAX_Y = MAX_Y ;

extern unsigned WINDOW_SIZE_X ;
extern unsigned WINDOW_SIZE_Y ;

extern chrono::nanoseconds refreshTime ;

extern char * MOVE_UP ;
extern char * MOVE_DOWN ;
extern char * MOVE_LEFT ;
extern char * MOVE_RIGHT ;

#endif
