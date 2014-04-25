//
//  Input.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/24/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Input.h"

using namespace std ;

SDL_Event * Input::inputEvent = nullptr ;

thread * Input::keyEventsThread = nullptr ;

list<KeyInputRegister<GameInterface>> * Input::keyInputRegistry = nullptr ;

void Input::init() {

	keyInputRegistry = new list<KeyInputRegister<GameInterface>> ;
	
	listenForKeyEvents_thread_start() ;
	
}

void Input::registerForKeypress(KeyInputRegister<GameInterface> & reg) {
	keyInputRegistry->push_back(reg) ;
}

void Input::listenForKeyEvents() {
	
	//SDL_GetKeyName(SDL_Keycode key) ;
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		if (SDL_PollEvent(inputEvent) == 1) {
			if (inputEvent->type == SDL_KEYDOWN) {
				
				auto ch = inputEvent->key.keysym.sym ;
				
			}
		}
	}
}

void Input::listenForKeyEvents_thread_start() {
	void (*listenPtr)() = Input::listenForKeyEvents ;
	Input::keyEventsThread = new std::thread(listenPtr) ;
}

void Input::close() {
	keyEventsThread->join() ;
	delete keyEventsThread ;
	delete inputEvent ;
}
