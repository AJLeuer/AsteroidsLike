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

const unsigned * MainController::loopCount = &mainGameLoopCount ; //Debug symbol, delete

void MainController::setupMainContrExit() {
	
	/* Register for MainController::exit() to be called if a quit event is initiated (i.e. user clicks
	  window close button, presses ⌘Q, etc */
	EventRegister * quitEvent = new EventRegister(&MainController::exit, EventType::SDL_QUIT) ;
	InputController::registerForEvent(quitEvent) ;
}

void MainController::init() {
	
	GameState::mainGameClock->startTimer() ;
	GLOBAL_CONTINUE_SIGNAL = true ;
	
	//do initializations
   GameState::initCoordinateSystems() ;
	
	int sdlinit_error = SDL_Init(0) ;
	
	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_Init() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}

	GraphicalOutput::init() ;
	InputController::init() ;
	WorldController::init() ;    //must be last, will init GameState as well
	player = new Player() ;

	//setup MainController to exit() later (typically with a callback assigned to a keypress)
	setupMainContrExit() ;
}

void MainController::main() {
    
	/* Start main functions for all controller classes. WorldController manages the world on its own thread, and input and output
	 switch off on the main thread */
	WorldController::begin_main() ;
	
	auto rt = refreshTime ; //debug symbol
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		auto startTime = GameState::mainGameClock->checkTimeElapsed() ;
		
		GraphicalOutput::update() ;
		InputController::update() ;
		
		auto time2 = GameState::mainGameClock->checkTimeElapsed() ;
		auto timeElapsed = time2 - startTime ;
		
		auto sleepTime = (refreshTime - timeElapsed) ;
		mainGameLoopCount++ ;
		this_thread::sleep_for(sleepTime) ;
        
       /* Sync loops */
       while (mainGameLoopCount > worldLoopCount) {
           /* wait */
       }
	}
}

void MainController::exit() {

	GLOBAL_CONTINUE_SIGNAL = false ; 

	WorldController::exit() ;
	GraphicalOutput::exit() ;
	InputController::exit() ;
	SDL_Quit() ; /* Call this only making all calls to SDL_QuitSubSystem() */
	GameState::mainGameClock->stopTimer() ;
}




