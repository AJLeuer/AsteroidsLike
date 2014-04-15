//
//  TestAdapter.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TestAdapter.h"

void TestAdapter::init() {
	this->AdapterInterface::init() ;
	
	//local initializations:
}

void TestAdapter::show() {
	 aiThread = new std::thread(&TestAdapter::show_threaded, std::move(this)) ;
}

void TestAdapter::show_threaded() {
	while (WorldController::isRunning()) {
		GameObject * temp  = nullptr ;
		for (auto i = WorldController::gameObjects->begin() ; i != WorldController::gameObjects->end() ; i++) {
			WorldController::runningMtx.lock() ;
			if (WorldController::gameObjects == nullptr) {
				return ;
			}
			else {
				temp = (*i) ;
				*(Debug::debugOutput) << "Current GameObject: " ;
				*(Debug::debugOutput) << *temp ;
				Location<long> trans = AdapterUtil::transLocation(*(temp->getLocation())) ;
				*(Debug::debugOutput) << "Tranlated location: " ;
				*(Debug::debugOutput) << trans.toString() << endl ;
				*(Debug::debugOutput) << temp->getIcon().c_str() << endl << endl ;
				temp = nullptr ;
			}
			WorldController::runningMtx.unlock() ;
		}
	}
}

void TestAdapter::operator()() {
	this->show() ;
}

void TestAdapter::close() {
	this->AdapterInterface::close() ;
	
	//local cleanup:
	
}
