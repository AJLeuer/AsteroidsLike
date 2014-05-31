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

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "../Util/Configuration.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/Time.h"
#include "../Util/AssetFileIO.h"

#include "../Input/Input.h"

#include "../GameWorld/GameData.h"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/NPC.h"
#include "../GameWorld/WorldController.h"

#include "MainController.h"


int main(int argc, char ** argv) {
	
	currentDirectory = argv[0] ;
	cout << "the current directory is: " << currentDirectory << endl ;
	auto s = AssetFileIO::getRandomImageFilename(AssetType::character) ;

	Debug::init(true) ;
    
    float a = 2.56 ;
    int b = static_cast<int>(a) ;
    
    
	MainController::init() ;
	MainController::exec() ;
     
	//MainController calls its own exit()
    
    

	return 0;
}







