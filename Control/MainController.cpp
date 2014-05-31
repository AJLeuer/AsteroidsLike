//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainController.h"

using namespace std ;

/* not safe to initialize this yet */
Player * MainController::player = nullptr ;

void MainController::setupMainContrExit() {
	/*we can choose whatever method we want for when or how to tell InputController to exit() later
	we could also set up at timer that calls exit() after a certain amount of time, and call setInpContrExit() to run on its own thread during MainController::init() */
	KeyInputRegister * exitKey = new KeyInputRegister("-", &MainController::exit) ;
	InputController::registerForKeypress(exitKey) ;
}

void MainController::init() {
	GLOBAL_CONTINUE_SIGNAL = true ;
	
	//do initializations
	SDL_Init(0) ; /*SDL requires initializing with SDLInit(0) first, then we can init subsystems as we need them.
				  e.g. SDL_InitSubSystem(SDL_INIT_EVENTS) in MainInputController::init() */

	GraphicalOutput::init() ;
	InputController::init() ;
	WorldController::init() ;    //must be last
	player = new Player() ;

	//setup MainController to exit() later (typically with a callback assigned to a keypress)
	setupMainContrExit() ;
}

void MainController::exec() {
	/*start main functions for all controller classes. WorldController::exec() runs on its own thread, and input and output
	 switch off on the main thread */
	WorldController::exec() ;

	while (GLOBAL_CONTINUE_SIGNAL) {
		GraphicalOutput::update() ;
		InputController::update() ;
	}
}

void MainController::exit() {

	GLOBAL_CONTINUE_SIGNAL = false ; 

	WorldController::exit() ;
	GraphicalOutput::exit() ;
	InputController::exit() ;
	SDL_Quit() ; /* Call this only making all calls to SDL_QuitSubSystem() */
}




