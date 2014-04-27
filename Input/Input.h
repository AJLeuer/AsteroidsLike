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
#include "../GameWorld/ForwardDecl.h"

using namespace std ;

/**
 * For registering member functions
 */
template<class T>
struct KeyInputRegister {
	
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	const char * requestedChar ;
	
	T * member_callOn ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable holds
	 * pointers to member functions
	 */
	void (T::*member_callBackFn)() ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable pointers
	 * to static or global functions (member_calledOn should be a nullptr in any
	 * KeyInputRegister where a callBackFn, and not a member_callBackFn, is held
	 */
	void (*callBackFn)() ;
	
	KeyInputRegister(const char* ch, T * callOn, void (T::*cb)()) :
		requestedChar(ch), member_callOn(callOn), member_callBackFn(cb) {}
	
	KeyInputRegister(const char* ch, void (*cb)()) :
		requestedChar(ch), member_callOn(nullptr), callBackFn(cb) {}
	
	
	void callBack() ;
	
} ;

template<class T>
void KeyInputRegister<T>::callBack() {
	if (member_callOn != nullptr) {
		(member_callOn->*member_callBackFn)();
	}
	else if (member_callOn == nullptr) {
		(*callBackFn)() ;
	}
}


template <class T>
class InputController {
	
protected:

	
	
	static vector<KeyInputRegister<T>> * keyInputRegistry ;
	
	static void listenForKeyEvents() ;
	
public:
	
	static void init() ;
	static void exec() ;
	static SDL_Scancode getScancodeFromChar(const char* c) { return SDL_GetScancodeFromName(c) ; } //just to remember how to get scancodes
	static void registerForKeypress(KeyInputRegister<T> & reg) ;
	static void exit() ;
	
	
} ;





template <class T>
vector<KeyInputRegister<T>> * InputController<T>::keyInputRegistry = nullptr ;

template <class T>
void InputController<T>::init() {
	keyInputRegistry = new vector<KeyInputRegister<T>> ;
}

template <class T>
void InputController<T>::registerForKeypress(KeyInputRegister<T> & reg) {
	keyInputRegistry->push_back(reg) ;
}

template <class T>
void InputController<T>::exec() {
	listenForKeyEvents() ;
}

template <class T>
void InputController<T>::listenForKeyEvents() {
	
	int i ;
	auto * keys = SDL_GetKeyboardState(&i) ; //only need to call once, stores pointer that stays valid for program duration
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		for	(unsigned i = 0 ; i < keyInputRegistry->size() ; i++) {
			SDL_PumpEvents() ;
			auto currScanCode = getScancodeFromChar(keyInputRegistry->at(i).requestedChar) ;
			if (keys[currScanCode] == 1) {
				
				keyInputRegistry->at(i).callBack() ;
			}
		}
	}
}


template <class T>
void InputController<T>::exit() {
	SDL_Quit() ;
}

#endif /* defined(__GameWorld__Input__) */
