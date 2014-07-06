//
//  GameState.h
//  World
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
#include <array>
#include <vector>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "../Util/Util.hpp"
#include "../Util/Debug.h"
#include "../Util/Position.hpp"
#include "../Util/BoundsCheck.hpp"
#include "../Util/Timer.hpp"

#include "../Output/OutputData.hpp"

#include "ForwardDecl.h"

#include "../Control/DefaultConfig.h"

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
	
	static vector<OutputData> * addlOutputStorage ;
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * map ;
    
	static Window * window ;
	
	//this will allow GameObjects to initialize their own textures
	static Renderer * renderer ;
    
	static bool dataIsInit ;
	static bool graphicsAreInit ;
	
	GameState() ;
	
	friend class GraphicalOutput ;
	friend class WorldController ;
	friend class MainController ;
	
public:
	
	static bool GLOBAL_CONTINUE_SIGNAL ;
	
	static Timer * mainGameClock ;
	
	static std::mutex sharedMutex ;
	
	static string currentDirectory ;
    
    
	
	static void initData(vector<GameObject*> *, const GameMap<GameObject> *) ;
    
	static void initGraphics(Window *, Renderer *) ;
	
	/**
	 * A container holding most objects in the game world.
	 * Note: some indices of the returned vector<GameObject*> may be null. Any class calling this method is
	 * responsible for checking for null pointer.
	 */
	static const vector<GameObject*> * getGameObjects() ;
	
	/**
	 * Holds pointers to GameObjects like gameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with GameObjects's map.
	 */
	static const GameMap<GameObject> * getMap() ;
	
	static Window * getMainWindow() ;
    
	static Renderer * getMainRenderer() ;
	
	
} ;


/* Globals */

/**
 * The graphics and input loop, managed together by MainController 
 */
extern unsigned mainGameLoopCount ;

/**
 * Loop managed by WorldController
 */
extern unsigned worldLoopCount ;

/**
 * Returns the window origin coordinates (i.e. {0, 0, 0}
 * within the window's own coordinate system), but represented
 * as coordinates within the game world. Useful for translating
 * between the two coordinate systems.
 *
 * @return The window origin as world coordinates
 */
template<typename N>
Position<N> getWindowOriginAsWorldCoord() {
	
	/* center the game window frame (i.e. what is visible) inside the world */
	
	N leftX = (globalMaxX() - windowSizeX()) / 2 ;
	N topY = (globalMaxY() - windowSizeY()) / 2 ;
	
	return Position<N>(leftX, topY, 0) ;
}







#define GLOBAL_CONTINUE_SIGNAL GameState::GLOBAL_CONTINUE_SIGNAL

#endif /* defined(__GameWorld__GameState__) */
