//
//  Configuration.h
//  GameWorld
//
//  Created by Adam James Leuer on 5/25/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Configuration__
#define __GameWorld__Configuration__

#include <iostream>
#include <fstream>


using namespace std ;


class Configuration {

private:

	static ifstream configFile ;

public:

	template <class Value>
	Value getValue(string key) ;

	template <class Value>
	void writeValue(string key, Value value) ;

} ;



#endif /* defined(__GameWorld__Configuration__) */
