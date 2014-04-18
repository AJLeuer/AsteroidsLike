//
//  MainController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/16/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "MainController.h"


AdapterInterface<GameObject> MainController::currentAdapter = Adapter<GameObject>() ;

void MainController::start(unsigned long microseconds) {
	
	Debug::init(true) ;
	
	Time timer ;
	timer.startTimer() ;
	
	bool b = true ;
	
	WorldController::init() ;
	
	currentAdapter.init(WorldController::getGameObjects()) ;
	
	currentAdapter.show(&b) ;
	
	currentAdapter.drawRepresentation(GameObject::map->getMapVect(), nullptr) ;
	
	WorldController::runWorldSimulation(0e+6 + 10) ;
	
	while (timer.checkTimeElapsed() < microseconds) {
		usleep(((unsigned int)microseconds)/100) ;
	}
	
	b = false ;
	
	timer.stopTimer() ;
	
}

void MainController::stop() {
	
	WorldController::close() ;
	
	currentAdapter.close() ;
	
}




