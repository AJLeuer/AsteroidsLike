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

#include "Location.h"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/World.h"



/**
 * Much like World, Adapter is a singleton with all static class variables and functions. It has access to World's data
 * members, but while World is in charge of manipulating objects in the GameWorld and mediating their interactions,
 * Adapter only uses World's data to update the view
 */
class Adapter {
	
private:
	friend class World ;
	static WINDOW *  gameWindow ;
	
	/**
	 * A pointer to all GameObjects in World
	 */
	static vector<GameObject*> * WorldObjects ;
	
	/**
	 * This class won't be instantiated, should be treated as
	 * a singleton (call init)
	 */
	Adapter() ;
	
public:
	static void init() ;
	/*static void* resizeHandler() ; */  //maybe finish this later
	static void start() ;
	static void close() ;
	
};

#endif /* defined(__GameWorld__Adapter__) */
