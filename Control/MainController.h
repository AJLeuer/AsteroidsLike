//
//  MainController.h
//  World
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__MainController__
#define __GameWorld__MainController__

#include <iostream>
#include <csignal>
#include <mutex>      
#include <condition_variable>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "../Util/Debug.h"
#include "../Util/Velocity.hpp"
#include "../Util/Timer.hpp"
#include "../Util/BasicConcurrency.h"

#include "../Output/GraphicalOutput.h"
#include "../Output/TextOutput.h"

#include "../World/GameState.hpp"
#include "../World/WorldController.h"

#include "../Control/Configuration.h"
#include "../Control/Player.h"
#include "../Control/Input.hpp"


using namespace std ;


class MainController {

protected:
	
	static Player * player0 ;
	static Player * player1 ;
	static const unsigned * loopCount ; //Debug symbol, delete
	
	static void begin_exit() ;
	
	/**
	 * This function will call exit at some predetermined point, or when
	 * some criterion is met (TBD - see implementation).
	 * This is neccesary since InputController, which depends on SDL, needs to run on the main thread,
	 * and since each controller runs a while loop, it will not return until the bool* it checks changes.
	 */
	static void setupMainContrExit() ;
	
	/**
	 * @brief Exits the game
	 * 
	 * @see init()
	 */
	static void exit(int sig = 0) ;
	
public:
	
	static void init() ;
	static void main() ;

} ;


#endif /* defined(__GameWorld__MainController__) */