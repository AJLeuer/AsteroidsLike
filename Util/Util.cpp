//
//  Util.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Util.hpp"

using namespace std ;

char * basicAlphabet = new char[26] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'} ;


const string generateName(unsigned int length)  {
	string s = "" ;
	fastRand<int> rnd(0, 27) ;
	s += std::toupper(basicAlphabet[rnd.nextValue()]) ;
	for (unsigned i = 0 ; i < length ; i++) {
		s += basicAlphabet[(rnd.nextValue())] ;
	}
	return std::move(s) ;
}

unsigned termWidth() {
	unsigned r = atoi(getenv("COLUMNS")) ;
	return r ;
}

unsigned termHeight() {
	unsigned r = atoi(getenv("LINES")) ;
	return r ;
}
