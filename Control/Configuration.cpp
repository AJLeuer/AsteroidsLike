//
//  Configuration.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/25/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Configuration.h"

using namespace std ;

ifstream Configuration::configFile("/Settings/Setting.cfg") ;

void Configuration::init() {
    doOverrides() ;
}

void Configuration::doOverrides() {
    //todo
    /* make any user requested changes from values in default config */
}