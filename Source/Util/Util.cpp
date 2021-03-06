//
//  Util.cpp
//  World
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Util.hpp"
#include "Random.hpp"


using namespace std ;

/* Apple-specific DPI-checking is implemented in Util.mm,
   Linux and Windows (both todo) are implemented here  */

#ifdef __linux__

//todo implement display scaling checking on linux

#elif defined _WIN64

//todo implement display scaling checking on windows

#endif

namespace Util {

char * basicAlphabet = new char[26] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'} ;

const string generateName(unsigned int length)  {
	string s = "" ;
	Randm<int> rnd(0, 27) ;
	s += std::toupper(basicAlphabet[rnd.nextValue()]) ;
	for (unsigned i = 0 ; i < length ; i++) {
		s += basicAlphabet[(rnd.nextValue())] ;
	}
	return s;
}

}

