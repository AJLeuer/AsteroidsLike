//
//  WorldController.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__WorldController__
#define __GameWorld__WorldController__

#include <unistd.h>

#include <iostream>
#include <ostream>
#include <vector>
#include <thread>

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/GameRandom.hpp"
#include "../Util/Timer.hpp"

#include "GameState.h"
#include "GameMap.hpp"
#include "GameInterface.h"
#include "GameEvent.h"
#include "Ability.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Character.h"
#include "NPC.h"

#include "../Control/Configuration.h"



using namespace std ;

/**
 * The class controlling the world managing all the objects within it.
 * It is generally responsible for managing everything that happens within the scope of a game. 
 * It holds all the GameObjects, mediates their interactions when neccessary (more or less, in many
 * cases they interact directly), handles output of the game state (whether that means realtime or recorded.
 * rendered or printed. etc. - it was designed to be flexible and extensible), and handles initialization and
 * clean up. It is essentially all static should be treated as a singleton object: don't try to create instances
 * of it. Rather call init() once at the beginning, and end the game program with exit().
 */
class WorldController {
	
protected:

	
	/**
	 * A container holding most objects in the game world
	 */
	static vector<GameObject*> * gameObjects ; /* Convenience to access GameObject::getAllGameObjects() */
	
	/* We might need these later. Or we might not */
	//static vector<GameObject*> * secondaryGameObjects ;
	//static vector<GameObject*> * obstacleObjects ;
 
	static thread mainThread ;
	static thread checkDelThread ;
	
	
	
	friend class GameInterface ;
	friend class GameObject ;
	
	static void main() ;
	
	WorldController() ;
	
	/* can umcomment this later if we end up needing it */
	//static void update() ;
	
public:
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * map ;
	
	static void init() ;
	
	static void begin_main() ;
	
	static void exit() ;
	
} ;

#endif /* defined(__GameWorld__WorldController__) */
