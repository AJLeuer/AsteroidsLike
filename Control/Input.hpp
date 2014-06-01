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

typedef SDL_Event Event ;
typedef SDL_EventType EventType ;

class EventRegisterBase {
	
protected:
	
	/**
	 * See member_callBackFunction.
	 */
	GameInterface * memberToCallOn ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable holds
	 * pointers to member functions
	 */
	void (GameInterface::*member_callBackFunction)() ;
	
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. This variable points
	 * to static or global functions (member_callOn should be a nullptr in any
	 * EventRegisterBase where a callBackFn, and not a member_callBackFn, is held)
	 */
	void (*callBackFunction)() ;
	
	EventRegisterBase() {}
	
	/**
	 * Copy constructor
	 */
	EventRegisterBase(const EventRegisterBase & other) :
		memberToCallOn(other.memberToCallOn),
		member_callBackFunction(other.member_callBackFunction),
		callBackFunction(other.callBackFunction) {}
	
	/**
	 * Move constructor
	 */
	EventRegisterBase(EventRegisterBase && other) :
		memberToCallOn(other.memberToCallOn),
		member_callBackFunction(std::move(other.member_callBackFunction)),
		callBackFunction(std::move(other.callBackFunction)) {}
	
	EventRegisterBase(GameInterface * callOn, void (GameInterface::*cb)()) :
		memberToCallOn(callOn),
		member_callBackFunction(cb),
		callBackFunction(nullptr) {}
	
	EventRegisterBase(void (*cb)()) :
		memberToCallOn(nullptr),
		member_callBackFunction(nullptr),
		callBackFunction(cb) {}
	
public:
	
	void callBack() ;
	
} ;

/**
 * An implementation of EventRegisterBase with the ability to handle any type of SDL_Event.
 */
class EventRegister : public EventRegisterBase {
	
private:
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	EventType eventType ;
	

public:
	
	EventRegister() {}
	
	EventRegister(const EventRegister & other) :
		EventRegisterBase(other),
		eventType(other.eventType) {}
	
	EventRegister(EventRegister && other) :
		EventRegisterBase(std::move(other)),
		eventType(other.eventType) {}
    
	EventRegister(GameInterface * callOn, void (GameInterface::*cb)(), EventType eventType) :
		EventRegisterBase(callOn, cb),
		eventType(eventType) {}
	
	EventRegister(void (*cb)(), EventType eventType) :
		EventRegisterBase(cb),
		eventType(eventType) {}
	
	~EventRegister() {}
	
	EventType getEventType() { return this->eventType ; }
	
} ;

/**
 * An implementation of EventRegisterBase specialized for handling keyboard input.
 */
class KeyInputRegister : public EventRegisterBase {
	
private:
	
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	vector<string> requestedKeyboardChars ;
	
	
public:
	
	KeyInputRegister() {} ;
    
	KeyInputRegister(const KeyInputRegister & other) :
		EventRegisterBase(other),
		requestedKeyboardChars(vector<string>(other.requestedKeyboardChars)) {}
	
	KeyInputRegister(KeyInputRegister && other) :
		EventRegisterBase(std::move(other)),
		requestedKeyboardChars(std::move(other.requestedKeyboardChars)) {}
    
    KeyInputRegister(GameInterface * callOn, void (GameInterface::*cb)(), vector<string> keyChars) :
		EventRegisterBase(callOn, cb),
		requestedKeyboardChars(keyChars) {}
	
	KeyInputRegister(void (*cb)(), vector<string> keyChars) :
		EventRegisterBase(cb),
		requestedKeyboardChars(keyChars) {}
	
	KeyInputRegister(const char * ch, GameInterface * callOn, void (GameInterface::*cb)()) :
		EventRegisterBase(callOn, cb),
		requestedKeyboardChars(vector<string>{ch}) {}
	
	KeyInputRegister(const char * ch, void (*cb)()) :
		EventRegisterBase(cb),
		requestedKeyboardChars(vector<string>{ch}) {}
	
	~KeyInputRegister() {}
    
    const vector<string> * getAllRequestedKeys() { return &requestedKeyboardChars ; }
	
} ;



class InputController {
	
protected:
	
	/**
	 * Associates keys with callback functions.
	 */
	static vector<EventRegister *> * eventListenerRegistry ;
	
	/**
	 * Associates keys with callback functions.
	 */
	static vector<KeyInputRegister *> * keyInputRegistry ;
	
	static Event * event ;
	
	/**
	 * Holds pointers to the state of each key on the keyboard. Initialized once, but valid for the scope of the program.
	 */
	static const unsigned char * keys ;
	static int keyArraySize ;
	
	static SDL_Scancode getScancodeFromChar(const char* c) { return SDL_GetScancodeFromName(c) ; } //wrapper (just in case
																								   //I forget how to get scancodes!)
	
	static void listenForKeypress() ;
	
	static void listenForEvents() ;
	
	InputController() ;
	
public:
	
	static void registerForEvent(EventRegister * reg) ;
	static void registerForKeypress(KeyInputRegister * reg) ;
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;

#endif /* defined(__GameWorld__Input__) */
