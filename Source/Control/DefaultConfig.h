//
//  DefaultConfig.h
//  World
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef SpriteFight_DefaultConfig_h
#define SpriteFight_DefaultConfig_h

#include <SDL2/SDL_render.h>

typedef SDL_Color Color ;
typedef SDL_Surface Surface ;
typedef SDL_Texture Texture ;
typedef SDL_Renderer Renderer ;
typedef SDL_Window Window ;
typedef SDL_RendererInfo RenderInfo ;


#include "../Util/Util.hpp"

#include <chrono>

using namespace std ;

/* Forward declaring... */
template<typename N>
struct Resolution ;


/**
 * This header holds important data and definitions used throughout the program
 */

/*
 * The __cplusplus macro's value changes based on which standard we're using.
 * We will use this for preprocessor checks to make sure we can use C++14 features, and
 * if not default to an equivalent solution compatible with C++11
 */
#define CPP_VERSION __cplusplus

#define CPP14 201305

extern bool debugToSTDOutput ;

/* Don't change */
#if CPP_VERSION == CPP14
constexpr unsigned MAX_X_REFERENCE_VAL = 1280 ;
constexpr unsigned MAX_Y_REFERENCE_VAL = 720 ;

/* Default value for window height and width */
constexpr unsigned DEFAULT_W_MAX_X = 1600 ;
constexpr unsigned DEFAULT_W_MAX_Y = 900 ;

#else
const unsigned MAX_X_REFERENCE_VAL = 1280 ;
const unsigned MAX_Y_REFERENCE_VAL = 720 ;

/* Default value for window height and width */
const unsigned DEFAULT_W_MAX_X = 960 ;
const unsigned DEFAULT_W_MAX_Y = 540 ;
#endif

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
N defaultMoveDistance = 6;

constexpr chrono::duration refreshTime = chrono::milliseconds(16);
constexpr chrono::duration defaultSleepTime = chrono::milliseconds(240);

/* todo replace at runtime with
 user-selected values */
extern char   JUMP_KEY ;
extern char * MOVE_UP_KEY ;
extern char * MOVE_DOWN_KEY ;
extern char * MOVE_LEFT_KEY ;
extern char * MOVE_RIGHT_KEY ;
extern char * TIME_REVERSE_KEY ;
extern char * RotateClockwiseKey;
extern char * RotateCounterClockwiseKey;



#endif
