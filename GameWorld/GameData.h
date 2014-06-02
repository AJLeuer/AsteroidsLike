//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

/**
 * This header holds important data and definitions used throughout the program
 */

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h

#define MAX_X 1600
#define MIN_X 0
#define MAX_Y 900
#define MIN_Y 0

/* comment this out (or #undef) when not running in debug mode */
#define DEBUG_MODE

/* comment this out (or #undef) when not running in high-dpi mode */
#define HIGH_DPI

#ifdef  HIGH_DPI
#define WINDOW_SIZE_X (MAX_X / 2)
#define WINDOW_MIN_X (MIN_X / 2)
#define WINDOW_SIZE_Y (MAX_Y / 2)
#define WINDOW_MIN_Y (MIN_Y / 2)

#else
#define WINDOW_MAX_X MAX_X
#define WINDOW_MIN_X MIN_X
#define WINDOW_MAX_Y MAX_Y
#define WINDOW_MIN_Y MIN_Y
#endif

#define eight_milliseconds std::chrono::milliseconds(8)

#include <iostream>
#include <ostream>
#include <sstream>
#include <mutex>
#include <vector>

#include <SDL2/SDL_render.h>

#include "../Util/Debug.h"
#include "../Util/BoundsCheck.hpp"
#include "../Util/Time.hpp"

#include "ForwardDecl.h"

using namespace std ;

/**
 * A singleton class that will be used purely for shared storage and retrieval.
 * It, and all its members, should be accessible to every other class.
 * It will have only one, statically initialized instance.
 * GameData.h also holds various statics, globals and defines that are used throughout the scope
 * of the program.
 */
struct SharedGameData {
	
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
	
	SharedGameData() ;
	
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

enum class AssetType {
	block = 0,
    ship = 1
	//scenery? what else do we need?
	//add more here
} ;

extern unsigned globalGameLoopCount ;

template<typename N = float>
N defaultOffset = 4 ;

extern Time<nanosecPrecisionClock> * mainGameClock ;

extern std::mutex sharedMutex ;

extern bool GLOBAL_CONTINUE_SIGNAL ;

constexpr int GLOBAL_MAX_X = MAX_X ;
constexpr int GLOBAL_MIN_X = MIN_X ;
constexpr int GLOBAL_MAX_Y = MAX_Y ;
constexpr int GLOBAL_MIN_Y = MIN_Y ;

template<typename N>
static const BoundsCheck<N> defaultCheck(GLOBAL_MIN_X, GLOBAL_MAX_X, GLOBAL_MIN_Y, GLOBAL_MAX_Y) ;

extern string currentDirectory ;


#endif
