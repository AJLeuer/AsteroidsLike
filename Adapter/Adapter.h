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
#include <thread>

#include "AdapterUtil.h"
#include "AdapterInterface.h"

#include "Util.h"
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
	
	void show_threaded() ;

public:
	
	/**
	 * This class should be instantiated only once, and not with this constructor. Call Adapter(int).
	 * Trying to create more than one instance of Adapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	Adapter() : AdapterInterface() {} ;
	
	Adapter(int n) : AdapterInterface(this) {} ;
	
	/**
	 * Copy constructor.
	 */
	Adapter(Adapter &) : AdapterInterface() {}
	
	/**
	 * Move constructor. Will change AdapterInterface's pointer to new object
	 */
	Adapter(Adapter &&) : AdapterInterface(this) {}
	
	/**
	 * Assignment operator overload (copy)
	 */
	Adapter & operator=(const Adapter &){ return *this ;}
	
	/**
	 * Assignment operator overload (move)
	 */
	Adapter & operator=(Adapter &&){ AdapterInterface(this) ; return *this ;}
	
	~Adapter() {} //should automatically call ~AdapterInterface()
	
	
	void init() ;

	void show() ;
	
	
	/**
	 * See show()
	 */
	void operator()() ;
	
	void close() ;
	
};

#endif /* defined(__GameWorld__Adapter__) */
