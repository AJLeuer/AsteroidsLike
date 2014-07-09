//
//  DefaultConfig.h
//  World
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_DefaultConfig_h
#define GameWorld_DefaultConfig_h

#define Color SDL_Color
#define Surface SDL_Surface
#define Texture SDL_Texture
#define Renderer SDL_Renderer
#define Window SDL_Window


#include "../Util/Util.hpp"

#include <chrono>

using namespace std ;

/* Forward declaring... */
template<typename N>
struct Resolution ;


/**
 * This header holds important data and definitions used throughout the program
 */


/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE
#define one_millisecond std::chrono::nanoseconds(1000000)
#define four_milliseconds std::chrono::nanoseconds(4000000)
#define eight_milliseconds std::chrono::nanoseconds(8000000) /* Change if we decide to use microsecond precision,
													         etc., instead of nano. */
#define sixfour_milliseconds std::chrono::nanoseconds(64000000)

extern bool debugToSTDOutput ;

/* Don't change */
constexpr unsigned MAX_X_REFERENCE_VAL = 1280 ;
constexpr unsigned MAX_Y_REFERENCE_VAL = 720 ;

/* Default value for window height and width */
constexpr unsigned DEFAULT_W_MAX_X =  1920 ;
constexpr unsigned DEFAULT_W_MAX_Y = 1080 ;

extern unsigned globalMaxX() ; /* To give us buffer space outside the window margins */
extern unsigned globalMaxY() ;

/**
 * Refer to RESOLUTION_X_BASE_VALUE and RESOLUTION_Y_BASE_VALUE only for telling the OS
 * or renderer how large our window should be. Everything else should refer
 * to windowSizeX() and windowSizeY().
 * RESOLUTION_X_BASE_VALUE and RESOLUTION_Y_BASE_VALUE give only the number of LOGICAL
 * pixels, which can be represented by a varying number of physical pixels, depending
 * on DPI settings.
 */
extern unsigned RESOLUTION_X_BASE_VALUE ;

/**
 * Refer to RESOLUTION_X_BASE_VALUE and RESOLUTION_Y_BASE_VALUE only for telling the OS
 * or renderer how large our window should be. Everything else should refer
 * to windowSizeX() and windowSizeY().
 * RESOLUTION_X_BASE_VALUE and RESOLUTION_Y_BASE_VALUE give only the number of LOGICAL
 * pixels, which can be represented by a varying number of physical pixels, depending
 * on DPI settings.
 */
extern unsigned RESOLUTION_Y_BASE_VALUE ;

/**
 * Any function that needs information about the size of the window should
 * refer to windowSizeX() or windowSizeY(). Unlike RESOLUTION_X_BASE_VALUE and
 * RESOLUTION_Y_BASE_VALUE, these values will vary depending on our DPI settings.
 * windowSizeX() and windowSizeY() always give the ACTUAL number of pixels,
 * regardless of DPI settings.
 */
extern unsigned windowSizeX() ;

/**
 * Any function that needs information about the size of the window should
 * refer to windowSizeX() or windowSizeY(). Unlike RESOLUTION_X_BASE_VALUE and
 * RESOLUTION_Y_BASE_VALUE, these values will vary depending on our DPI settings.
 * windowSizeX() and windowSizeY() always give the ACTUAL number of pixels,
 * regardless of DPI settings.
 */
extern unsigned windowSizeY() ;

/**
 * Returns the current resolution. Delete when done using.
 */
extern Resolution<unsigned> * currentResolution() ;

/**
 * Returns the current resolution, with no adjustments made for DPI. Use with
 * caution. Delete when done using.
 */
extern Resolution<unsigned> * currentResolutionBaseValue() ;

extern int WINDOW_ARGS ;

template<typename N>
N defaultOffset = 8 ;



extern chrono::nanoseconds refreshTime ;

/* todo replace at runtime with
 user-selected values */
extern char   JUMP_KEY ;
extern char * MOVE_UP_KEY ;
extern char * MOVE_DOWN_KEY ;
extern char * MOVE_LEFT_KEY ;
extern char * MOVE_RIGHT_KEY ;



#endif
