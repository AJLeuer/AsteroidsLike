//
//  WorldController.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__WorldController__
#define __GameWorld__WorldController__

#include <iostream>
#include <ostream>
#include <vector>
#include <thread>

#include "../Util/Debug.h"
#include "../Util/Util.h"
#include "../Util/Location.hpp"
#include "../Util/BasicTime.h"
#include "GameMap.hpp"
#include "GameInterface.h"
#include "GameEvent.h"
#include "Ability.h"
#include "GameObject.h"
#include "Character.h"
#include "NPC.h"



using namespace std ;

/**
 * The class controlling the world managing all the objects within it.
 * It is generally responsible for managing everything that happens within the scope of a game. 
 * It holds all the GameObjects, mediates their interactions when neccessary (more or less, in many 
 * cases they interact directly), handles output of the game state (whether that means realtime or recorded.
 * rendered or printed. etc. - it was designed to be flexible and extensible), and handles initialization and
 * clean up. It is essentially all static should be treated as a singleton object: don't try to create instances
 * of it. Rather call init() once at the beginning, and end the game program with clear() or end().
 */
class WorldController {
	
private:
	
	
	friend class GameInterface ;
	
	/**
	 * gameLog is a pointer which references the basic ostream* which will
	 * record, print, draw, or render everything that happens in our game. As it is defined now, it
	 * may in some cases point to std::cout, in other cases it may record to a stringstream. If
	 * other means of outputting that game state are created we may change the type of gameLog later.
	 */
	static ostream * gameLog ;
	
	static bool running ; //has WorldController started yet?
	
	WorldController() ;
	
public:
	
	/**
	 * A container holding most objects in the game world
	 */
	static list<GameObject*> * gameObjects ; //Adapters need to access this too
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its location. Is synced with GameObjects's map.
	 */
	static GameMap<GameObject> * map ;
	
	static mutex runningMtx ;
	
	static const long MAX_X ;
	static const long MIN_X ;
	static const long MAX_Y ;
	static const long MIN_Y ;
	
	static ostream ** getGameLog() { return & gameLog ; }
	
	static bool isRunning() { return running ; }
	
	static void init() ;
	
	static void foo(double xyOffs, unsigned long, bool * b) ;
	
	/**
	 * Plays the game in real time. Cannot be called if gameRecorded() is active. Attempting
	 * to call both concurrently will result in an exception being thrown.
	 */
	static void playGameInRealTime() ;
	
	/**
	 * Plays the game and saves all events to an output stream.
	 */
	static void playGameRecorded(std::ostream * writeTo) ;
	
	static void close() ;
	
	
	
};

#endif /* defined(__GameWorld__WorldController__) */
