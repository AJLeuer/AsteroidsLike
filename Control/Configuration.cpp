//
//  Configuration.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/25/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Configuration.h"

using namespace std ;

//define extern values from DefaultConfig.h as well
unsigned WINDOW_SIZE_X = 1600 ;
unsigned WINDOW_SIZE_Y = 900 ;

chrono::nanoseconds refreshTime = eight_milliseconds ;

char * MOVE_UP = (char *)"W" ;
char * MOVE_DOWN = (char *)"S" ;
char * MOVE_LEFT = (char *)"A" ;
char * MOVE_RIGHT = (char *)"D" ;

ifstream Configuration::configFile("/Settings/Setting.cfg") ;

void Configuration::init() {
    doOverrides() ;
}

void Configuration::doOverrides() {
    //todo
    /* make any user requested changes from values in default config */
}