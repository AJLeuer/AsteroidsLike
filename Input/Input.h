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
#include <SDL2/SDL_video.h>

#include "../GameWorld/GameData.h"
#include "../GameWorld/ForwardDecl.h"

using namespace std ;

template<class T>
struct KeyInputRegister {
	
	/**
	 * The string representing the keyboard key
	 * the client wishes to listen for
	 */
	const char * requestedChar ;
	
	T * caller ;
	
	/**
	 * A pointer to the function to be called
	 * when the requested keyboard input is detected. Will
	 * typically be called on member functions of some GameObject class
	 */
	void (T::*callBack)() ;
	
	KeyInputRegister(const char* ch, T * callr, void (T::*cb)()) :
		requestedChar(ch), caller(callr), callBack(cb) {}
	
} ;

template <class T>
class InputController {
	
protected:
	static SDL_Event * event ;
	static struct SDL_Window * window ;
	static std::thread * keyEventsThread ;
	
	static vector<KeyInputRegister<T>> * keyInputRegistry ;
	
	static void listenForKeyEvents() ;
	
public:
	
	static void init() ;
	static void exec() ;
	static void registerForKeypress(KeyInputRegister<T> & reg) ;
	static void exit() ;
	
	
} ;

template <class T>
SDL_Event * InputController<T>::event = nullptr ;

template <class T>
SDL_Window * InputController<T>::window ;

template <class T>
thread * InputController<T>::keyEventsThread = nullptr ;

template <class T>
vector<KeyInputRegister<T>> * InputController<T>::keyInputRegistry = nullptr ;

template <class T>
void InputController<T>::init() {
	auto n = SDL_Init(SDL_INIT_EVERYTHING) ;
	
	InputController<GameObject>::window = SDL_CreateWindow("SDL 2 window",
														   SDL_WINDOWPOS_CENTERED,     // x position, centered
														   SDL_WINDOWPOS_CENTERED,     // y position, centered
														   640,                        // width, in pixels
														   480,                        // height, in pixels
														   SDL_WINDOW_OPENGL           // flags
														   );
	
	
	keyInputRegistry = new vector<KeyInputRegister<T>> ;
	
	event = new SDL_Event() ;
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
	
	while (*GLOBAL_CONTINUE_SIGNAL) {
		while (SDL_PollEvent(event)) {
			
			if (event->type == SDL_KEYDOWN) {
				cout << "keypress event detected!" << endl ;
				auto key = event->key.keysym.sym ;
				const char * ch = SDL_GetKeyName(key) ;
				
				for (auto i = 0 ; i < keyInputRegistry->size() ; i++) {
					if (ch == (keyInputRegistry->at(i).requestedChar)) {
						
						T * callr = keyInputRegistry->at(i).caller ;
						
						void (T::*callBak)() = (keyInputRegistry->at(i).callBack) ;
						
						(callr->*callBak)();
					
					}
				}
			}
		}
	}
}


template <class T>
void InputController<T>::exit() {
	SDL_Quit() ;
	keyEventsThread->join() ;
	delete keyEventsThread ;
	delete event ;
}

#endif /* defined(__GameWorld__Input__) */
