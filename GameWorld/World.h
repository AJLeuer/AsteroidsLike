//
//  World.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__World__
#define __GameWorld__World__

#include <iostream>
#include <ostream>
#include <vector>

#include "AddlEnums.h"
#include "Location.h"
#include "GameInterface.h"
#include "GameEvent.h"
#include "Ability.h"
#include "GameObject.h"
#include "Character.h"
#include "NPC.h"



using namespace std ;

/**
 * The class representing the world, and the class that manages all the objects within it. 
 * It is generally responsible for managing everything that happens within the scope of a game. 
 * It holds all the GameObjects, mediates their interactions when neccessary (more or less, in many 
 * cases they interact directly), handles output of the game state (whether that means realtime or recorded.
 * rendered or printed. etc. - it was designed to be flexible and extensible), and handles initialization and
 * clean up. It is essentially all static should be treated as a singleton object: don't try to create instances
 * of it. Rather call init() once at the beginning, and end the game program with clear() or end().
 */
class World {
	
private:
	World() ;
	
public:
	
	friend class GameInterface ;
	
	/**
	 * gameLog is a pointer which references the basic ostream* which will
	 * record, print, draw, or render everything that happens in our game. As it is defined now, it
	 * may in some cases point to std::cout, in other cases it may record to a stringstream. If
	 * other means of outputting that game state are created we may change the type of gameLog later.
	 */
	static std::ostream * gameLog ;
	
	static bool running ; //has World started yet?
	
	static vector<GameObject*> allObjects ;
	
	static void init() ;
	
	/**
	 * Plays the game in real time. Cannot be called if gameRecorded() is active. Attempting
	 * to call both concurrently will result in an exception being thrown.
	 */
	static void playGameInRealTime() ;
	
	/**
	 * Plays the game and saves all events to an output stream.
	 */
	static void playGameRecorded(std::ostream * writeTO) ;
	
	
	
};

#endif /* defined(__GameWorld__World__) */
