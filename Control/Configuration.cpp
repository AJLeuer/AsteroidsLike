//
//  Configuration.cpp
//  World
//
//  Created by Adam James Leuer on 5/25/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Configuration.h"

using namespace std ;

/* Many of these values will be overridden */

//define extern values from DefaultConfig.h as well
#ifdef __APPLE__

bool hiDPI() { return hiDPI_OSX ; } ;
float displayScalingFactor() { return displayScalingFactor_OSX ; }

#elif defined __linux__

bool hiDPI() { return hiDPI_Linux ; } ;
float displayScalingFactor() { return displayScalingFactor_Linux ; }

#elif defined _WIN64

bool hiDPI() { return hiDPI_Windows ; } ;
float displayScalingFactor() { return displayScalingFactor_Windows ; }

#endif

/* May be larger than window size to give us buffer space outside the window margins */
unsigned globalMaxX() { return GLOBAL_MAX_X_BASE_VALUE * displayScalingFactor() ; }
unsigned globalMaxY()   { return GLOBAL_MAX_Y_BASE_VALUE * displayScalingFactor() ; }

unsigned LOGICAL_WINDOW_SIZE_X = DEFAULT_W_MAX_X ;
unsigned LOGICAL_WINDOW_SIZE_Y = DEFAULT_W_MAX_Y ;

/* Default values, will most likely be changed by Configuration::init() */
unsigned windowSizeX() {
    return (LOGICAL_WINDOW_SIZE_X * displayScalingFactor()) ;
}

unsigned windowSizeY() {
    return (LOGICAL_WINDOW_SIZE_Y * displayScalingFactor()) ;
}

int WINDOW_ARGS = (SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN) ; /* will always need proper initialization to check for DPI changes */

chrono::nanoseconds refreshTime = eight_milliseconds ;

char * MOVE_UP    = (char *)"W" ;
char * MOVE_DOWN  = (char *)"S" ;
char * MOVE_LEFT  = (char *)"A" ;
char * MOVE_RIGHT = (char *)"D" ;

bool Configuration::isInit = false ;

ifstream Configuration::configFile("/Settings/Setting.cfg") ;


void Configuration::init() {
	
	/* make any user requested changes from values in default config */
    doUserOverrides() ;
	
	adjustForHiDPI() ;
    
    isInit = true ;
}

void Configuration::doUserOverrides() {
	//todo
}


void Configuration::adjustForHiDPI() {
	if (hiDPI()) {
		WINDOW_ARGS = (SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN) ;
	}
	else {
		/* No change to window size */
		WINDOW_ARGS = (SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN) ;
	}
}




