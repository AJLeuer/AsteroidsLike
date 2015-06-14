//
//  Input.cpp
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Input.hpp"

using namespace std ;

void EventRegisterBase::callBack() {
	if ((memberToCallOn != nullptr) && (member_callBackFunction != nullptr)) { /* if this is an instance member function (we'll know by checking that member_callOn isn't null) then call it on that object) */
        (memberToCallOn->*member_callBackFunction)();
    }
    else { //if (memberToCallOn == nullptr) else this is a global or static function, just call it
		callBackFunction() ;
    }
}

void EventRegister::handleEvent(const Event * currentEvent) {
	if (currentEvent->type == eventType) {
		callBack() ;
	}
}

bool KeyInputRegister::checkForPressedKeys(const unsigned char * keyboardState, vector<ScanCode>::iterator scanCode) {
	if (*scanCode == scanCodes.back()) {
		if (keyboardState[*scanCode]) {
			return true ;
		}
		else {
			return false ;
		}
	}
	else {
		auto current = scanCode ;
		auto next = scanCode + 1 ;
		if (requestedKeyEvalMethod == KeypressEvaluationMethod::exactlyOne) {
			stringstream ss ;
			ss << "Error: KeyInputRegistry with KeypressEvaluationMethod::exactlyOne has more than one keypress registered" << '\n' ;
			ss << "Set a breakpoint in checkForPressedKeys() to debug" << '\n' ;
			DebugOutput << ss.rdbuf() ;
			return false ;
		}
		else if (requestedKeyEvalMethod == KeypressEvaluationMethod::all) {
			return ((keyboardState[*current]) && (checkForPressedKeys(keyboardState, next))) ;
		}
		else { // if (requestedKeyEvalMethod == KeypressEvaluationMethod::any)
			return ((keyboardState[*current]) || (checkForPressedKeys(keyboardState, next))) ;
		}
		
	}
}


void KeyInputRegister::handleKeyboardInput(const unsigned char * keyboardState) {
	auto scanCode = scanCodes.begin() ;
	if (checkForPressedKeys(keyboardState, scanCode)) {
		callBack() ;
	}
}

vector<EventRegister *> * InputControl::eventListenerRegistry = new vector<EventRegister *>() ;
vector<KeyInputRegister *> * InputControl::keyInputRegistry = new vector<KeyInputRegister *>() ;

Event * InputControl::event = (Event *) malloc(sizeof(Event)) ;

const unsigned char * InputControl::keys ;

int InputControl::keyArraySize = 1 ; //initializing to 1 only because SDL_GetKeyboardState requires non-null parameter


void InputControl::listenForEvents() {
	while ((SDL_PollEvent(event)) && (GLOBAL_CONTINUE_FLAG == true)) {
		for (auto i = 0 ; ((i < eventListenerRegistry->size()) && (GLOBAL_CONTINUE_FLAG == true)) ; i++) {
			if (eventListenerRegistry->at(i) != nullptr) {
				eventListenerRegistry->at(i)->handleEvent(event) ;
			}
		}
	}
}


void InputControl::listenForKeypress() {
	for	(unsigned i = 0 ; ((GLOBAL_CONTINUE_FLAG == true) && (i < keyInputRegistry->size())) ; i++) {
		if (keyInputRegistry->at(i) != nullptr) {
			SDL_PumpEvents() ;
			keyInputRegistry->at(i)->handleKeyboardInput(keys) ;
		}
	}
}


void InputControl::init() {
	//keyInputRegistry is already initialized. add anything else here.
	
	/* SDL automatically inits events */
	int sdlinit_error = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER|SDL_INIT_JOYSTICK) ;

	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER|SDL_INIT_JOYSTICK) failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	
	keys = SDL_GetKeyboardState(&keyArraySize) ; //only need to call once, stores pointer that stays valid for program duration
}

void InputControl::registerForEvent(EventRegister *reg) {
	eventListenerRegistry->push_back(reg) ;
}

void InputControl::registerForKeypress(KeyInputRegister * reg) {
	keyInputRegistry->push_back(reg) ;
}

/*
 * Deregisters any KeyInputRegistries and EventRegistries registered with the object registeredObject
 * as the memberToCallOn
 *
 * @param registeredObject A pointer to the object whose KeyInputRegister(s) and EventRegistries should be deleted
 */
void InputControl::deregister(GameInterface * registeredObject) {
	for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
		if (keyInputRegistry->at(i)->memberToCallOn == registeredObject) {
			delete keyInputRegistry->at(i) ;
			keyInputRegistry->at(i) = nullptr ;
		}
	}
	for (auto i = 0 ; i < eventListenerRegistry->size() ; i++) {
		if (eventListenerRegistry->at(i)->memberToCallOn == registeredObject) {
			delete eventListenerRegistry->at(i) ;
			eventListenerRegistry->at(i) = nullptr ;
		}
	}
}

void InputControl::update() {
	listenForKeypress() ;
	listenForEvents() ;
}


void InputControl::exit() {
	for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
		if (keyInputRegistry->at(i) != nullptr) {
			delete keyInputRegistry->at(i) ;
		}
	}
	delete keyInputRegistry ;
	delete event ;
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER|SDL_INIT_JOYSTICK) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}

