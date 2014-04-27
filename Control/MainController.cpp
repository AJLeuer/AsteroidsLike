//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainController.h"


AdapterInterface<GameObject> * MainController::currentOutAdapter = nullptr ;
thread * MainController::outPutThread = nullptr ;

void MainController::setupMainContrExit() {
	//we can choose whatever method we want for when or how to tell InputController to exit() later
	//we could also set up at timer that calls exit() after a certain amount of time, and call setInpContrExit() to run on its own thread during MainController::init()
	KeyInputRegister<GameObject> exitKey = KeyInputRegister<GameObject>("-", &MainController::exit) ;
	InputController<GameObject>::registerForKeypress(exitKey) ;
}

void MainController::init() {
	GLOBAL_CONTINUE_SIGNAL = true ;
	
	currentOutAdapter = new TestOutputAdapter<GameObject>() ;
	
	//do initializations
	WorldController::init() ;
	InputController<GameObject>::init() ;
	currentOutAdapter->init(WorldController::getGameObjects()) ;
	
	//setup MainController to exit() later
	setupMainContrExit() ;
	
	//start main functions for all controller classes
	WorldController::exec() ;
	outPutThread = new std::thread(&AdapterInterface<GameObject>::show, currentOutAdapter) ; //runs OutputAdapter.show() on its own thread
	InputController<GameObject>::exec() ;
}



void MainController::exit() {
	GLOBAL_CONTINUE_SIGNAL = false ; // b is the same as GLOBAL_CONTINUE_SIGNAL normally.
	
	outPutThread->join() ;
	
	WorldController::exit() ;
	InputController<GameObject>::exit() ; //joins GameObjects threads also
	currentOutAdapter->exit() ;
	
	delete outPutThread ;
	delete currentOutAdapter ;
}




