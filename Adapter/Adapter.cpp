//
//  Adapter.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Adapter.h"

void Adapter::init() {
	this->AdapterInterface::init() ;
	
	//local initializations:
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();  //this inits and returns a reference to a WINDOW*, which we don't need
	
	cbreak() ; //character-at-a-time  input  without buffering
	noecho() ; //wont print input back out
	
	curs_set(0) ; //sets cursor to invisible
}

void Adapter::show() {
	aiThread = new std::thread(&Adapter::show_threaded, std::move(this)) ;
}

void Adapter::show_threaded() {
	while (WorldController::isRunning()) {
		GameObject * temp  = nullptr ;
		for (auto i = 0 ; i < WorldController::gameObjects->size() ; i++) {
			WorldController::runningMtx.lock() ;
			if (WorldController::gameObjects == nullptr) { //we want to know if the pointer pointed to by WorldControllerObjects (i.e. WorldController::gameObjects) is null
				return ;
			}
			else {
				temp = WorldController::gameObjects->at(i) ;
				WorldController::runningMtx.unlock() ;
				*Debug::debugFile << "Current GameObject: " << endl << temp << endl ;
				Location trans = AdapterUtil::transLocation(*(temp->getLocation())) ;
				*Debug::debugFile << "This GameObject's translated location: " << trans.toString() << endl ;
				mvwaddstr(stdscr, trans.getY(), trans.getX(), temp->getIcon().c_str()) ;
				temp = nullptr ;
				wnoutrefresh(stdscr) ;
			}
			WorldController::runningMtx.unlock() ;
		}
		doupdate() ;
		usleep(90000) ;
		wclear(stdscr) ;
	}
}

void Adapter::operator()() {
	this->show() ;
}

void Adapter::close() {
	this->AdapterInterface::close() ;
	
	//local cleanup:
	endwin() ;
}