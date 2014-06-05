//
//  GameState.h
//  GameWorld
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameState__
#define __GameWorld__GameState__


#include <iostream>
#include <ostream>
#include <sstream>
#include <mutex>
#include <vector>

#include <SDL2/SDL_render.h>

#include "../Util/Util.hpp"
#include "../Util/Debug.h"
#include "../Util/BoundsCheck.hpp"
#include "../Util/Time.hpp"

#include "ForwardDecl.h"

#include "../Control/Configuration.h"

using namespace std ;

/**
 * A singleton class that will be used purely for shared storage and retrieval.
 * It, and all its members, should be accessible to every other class.
 * It will have only one, statically initialized instance.
 * GameState.h also holds various statics, globals and defines that are used throughout the scope
 * of the program.
 */
struct GameState {
	
private:
	
	/**
	 * A container holding most objects in the game world
	 */
	static const vector<GameObject*> * gameObjects ;
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * map ;
    
	//this will allow GameObjects to initialize their own textures
	static SDL_Renderer * renderer ;
    
	static bool dataIsInit ;
	static bool graphicsAreInit ;
	
	GameState() ;
	
public:
	
	static void initData(vector<GameObject*> *, const GameMap<GameObject> *) ;
    
	static void initGraphics(SDL_Renderer *) ;
	
	/**
	 * A container holding most objects in the game world
	 */
	static const vector<GameObject*> * getGameObjects() ;
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * getMap() ;
    
	static SDL_Renderer * getMainRenderer() ;
	
} ;

/* Globals */

/* The graphics and input loop, managed together by MainController */
extern unsigned mainGameLoopCount ;

/* Loop managed by WorldController */
extern unsigned worldLoopCount ;

template<typename N = float>
N defaultOffset = 4 ;

extern Time<nanosecPrecisionClock> * mainGameClock ;

extern std::mutex sharedMutex ;

extern bool GLOBAL_CONTINUE_SIGNAL ;

template<typename N>
FastRand<N> randPosSetter(findSmallest<N>({GLOBAL_MIN_X, GLOBAL_MIN_Y}), findLargest<N>({GLOBAL_MAX_X, GLOBAL_MAX_Y})) ;

template<typename N>
static const BoundsCheck<N> defaultCheck(GLOBAL_MIN_X, GLOBAL_MAX_X, GLOBAL_MIN_Y, GLOBAL_MAX_Y) ;

extern string currentDirectory ;

#endif /* defined(__GameWorld__GameState__) */
