//
//  Input.h
//  World
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

#include "../World/GameState.h"
#include "../World/GameInterface.h"

#include "../Control/Configuration.h"

using namespace std ;

typedef SDL_Event Event ;
typedef SDL_EventType EventType ;
typedef SDL_Keycode KeyCode ;
typedef SDL_Scancode ScanCode ;

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
	
	void handleEvent(const Event * currentEvent) ;
	
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
	string requestedKeyboardChar ;
	
	KeyCode requestedKeyCode ;
	
	/**
	 * This enumeration represents whether this KeyInputRegister references
	 */
	enum class KeyIdenMethod {
		keyChar = 0,
		keyCode = 1,
		both = (keyChar & keyCode)
	} keyIDMethod ;
	
public:
	
	KeyInputRegister() {} ;
    
	KeyInputRegister(const KeyInputRegister & other) :
		EventRegisterBase(other),
		requestedKeyboardChar(string(other.requestedKeyboardChar)),
		keyIDMethod(other.keyIDMethod) {}
	
	KeyInputRegister(KeyInputRegister && other) :
		EventRegisterBase(std::move(other)),
		requestedKeyboardChar(std::move(other.requestedKeyboardChar)),
		keyIDMethod(std::move(other.keyIDMethod)) {}
    
    KeyInputRegister(GameInterface * callOn, void (GameInterface::*cb)(), string keyChar) :
		EventRegisterBase(callOn, cb),
		requestedKeyboardChar(keyChar),
		keyIDMethod(KeyIdenMethod::keyChar) {}
	
	KeyInputRegister(void (*cb)(), string keyChar) :
		EventRegisterBase(cb),
		requestedKeyboardChar(keyChar),
		keyIDMethod(KeyIdenMethod::keyChar) {}
	
	KeyInputRegister(GameInterface * callOn, void (GameInterface::*cb)(), KeyCode keyCode) :
		EventRegisterBase(callOn, cb),
		requestedKeyCode(keyCode),
		keyIDMethod(KeyIdenMethod::keyCode) {}
	
	KeyInputRegister(void (*cb)(), KeyCode keyCode) :
		EventRegisterBase(cb),
		requestedKeyCode(keyCode),
		keyIDMethod(KeyIdenMethod::keyCode) {}
	
	KeyInputRegister(GameInterface * callOn, void (GameInterface::*cb)(), string keyChar, KeyCode keyCode) :
		EventRegisterBase(callOn, cb),
		requestedKeyboardChar(keyChar),
		requestedKeyCode(keyCode),
		keyIDMethod(KeyIdenMethod::both) {}
	
	KeyInputRegister(void (*cb)(), string keyChar, KeyCode keyCode) :
		EventRegisterBase(cb),
		requestedKeyboardChar(keyChar),
		requestedKeyCode(keyCode),
		keyIDMethod(KeyIdenMethod::both) {}
	
	~KeyInputRegister() {}
    
    void handleKeyboardInput(const unsigned char * keyboardState) ;
	
	static ScanCode getScanCodeFor(const char* c) { return SDL_GetScancodeFromName(c) ; } /* wrapper (just in case
																							   I forget how to get scancodes!) */
	static ScanCode getScanCodeFor(KeyCode code) { return SDL_GetScancodeFromKey(code) ; }
	
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
