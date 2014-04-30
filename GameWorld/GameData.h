//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h

#define GLOBAL_MAX_X_ 960
#define GLOBAL_MIN_X_ 0
#define GLOBAL_MAX_Y_ 540
#define GLOBAL_MIN_Y_ 0


#include <iostream>
#include <ostream>
#include <vector>

#include "../Util/BoundsCheck.hpp"
#include "ForwardDecl.h"
//#include "GameMap.hpp"

using namespace std ;

/**
 * A singleton class that will be used purely for shared storage and retrieval.
 * It, and all its members, should be accessible to every other class.
 * It will have only one, statically initialized instance.
 */

struct SharedGameData {
	
private:
	SharedGameData() ;
	
public:
	/**
	 * A container holding most objects in the game world
	 */
	static const vector<GameObject*> * gameObjects ;
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * map ;
	
	
	static void initData(vector<GameObject*> *, const GameMap<GameObject> *) ;
} ;

extern std::mutex sharedMutex ;

extern bool GLOBAL_CONTINUE_SIGNAL ;

extern const long GLOBAL_MAX_X ;
extern const long GLOBAL_MIN_X ; 
extern const long GLOBAL_MAX_Y ;
extern const long GLOBAL_MIN_Y ;

static const BoundsCheck<long> defaultCheck(GLOBAL_MAX_X, GLOBAL_MIN_X, GLOBAL_MAX_Y, GLOBAL_MIN_Y) ;

extern string currentDirectory ;





#endif
