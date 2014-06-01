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
	
	/* Register for MainController::exit() to be called if a quit event is initiated (i.e. user clicks
	  window close button, presses ⌘Q, etc */
	EventRegister * quitEvent = new EventRegister(&WorldController::exit, EventType::SDL_QUIT) ;
	InputController::registerForEvent(quitEvent) ;
}

void MainController::init() {
	GLOBAL_CONTINUE_SIGNAL = true ;
	
	//do initializations
	
	int sdlinit_error = SDL_Init(0) ;
	
	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_Init() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

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




