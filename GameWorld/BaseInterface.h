//
//  BaseInterface.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__BaseInterface__
#define __GameWorld__BaseInterface__

#include <iostream>

using namespace std ;

/**
 * The virtual base class that will serve as a parent class
 * to every object in the game world. It defines all methods by
 * which any class can talk to another.
 */
class BaseInterface {

 
	
public:
	
	/**
	 * Any class with a reference to this can call notify on it
	 * to pass a message.
	 */
	virtual void notify() = 0 ;
	
	
};

#endif /* defined(__GameWorld__BaseInterface__) */
