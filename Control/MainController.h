//
//  MainController.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__MainController__
#define __GameWorld__MainController__

#include <iostream>

#include <SDL2/SDL.h>

#include "../Util/Debug.h"
#include "../Util/Time.hpp"

#include "../Output/GraphicalOutput.h"

#include "../GameWorld/GameData.h"
#include "../GameWorld/WorldController.h"

#include "../Control/Player.h"
#include "../Control/Input.hpp"


using namespace std ;


class MainController {
	
private:
	
	static Player * player ;
	static void exit() ;

public:
	
	/**
	 * This function will call exit at some predetermined point, or when
	 * some criterion is met (TBD - see implementation).
	 * This is neccesary since InputController, which depends on SDL, needs to run on the main thread, 
	 * and since each controller runs a while loop, it will not return until the bool* it checks changes.
	*/
	static void setupMainContrExit() ;

	static void init() ;
	static void exec() ;

} ;


#endif /* defined(__GameWorld__MainController__) */