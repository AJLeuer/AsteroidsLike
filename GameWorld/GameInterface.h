//
//  BaseInterface.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameInterface__
#define __GameWorld__GameInterface__

#include <iostream>

#include "Message.h"


/**
 * The virtual base class that will serve as an interface
 * to every object in the game world. It defines all methods by
 * which any class can talk to another.
 */
class GameInterface {

 
	
public:
		
	/**
	 * Any class with a reference to this object can call notify on it
	 * to pass a message.
	 */
	virtual void notify() = 0 ;
	
	
	/**
	 * An implementing class can define a default function by overloading its () operator
	 */
	virtual void operator()() = 0 ;
	
	
};

#endif /* defined(__GameWorld__GameInterface__) */
