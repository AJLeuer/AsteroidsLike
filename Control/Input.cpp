//
//  Input.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "Input.hpp"

using namespace std ;

void EventRegisterBase::callBack() {
	if ((memberToCallOn != nullptr) || (member_callBackFunction == nullptr)) { //if this is an instance member function (we'll know by checking that member_callOn isn't null,
																			   //then call it on that object
        (memberToCallOn->*member_callBackFunction)();
    }
    else if (memberToCallOn == nullptr) { //else this is a global or static function, just call it
        (*callBackFunction)() ;
    }
}


vector<EventRegister *> * InputController::eventListenerRegistry = new vector<EventRegister *>() ;
vector<KeyInputRegister *> * InputController::keyInputRegistry = new vector<KeyInputRegister *>() ;

Event * InputController::event = (Event *) malloc(sizeof(Event)) ;

const unsigned char * InputController::keys ;

int InputController::keyArraySize = 1 ; //initializing to 1 only because SDL_GetKeyboardState requires non-null parameter

void InputController::listenForEvents() {
	while ((GLOBAL_CONTINUE_SIGNAL == true) && (SDL_PollEvent(event))) {
		EventType currentEventType ;
		for (auto i = 0 ; i < eventListenerRegistry->size() ; i++) {
			currentEventType = eventListenerRegistry->at(i)->getEventType() ;
			if (event->type == currentEventType) {
				eventListenerRegistry->at(i)->callBack() ;
			}
			if (GLOBAL_CONTINUE_SIGNAL == false) {
				break ;
			}
		}
	}
}

void InputController::listenForKeypress() {
	
	const vector<string> * keysAssignedToCallback ;
	for	(unsigned i = 0 ; ((GLOBAL_CONTINUE_SIGNAL == true) && (i < keyInputRegistry->size())) ; i++) {
        
		SDL_PumpEvents() ;
		
		keysAssignedToCallback = keyInputRegistry->at(i)->getAllRequestedKeys() ;
        
        /* iterate through every key paired with the callback function at the current index, and
           if there's a match (the key was pressed) call it */
        for (auto j = 0 ; j < keysAssignedToCallback->size() ; j++) {
            auto currScanCode = getScancodeFromChar(keysAssignedToCallback->at(j).c_str()) ;
            if (keys[currScanCode] == 1) { // key is 1 if pressed
                keyInputRegistry->at(i)->callBack() ;
            }
			if (GLOBAL_CONTINUE_SIGNAL == false) {
				break ;
			}
        }
	}
}


void InputController::init() {
	//keyInputRegistry is already initialized. add anything else here.
	int sdlinit_error = SDL_InitSubSystem(SDL_INIT_EVENTS) ;

	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_InitSubSystem(SDL_INIT_EVENTS) failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	
	keys = SDL_GetKeyboardState(&keyArraySize) ; //only need to call once, stores pointer that stays valid for program duration
}

void InputController::registerForEvent(EventRegister *reg) {
	eventListenerRegistry->push_back(reg) ;
}

void InputController::registerForKeypress(KeyInputRegister * reg) {
	keyInputRegistry->push_back(reg) ;
}

void InputController::update() {
	listenForKeypress() ;
	listenForEvents() ; /* this will also call listenForKeypress()d when needed */
}


void InputController::exit() {
	for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
		if (keyInputRegistry->at(i) != nullptr) {
			delete keyInputRegistry->at(i) ;
		}
	}
	delete keyInputRegistry ;
	delete event ;
	SDL_QuitSubSystem(SDL_INIT_EVENTS) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}

