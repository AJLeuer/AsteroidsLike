//
//  Input.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Input.hpp"


vector<KeyInputRegister *> * InputController::keyInputRegistry = new vector<KeyInputRegister *>() ;

const unsigned char * InputController::keys ;

int InputController::keyArraySize = 1 ; //initializing to 1 only because SDL_GetKeyboardState requires non-null parameter

void InputController::listenForKeyEvents() {
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


void InputController::init() {
	//keyInputRegistry is already initialized. add anything else here.
	SDL_InitSubSystem(SDL_INIT_EVENTS) ;
	keys = SDL_GetKeyboardState(&keyArraySize) ; //only need to call once, stores pointer that stays valid for program duration
}


void InputController::registerForKeypress(KeyInputRegister * reg) {
	keyInputRegistry->push_back(reg) ;
}


void InputController::update() {
	listenForKeyEvents() ;
}


void InputController::exit() {
	for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
		if (keyInputRegistry->at(i) != nullptr) {
			delete keyInputRegistry->at(i) ;
		}
	}
	delete keyInputRegistry ;
	SDL_QuitSubSystem(SDL_INIT_EVENTS) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}

