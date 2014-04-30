//
//  ConsoleOutput.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__ConsoleOutput__
#define __GameWorld__ConsoleOutput__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>

#include "../Util/Position.hpp"
#include "../GameWorld/GameData.h"

using namespace std ;


class ConsoleOutput {
	
private:
	
	
public:
	
	
	static void init() ;
	
	template<typename N>
	static void setOutput(const Position<N> pos_on_screen, const char * str) ;
	
	static void update(unsigned int wait_micros) ;
	
	static void exit() ;
			
} ;


template<typename N>
void ConsoleOutput::setOutput(const Position<N> pos_on_screen, const char * str) {
	const Position<long> trans = transPosition(pos_on_screen) ;
	mvwaddstr(stdscr, trans.getY(), trans.getX(), str) ;
	wnoutrefresh(stdscr) ;
}


#endif /* defined(__GameWorld__ConsoleOutput__) */
