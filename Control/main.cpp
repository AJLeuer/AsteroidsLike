//
//  main.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "../Util/Util.hpp"
#include "../Util/Time.h"
#include "../Util/OpenClUtil.h"

#include "../Input/Input.h"

//#include "../Output/OutputAdapter.hpp"
#include "../Output/TestOutputAdapter.hpp"

#include "../GameWorld/GameObject.h"
#include "../GameWorld/NPC.h"
#include "../GameWorld/WorldController.h"

#include "MainController.h"


int main(int argc, char ** argv) {

	Time timer ;
	timer.startTimer() ;
	Debug::init(true) ;
	
	MainController::init() ;

	return 0;
}







