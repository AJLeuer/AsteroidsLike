//
//  AdapterInterface.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__AdapterInterface__
#define __GameWorld__AdapterInterface__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>

#include "AdapterUtil.h"

#include "Location.h"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/World.h"

using namespace std ;

/**
 * An abstract class that mainly serves as the base class for Adapter, but will also allow us to create
 * other Adapter-like classes to handle output
 *
 */
class AdapterInterface {
	
protected:
	
	/**
	 * The total number of objects of AdapterInterface or of
	 * derived classes created by the client (but not including those
	 * created by the system). Creating more than one at a time will throw an
	 * exception
	 */
	static unsigned extentMembers ;
	
	/**
	 * The current AdapterInterface object
	 */
	static AdapterInterface * current ;
	
	/**
	 * A pointer to all GameObjects in World
	 */
	static vector<GameObject*> ** WorldObjects ;
	
	/**
	 * This class won't be instantiated by any client directly, only by calling one of its
	 * derived classes
	 */
	AdapterInterface() {}
	
	/**
	 * Copy constructor
	 */
	AdapterInterface(AdapterInterface &) {}
	
	/**
	 * Called by a derived class to instantiate
	 */
	AdapterInterface(AdapterInterface * current, int) ;
	
public:
	
	virtual ~AdapterInterface() ;
	
	/**
	 * Assignment operator overload
	 */
	AdapterInterface & operator=(const AdapterInterface &) {return *this;}
	
	virtual void init() ;
	virtual void show() ;
	virtual void close() ;
	
};


#endif /* defined(__GameWorld__AdapterInterface__) */
