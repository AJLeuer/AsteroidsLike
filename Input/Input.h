//
//  Input.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/24/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Input__
#define __GameWorld__Input__

#include <iostream>
#include <thread>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "../GameWorld/GameInterface.h"

using namespace std ;

template<class T>
struct KeyInputRegister {
	
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	const char* unicodeSymbol ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. Will
	 * typically be called on members functions of some GameObject class
	 * or desc
	 */
	void (*callBack)(T * calledOn) ;
	
	KeyInputRegister(const char* ch, void (*ca)(T*)) :
		unicodeSymbol(ch), callBack(ca) {}
	
} ;

class Input {
	
protected:
	static SDL_Event * inputEvent ;
	static std::thread * keyEventsThread ;
	
	static list<KeyInputRegister<GameInterface>> * keyInputRegistry ;
	
	static void listenForKeyEvents() ;
	static void listenForKeyEvents_thread_start() ;
	
public:
	
	static void init() ;
	static void registerForKeypress(KeyInputRegister<GameInterface> & reg) ;
	static void close() ;
	
	
} ;

#endif /* defined(__GameWorld__Input__) */
