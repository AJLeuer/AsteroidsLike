//
//  main.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <unistd.h>
#include <fcntl.h>

#include <cstdlib>
#include <cmath>
#include <limits>
#include <functional>

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "../Util/Util.hpp"
#include "../Util/Vect.hpp"
#include "../Util/Line.hpp"
#include "../Util/Timer.hpp"
#include "../Util/AssetFileIO.h"
#include "../Util/Velocity.hpp"

#include "../World/GameState.hpp"
#include "../World/GameObject.h"
#include "../World/NPC.h"
#include "../World/WorldControl.h"

#include "../Control/Configuration.h"
#include "../Control/Input.hpp"
#include "MainControl.h"


int main(int argc, char ** argv) {
	
	constexpr InitializeFromCenterCoordinates initializeFromCenterCoordinates ;
	
	Rectangle<float, long> rectangle0 (Vect<float>(2, 2), Size<long>(8, 8), initializeFromCenterCoordinates) ;
	Rectangle<float, long> rectangle1 (Vect<float>(4, 4), Size<long>(4, 4), initializeFromCenterCoordinates) ;
	
	bool collision = Rectangle<float, long>::detectCollision(rectangle0, rectangle1) ;
	
	GameState::currentDirectory = argv[0] ;
	cout << "the current directory is: " << GameState::currentDirectory << endl ;

	MainControl::init() ;
	MainControl::main() ;
     
	//MainControl calls its own exit()
    
	return 0 ;
}







