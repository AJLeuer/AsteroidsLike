//
//  Configuration.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/25/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Configuration.h"

using namespace std ;

/* Many of these values will be overridden */

//define extern values from DefaultConfig.h as well
#ifdef __APPLE__
bool HIGH_DPI = checkHiDPI() ;
#else
bool HIGH_DPI = false ; //todo: add checking on windows and linux
#endif

unsigned GLOBAL_MAX_X = 10000 ; /* To give us buffer space outside the window margins */
unsigned GLOBAL_MAX_Y = 4000 ;

unsigned * WINDOW_SIZE_X = new unsigned(DEFAULT_W_SZ_X) ; /* Default values, will most likely be changed by Configuration::init() */
unsigned * WINDOW_SIZE_Y = new unsigned(DEFAULT_W_SZ_Y) ;

int WINDOW_ARGS = 0 ; /* will always need to be initialized */

chrono::nanoseconds refreshTime = eight_milliseconds ;

char * MOVE_UP    = (char *)"W" ;
char * MOVE_DOWN  = (char *)"S" ;
char * MOVE_LEFT  = (char *)"A" ;
char * MOVE_RIGHT = (char *)"D" ;

ifstream Configuration::configFile("/Settings/Setting.cfg") ;


void Configuration::init() {
	
	BoundsCheck<int> * bc = defaultCheck<int> ;
	
	/* make any user requested changes from values in default config */
    doUserOverrides() ;
	
	adjustForHiDPI() ;
}

void Configuration::doUserOverrides() {
	//todo
}


void Configuration::adjustForHiDPI() {
	if (HIGH_DPI) {
		*WINDOW_SIZE_X = (*WINDOW_SIZE_X / 2) ;
		*WINDOW_SIZE_Y = (*WINDOW_SIZE_Y / 2) ;
		WINDOW_ARGS  = (SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN) ;
	}
	else {
		/* No change to window size */
		WINDOW_ARGS = (SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN) ;
	}
}


