//
//  MainController.cpp
//  World
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainController.h"

using namespace std ;

/* not safe to initialize this yet */
Player * MainController::player0 = nullptr ;
Player * MainController::player1 = nullptr ;

const unsigned * MainController::loopCount = &mainGameLoopCount ; //Debug symbol, delete

void MainController::begin_exit() {
	GLOBAL_CONTINUE_FLAG = false ;
}

void MainController::setupMainContrExit() {
	
	/* Signal handling */
	signal(SIGQUIT, &MainController::exit) ;
	signal(SIGABRT, &MainController::exit) ;
	signal(SIGTERM, &MainController::exit) ;
	
	/* Register for MainController::exit() to be called if a quit event is initiated (i.e. user clicks
	  window close button, presses ⌘Q, etc */
	EventRegister * quitEvent = new EventRegister(&MainController::begin_exit, EventType::SDL_QUIT) ;
	InputController::registerForEvent(quitEvent) ;
}

auto reverseTimeFlow = [] () -> void {
	if (timeFlow == TimeFlow::forward) {
		timeFlow = TimeFlow::reverse ;
	}
	else { //if (timeFlow == TimeFlow::reverse)
		timeFlow = TimeFlow::forward ;
	}
} ;

void MainController::setupCallbacks() {
	KeyInputRegister * onKeypressReverseTime = new KeyInputRegister(reverseTimeFlow, {"1"}, KeypressEvaluationMethod::any) ;
	InputController::registerForKeypress(onKeypressReverseTime) ;
}

void MainController::init() {
	
	GameState::mainGameClock->startTimer() ;
	GLOBAL_CONTINUE_FLAG = true ;
	mainThreadID = this_thread::get_id() ;
	
	//do initializations
    Configuration::init() ;
	Debug::init() ;
	
	int sdlinit_error = SDL_Init(0) ;
	
	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_Init() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	
	GraphicalOutput::init() ;
    TextOutput<float, int>::init() ;
	InputController::init() ;
	MainController::setupCallbacks() ;
	WorldController::init() ;    //must be last, will init GameState as well
	
	Player::initDefaultPlayers() ;
	
	player0 = Player::defaultPlayer0 ;
	player1 = Player::defaultPlayer1 ;
	
	player0->displayVelocity( { static_cast<float>((globalMaxX() * 0.70)), static_cast<float>((globalMaxY() * 0.85))},
						     {142, 255, 8, 0}, {0, 0, 0, 0}) ;
	
	player1->displayVelocity( { static_cast<float>((globalMaxX() * 0.15)), static_cast<float>((globalMaxY() * 0.85))},
							 {251, 0, 107, 0}, {0, 0, 0, 0}) ;
	
	//setup MainController to exit() later (typically with a callback assigned to a keypress)
	setupMainContrExit() ;
}

void MainController::main() {
    
	/* Start main functions for all controller classes. WorldController manages the world on its own thread, and input and output
	 switch off on the main thread */
	WorldController::begin_main() ;
	
	auto rt = refreshTime ; //debug symbol
	
	while (GLOBAL_CONTINUE_FLAG) {
		
		auto * mloop = &mainGameLoopCount ; //debug var, delete this
		auto * wloop = &worldLoopCount ; //debug var, delete
		
		/* Will need to lock the mutex when shutting down */
		
		auto startTime = GameState::mainGameClock->checkTimeElapsed() ;
		
		GraphicalOutput::update() ;
		InputController::update() ;
		
		if (GLOBAL_CONTINUE_FLAG == false) { /* we check here because setting false will have been done by callback during InputController::update() */
			break ;
		}
		
		auto time2 = GameState::mainGameClock->checkTimeElapsed() ;
		auto timeElapsed = time2 - startTime ;
		auto sleepTime = (refreshTime - timeElapsed) ;
		
		mainGameLoopCount++ ;
		
		//this_thread::sleep_for(sleepTime) ;
		
		if (mainGameLoopCount > worldLoopCount) {
			unique_lock<mutex> locked(syncMutex) ;
			
			conditionalWait.wait(locked) ;
		}

		conditionalWait.notify_all() ;
	}

	/* exit signaled GLOBAL_CONTINUE_FLAG. We're outta here! Handing off to MainController::exit() */
	exit() ;
}

void MainController::exit(int sig) {
	
	if (GLOBAL_CONTINUE_FLAG == true) {
		
		GLOBAL_CONTINUE_FLAG = false ;
		/* other signals to define false here? */
        
        TextOutput<float, int>::exit() ; /* quits() sdl_ttf */
		GraphicalOutput::exit() ;
		WorldController::exit() ;
		InputController::exit() ;
	
		
		SDL_Quit() ; /* Call this only making all calls to SDL_QuitSubSystem() */
	
		GameState::mainGameClock->stopTimer() ;
	}
	
	exit(sig) ;
	
	/* And we're done! Returning now... */
}




