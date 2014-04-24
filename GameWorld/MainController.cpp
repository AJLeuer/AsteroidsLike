//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "MainController.h"


AdapterInterface<GameObject> * MainController::currentAdapter = nullptr ;

void MainController::start(bool * b) {
	
	GLOBAL_CONTINUE_SIGNAL = b ;
	
	Debug::init(true) ;
	
	Time timer ;
	timer.startTimer() ;
	
	WorldController::init() ;
	
	currentAdapter = new Adapter<GameObject>() ;
	
	currentAdapter->init(WorldController::getGameObjects()) ;
	
	currentAdapter->show() ;
	
	WorldController::runWorldSimulation() ;
	
}

void MainController::stop(bool * b) {
	
	*b = false ; // b is the same as GLOBAL_CONTINUE_SIGNAL normally. 
	
	WorldController::close() ;
	
	currentAdapter->close() ;
	delete currentAdapter ;
	
}




