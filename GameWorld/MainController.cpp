//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "MainController.h"


AdapterInterface<GameObject> * MainController::currentAdapter = nullptr ;

void MainController::start(unsigned long microseconds) {
	
	Debug::init(true) ;
	
	Time timer ;
	timer.startTimer() ;
	
	bool b = true ;
	
	WorldController::init() ;
	
	currentAdapter = new TestAdapter<GameObject>() ;
	
	currentAdapter->init(WorldController::getGameObjects()) ;
	
	currentAdapter->show(&b) ;
	
	currentAdapter->drawRepresentation(GameObject::map->getMapVect()) ;
	
	WorldController::runWorldSimulation((unsigned)microseconds) ;
	
	
	usleep(((unsigned int)microseconds)) ;
	
	b = false ;
	
	timer.stopTimer() ;
	
}

void MainController::stop() {
	
	WorldController::close() ;
	
	currentAdapter->close() ;
	delete currentAdapter ;
	
}




