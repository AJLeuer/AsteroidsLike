//
//  main.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cmath>
#include <limits>

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/Timer.hpp"
#include "../Util/AssetFileIO.h"
#include "../Util/Velocity.hpp"

#include "../World/GameState.hpp"
#include "../World/GameObject.h"
#include "../World/NPC.h"
#include "../World/WorldController.h"

#include "../Control/Configuration.h"
#include "../Control/Input.hpp"
#include "MainController.h"


int main(int argc, char ** argv) {
	
	GameState::currentDirectory = argv[0] ;
	cout << "the current directory is: " << GameState::currentDirectory << endl ;
	
	std::thread::id id = this_thread::get_id() ;

	MainController::init() ;
	MainController::main() ;
     
	//MainController calls its own exit()
    
	return 0;
}







