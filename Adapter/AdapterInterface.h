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
	 * created by the system). Creating more than one at a time may throw an
	 * exception (TBD)
	 */
	static unsigned extentMembers ;
	
	/**
	 * The current AdapterInterface object. 
	 */
	static AdapterInterface * current ;
	
	/**
	 * A pointer to all GameObjects in World
	 */
	static vector<GameObject*> ** WorldObjects ;
	
	
	
	
public:
	
	AdapterInterface() {} ;
	
	/**
	 * Called by a derived class to instantiate. Main constructor.
	 */
	AdapterInterface(AdapterInterface * current) ;
	
	/**
	 * Copy constructor
	 */
	AdapterInterface(AdapterInterface &) {}
	
	/**
	 * Move constructor
	 */
	AdapterInterface(AdapterInterface &&) {}
	

	virtual ~AdapterInterface() ;
	
	/**
	 * Assignment operator overload (copy)
	 */
	AdapterInterface & operator=(const AdapterInterface &) {return *this;}
	
	/**
	 * Assignment operator overload (move)
	 */
	AdapterInterface & operator=(AdapterInterface &&) {return *this;}
	
	virtual void init() ;
	
	virtual void show() const ;
	
	/**
	 * See show()
	 */
	virtual void operator()() const ;
	
	virtual void close() ;
	
	
};


#endif /* defined(__GameWorld__AdapterInterface__) */
