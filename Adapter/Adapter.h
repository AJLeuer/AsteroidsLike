//
//  Adapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Adapter__
#define __GameWorld__Adapter__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>

#include "AdapterUtil.h"
#include "AdapterInterface.h"

#include "Location.h"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/World.h"

using namespace std ;


/**
 * Much like World, Adapter is a singleton, unlike World though its class variables and functions run in an instance
 * For the client however, the only difference in calling World's functions and calling Adapter's is that they will 
 * need to create a single object before calling Adapter's methods. However the naming conventions and usage should be
 * very similar to that found in the World class. It will have access to World's data
 * members, but while World is in charge of manipulating objects in the GameWorld and mediating their interactions,
 * Adapter only uses World's data to update the view
 */
class Adapter : public AdapterInterface {
	
private:
	
	Adapter() {}

public:
	
	/**
	 * This class may be instantiated only once, and should be treated as
	 * a singleton (call init). The int parameter only exists to differentiate this constructor
	 * from the default constructor (which can only be used by the system). The argument does not
	 * matter and is not used in any way. Trying to create more than one instance of Adapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	Adapter(int) ;
	
	void init() ;

	void show() ;
	
	void close() ;
	
};

#endif /* defined(__GameWorld__Adapter__) */
