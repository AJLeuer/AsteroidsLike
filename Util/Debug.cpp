//
//  Debug.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/6/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Debug.h"

using namespace std ;

ostream * Debug::debugOutput { nullptr } ;

unsigned Debug::debugCounter = 0 ;

void Debug::init(bool stdoutput) {
	if (stdoutput) {
		Debug::debugOutput = &cout ;
	}
	else {
		Debug::debugOutput = new ofstream("debug.txt")  ;
	}
}