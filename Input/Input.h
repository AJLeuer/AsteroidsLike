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
	string requestedKeyboardChar ;
	
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
	
	KeyInputRegister() {} ;
	
	
	KeyInputRegister(const KeyInputRegister<T> & other) :
		requestedKeyboardChar(other.requestedKeyboardChar),
		member_callOn(other.member_callOn), member_callBackFn(other.member_callBackFn) {} 
	
	KeyInputRegister(KeyInputRegister<T> && other) :
		requestedKeyboardChar(std::move(other.requestedKeyboardChar)),
		member_callOn(other.member_callOn), member_callBackFn(std::move(other.member_callBackFn)) {}
	
	KeyInputRegister(const char* ch, T * callOn, void (T::*cb)()) :
		requestedKeyboardChar(ch), member_callOn(callOn), member_callBackFn(cb) {}
	
	KeyInputRegister(const char* ch, void (*cb)()) :
		requestedKeyboardChar(ch), member_callOn(nullptr), callBackFn(cb) {}
	
	~KeyInputRegister() {}
	
	
	void callBack() ;
	
} ;

template<class T>
void KeyInputRegister<T>::callBack() {

	if (member_callOn != nullptr) { //if this is an instance member function (we'll know by checking that member_callOn isn't null,
									//then call it on that object
		(member_callOn->*member_callBackFn)();
	}
	else if (member_callOn == nullptr) { //else this is a global or static function, just call it
		(*callBackFn)() ;
	}
}


template <class T>
class InputController {
	
protected:
	
	/**
	 * Assigns keys to callback functions.
	 */
	static vector<KeyInputRegister<T> *> * keyInputRegistry ;
	
	/**
	 * Holds pointers to the state of each key on the keyboard. Initialized once, but valid for the scope of the program.
	 */
	static const Uint8 * keys ;
	
	static int keyArraySize ;
	
	static void listenForKeyEvents() ;
	static SDL_Scancode getScancodeFromChar(const char* c) { return SDL_GetScancodeFromName(c) ; } //wrapper (just in case
																								//I forget how to get scancodes!)
	InputController() ;
	
public:
	
	
	static void registerForKeypress(KeyInputRegister<T> * reg) ;
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;


template <class T>
vector<KeyInputRegister<T> *> * InputController<T>::keyInputRegistry = new vector<KeyInputRegister<T> *>() ;

template <class T>
const Uint8 * InputController<T>::keys ;

template <class T>
int InputController<T>::keyArraySize = 1 ; //initializing to 1 only because SDL_GetKeyboardState requires non-null parameter

template <class T>
void InputController<T>::listenForKeyEvents() {
	const char * keyAssignedToCallback ;
	for	(unsigned i = 0 ; ((i < keyInputRegistry->size()) && (GLOBAL_CONTINUE_SIGNAL == true)) ; i++) {
		SDL_PumpEvents() ; //calling this to update state of keys
		keyAssignedToCallback = keyInputRegistry->at(i)->requestedKeyboardChar.c_str() ;
		auto currScanCode = getScancodeFromChar(keyAssignedToCallback) ;
		if (keys[currScanCode] == 1) { // key is 1 if pressed
			keyInputRegistry->at(i)->callBack() ;
		}
		if (GLOBAL_CONTINUE_SIGNAL == false) {
			break ;
		}
	}
}

template <class T>
void InputController<T>::init() {
	//keyInputRegistry is already initialized. add anything else here.
	SDL_InitSubSystem(SDL_INIT_EVENTS) ;
	keys = SDL_GetKeyboardState(&keyArraySize) ; //only need to call once, stores pointer that stays valid for program duration
}

template <class T>
void InputController<T>::registerForKeypress(KeyInputRegister<T> * reg) {
	keyInputRegistry->push_back(reg) ;
}

template <class T>
void InputController<T>::update() {
	listenForKeyEvents() ;
}

template <class T>
void InputController<T>::exit() {
	for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
		if (keyInputRegistry->at(i) != nullptr) {
			delete keyInputRegistry->at(i) ;
		}
	}
	delete keyInputRegistry ;
	SDL_QuitSubSystem(SDL_INIT_EVENTS) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}

typedef InputController<GameObject> MainInputController ;

#endif /* defined(__GameWorld__Input__) */
