//
//  Debug.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/6/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Debug.h"

using namespace std ;


unsigned Debug::debugCounter = 0 ;

Debug * Debug::debugOutput = new Debug(new ofstream("Debug.txt")) ;

void Debug::init(bool stdoutput) {
	if (stdoutput) {
		Debug::debugOutput = new Debug(&cout) ;
	}
	else {
		Debug::debugOutput = new Debug(new ofstream("Debug.txt")) ;
	}
}