//
//  Util.cpp
//  World
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Util.hpp"
#include "GameRandom.hpp"

using namespace std ;

/* The Apple version is defined in Util.mm */
#ifdef __linux__

//todo implement display scaling checking on linux
bool hiDPI_Linux = getDisplayScalingFactor_Linux() ;
float displayScalingFactor_Linux = 1.0 ;
float getDisplayScalingFactor_Linux() { return false ; }

#elif defined _WIN64

//todo implement display scaling checking on windows
bool hiDPI_Windows = getDisplayScalingFactor_Windows() ;
float displayScalingFactor_Windows = 1.0 ;
float getDisplayScalingFactor_Windows() { return false ; }

#endif


char * basicAlphabet = new char[26] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'} ;

const string generateName(unsigned int length)  {
	string s = "" ;
	FastRand<int> rnd(0, 27) ;
	s += std::toupper(basicAlphabet[rnd.nextValue()]) ;
	for (unsigned i = 0 ; i < length ; i++) {
		s += basicAlphabet[(rnd.nextValue())] ;
	}
	return std::move(s) ;
}


