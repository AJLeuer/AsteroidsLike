//
//  MainControl.cpp
//  World
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainControl.h"

using namespace std ;


const unsigned * MainControl::loopCount = & mainGameLoopCount ;

void MainControl::begin_exit() {
	GLOBAL_CONTINUE_FLAG = false ;
}

void MainControl::setupMainContrExit() {
	
	std::atexit(& MainControl::exitmc) ;
	
	/* Signal handling */
	signal(SIGQUIT, & MainControl::exitmc) ;
	signal(SIGABRT, & MainControl::exitmc) ;
	signal(SIGTERM, & MainControl::exitmc) ;
	
	/* Register for MainControl::exit() to be called if a quit event is initiated (i.e. user clicks
	  window close button, presses ⌘Q, etc */
	EventRegister * quitEvent = new EventRegister(&MainControl::begin_exit, EventType::SDL_QUIT) ;
	InputControl::registerForEvent(quitEvent) ;
}

void reverseTimeFlow() {
	if (timeFlow == TimeFlow::forward) {
		timeFlow = TimeFlow::reverse ;
	}
	else { //if (timeFlow == TimeFlow::reverse)
		timeFlow = TimeFlow::forward ;
	}
} ;

void MainControl::setupCallbacks() {
    KeyPressEventRegister * onKeypressReverseTime = new KeyPressEventRegister(reverseTimeFlow, KeyPressEventRegister::KeyPressEventType::KeyPressed, {"1"}, KeypressEvaluationMethod::exactlyOne) ;
	InputControl::registerForEvent(onKeypressReverseTime) ;
}

void MainControl::init() {
	
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
	InputControl::init() ;
	MainControl::setupCallbacks() ;
	WorldControl::init() ;    //must be last, will init GameState as well
	Weapon::init() ;
	
	//players (i.e. of class Player) will set up callbacks from actual player (i.e. live human) input
	Player::defaultPlayer0 = new Player("Player 0", "Ship1_Blue.png", Player::defaultGeometrySize,
		Player::position_in_defaultStartingArea(), 0.0, "Blue", 500, 100, AssetFile::projectileImageFilenames->at(0)) ;
	
	Player::defaultPlayer0->displayVelocity( { static_cast<float>((globalMaxX() * 0.70)), static_cast<float>((globalMaxY() * 0.85))},
						     {142, 255, 8, 0}, {0, 0, 0, 0}) ;
	
	/*
	Player::defaultPlayer1 = new Player("Player 1", "Ship1_Green.png", Player::defaultGeometrySize,
		Player::position_in_defaultStartingArea(), 0.0, "Green", 500, 100, AssetFile::projectileImageFilenames->at(1)) ; 

	Player::defaultPlayer1->displayVelocity( { static_cast<float>((globalMaxX() * 0.15)), static_cast<float>((globalMaxY() * 0.85))},
							 {251, 0, 107, 0}, {0, 0, 0, 0}) ;
	
	 */
	
	//setup MainControl to exit() later (typically with a callback assigned to a keypress)
	setupMainContrExit() ;
}

void MainControl::main() {
    
	/* Start main functions for all controller classes. WorldControl manages the world on its own thread, and input and output
	 switch off on the main thread */
	WorldControl::begin_main() ;
	
	GraphicalOutput::drawFPS() ;
	
	auto & rt = refreshTime ; //debug symbol
	
	while (GLOBAL_CONTINUE_FLAG) {
		
		auto * mloop = &mainGameLoopCount ; //debug var, delete this
		auto * wloop = &worldLoopCount ; //debug var, delete
		thread & worldEventThread = WorldControl::worldEventThread ; //debug var
		
		/* Will need to lock the mutex when shutting down */
		
		auto startTime = GameState::mainGameClock->checkTimeElapsed() ;
		
		GraphicalOutput::update() ;
		GraphicsData<float, int>::checkForCollisions() ;
		InputControl::update() ;
		
		if (GLOBAL_CONTINUE_FLAG == false) { /* we check here because setting false will have been done by callback during InputControl::update() */
			break ;
		}
		
		auto time2 = GameState::mainGameClock->checkTimeElapsed() ;
		auto timeElapsed = time2 - startTime ;
		auto sleepTime = (refreshTime - timeElapsed) ;
		
		mainGameLoopCount++ ;
		
		this_thread::sleep_for(sleepTime) ;
		
		if (mainGameLoopCount > worldLoopCount) {
			unique_lock<mutex> locked(mainThreadsSync) ;
			
			shared_conditional.wait(locked) ;
		}

		shared_conditional.notify_all() ;
	}

	/* exit signaled GLOBAL_CONTINUE_FLAG. We're outta here! Handing off to MainControl::exit() */
}

void MainControl::exitmc(int sig) {
	bool & CONTINUE_FLAG = GLOBAL_CONTINUE_FLAG	; //debug var
    GLOBAL_CONTINUE_FLAG = false ;
    /* other signals to define false here? */
        
    TextOutput<float, int>::exit() ; /* quits() sdl_ttf */
    GraphicalOutput::exit() ;
    WorldControl::exit() ;
    InputControl::exit() ;
	
		
    SDL_Quit() ; /* Call this only making all calls to SDL_QuitSubSystem() */
	
	shared_conditional.notify_all() ; //so no one is left waiting on shared_conditional
	
    GameState::mainGameClock->stopTimer() ;

	
	exit(sig) ;
	
	/* And we're done! Returning now... */
}




