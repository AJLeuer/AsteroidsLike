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

#include "../Util/Debug.h"
#include "../Util/Time.h"

#include "../Input/Input.h"

#include "../Output/GraphicalOutput.h"

#include "../GameWorld/GameData.h"
#include "../GameWorld/WorldController.h"

#endif /* defined(__GameWorld__MainController__) */


class MainController {
	
private:
	
	static thread * outPutThread ;
	static thread * exitingThread ;
	
public:
	
	/**
	 * This function will call exit at some predetermined point, or when
	 * some criterion is met (TBD - see implementation).
	 * This is neccesary since InputController, which depends on SDL, needs to run on the main thread, 
	 * and since each controller runs a while loop, it will not return until the bool* it checks changes.
	*/
	static void setupMainContrExit() ;
	
	static void init() ;
	static void exit() ;
} ;