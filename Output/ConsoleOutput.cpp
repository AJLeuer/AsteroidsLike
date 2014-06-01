//
//  ConsoleOutput.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>
#include <thread>

#include "ConsoleOutput.hpp"

using namespace std ;

void ConsoleOutput::init() {
	
	//local initializations:
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();  //this inits and returns a reference to a WINDOW*, which we don't need
	
	cbreak() ; //character-at-a-time  input  without buffering
	
	noecho() ; //wont print input back out
	
	curs_set(0) ; //sets cursor to invisible
}


void ConsoleOutput::update(unsigned int waitForMicrosec) {
	doupdate() ;
	std::this_thread::sleep_for(chrono::microseconds(waitForMicrosec)) ;	//equivelent to usleep(waitForMicrosec) ;
	wclear(stdscr) ;
}

void ConsoleOutput::exit() {
	endwin() ;
}
