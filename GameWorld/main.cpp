//
//  main.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

/*
#define SDL_FOURCC(A, B, C, D) \
   ((SDL_static_cast(Uint32, SDL_static_cast(Uint8, (A))) << 0) | \
	(SDL_static_cast(Uint32, SDL_static_cast(Uint8, (B))) << 8) | \
	(SDL_static_cast(Uint32, SDL_static_cast(Uint8, (C))) << 16) | \
	(SDL_static_cast(Uint32, SDL_static_cast(Uint8, (D))) << 24))
*/

#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <SDL/SDL.h>
//#include <SDL/SDL_main.h>

#include "../Util/Util.hpp"
#include "../Util/Time.h"
#include "../Util/OpenClUtil.h"

#include "../Input/Input.h"

#include "../Output/Adapter.hpp"
#include "../Output/TestAdapter.hpp"

#include "MainController.h"
#include "GameObject.h"
#include "WorldController.h"
#include "NPC.h"


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char ** argv) {

	Time timer ;
	timer.startTimer() ;
	
	Input::init() ;
	
	MainController::start(GLOBAL_CONTINUE_SIGNAL) ;
	usleep(1e9) ; // sleep 1.0 x 10⁷ microseconds or 10 seconds
	
	MainController::stop(GLOBAL_CONTINUE_SIGNAL) ;
	
	
	/* end debug */
	return 0;
}

#ifdef main
#undef main
#endif
int main(int argc, char ** argv) {
	
	int r = SDL_main(argc, argv) ;
	
	return r ;
}







