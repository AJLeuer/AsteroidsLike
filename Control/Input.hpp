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
#include <vector>


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_video.h>

#include "../GameWorld/GameData.h"
#include "../GameWorld/GameInterface.h"

using namespace std ;

/**
 * For assigning functions to keypresses
 */
struct KeyInputRegister {
	
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	string requestedKeyboardChar ;

	GameInterface * member_callOn ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable holds
	 * pointers to member functions
	 */
	void (GameInterface::*member_callBackFn)() ;

	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable points
	 * to static or global functions (member_callOn should be a nullptr in any
	 * KeyInputRegister where a callBackFn, and not a member_callBackFn, is held)
	 */
	void (*callBackFn)() ;
	
	KeyInputRegister() {} ;
	
	
	KeyInputRegister(const KeyInputRegister & other) :
		requestedKeyboardChar(other.requestedKeyboardChar),
		member_callOn(other.member_callOn), member_callBackFn(other.member_callBackFn) {} 
	
	KeyInputRegister(KeyInputRegister && other) :
		requestedKeyboardChar(std::move(other.requestedKeyboardChar)),
		member_callOn(other.member_callOn), member_callBackFn(std::move(other.member_callBackFn)) {}
	
	KeyInputRegister(const char * ch, GameInterface * callOn, void (GameInterface::*cb)()) :
		requestedKeyboardChar(ch), member_callOn(callOn), member_callBackFn(cb) {}
	
	KeyInputRegister(const char * ch, void (*cb)()) :
		requestedKeyboardChar(ch), member_callOn(nullptr), callBackFn(cb) {}
	
	~KeyInputRegister() {}
	
	
	inline void callBack() {
		if (member_callOn != nullptr) { //if this is an instance member function (we'll know by checking that member_callOn isn't null,
										//then call it on that object
			(member_callOn->*member_callBackFn)();
		}
		else if (member_callOn == nullptr) { //else this is a global or static function, just call it
			(*callBackFn)() ;
		}
	}
	
} ;




class InputController {
	
protected:
	
	/**
	 * Assigns keys to callback functions.
	 */
	static vector<KeyInputRegister *> * keyInputRegistry ;
	
	/**
	 * Holds pointers to the state of each key on the keyboard. Initialized once, but valid for the scope of the program.
	 */
	static const unsigned char * keys ;
	
	static int keyArraySize ;
	
	static void listenForKeyEvents() ;
	static SDL_Scancode getScancodeFromChar(const char* c) { return SDL_GetScancodeFromName(c) ; } //wrapper (just in case
																								//I forget how to get scancodes!)
	InputController() ;
	
public:
	
	
	static void registerForKeypress(KeyInputRegister * reg) ;
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;

#endif /* defined(__GameWorld__Input__) */
