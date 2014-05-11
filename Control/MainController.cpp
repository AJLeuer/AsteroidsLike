//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainController.h"


thread * MainController::outPutThread = nullptr ;

void MainController::setupMainContrExit() {
	//we can choose whatever method we want for when or how to tell InputController to exit() later
	//we could also set up at timer that calls exit() after a certain amount of time, and call setInpContrExit() to run on its own thread during MainController::init()
	KeyInputRegister<GameObject> exitKey = KeyInputRegister<GameObject>("-", &MainController::exit) ;
	InputController<GameObject>::registerForKeypress(exitKey) ;
}

void MainController::init() {
	GLOBAL_CONTINUE_SIGNAL = true ;
	
	//do initializations
	SharedGameData::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
	WorldController::init() ;
	InputController<GameObject>::init() ;
	GraphicalOutput::init() ;
	
	//setup MainController to exit() later (typically with a callback assigned to a keypress)
	setupMainContrExit() ;
	
	/*start main functions for all controller classes. WorldController::exec() runs on its own thread, and input and output
	switch off on the main thread */
	WorldController::exec() ;
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		GraphicalOutput::update() ;
		InputController<GameObject>::update() ;
	}
}



void MainController::exit() {
	GLOBAL_CONTINUE_SIGNAL = false ; 
	
	outPutThread->join() ;
	
	WorldController::exit() ;
	InputController<GameObject>::exit() ; //joins GameObjects threads also

	delete outPutThread ;
	
}




