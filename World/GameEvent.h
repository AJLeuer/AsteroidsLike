//
//  GameEvent.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameEvent__
#define __GameWorld__GameEvent__

#include <dirent.h>
#include <iostream>

#include "GameInterface.h"

using namespace std ;

class GameEvent : public GameInterface {
	
private:
	
	static unsigned IDs ;
	
protected:
	
	unsigned ID ;
	string icon ;
	
  
public:
	
	/**
	 * Creates a new GameEvent
	 */
	GameEvent() ;
	
	/**
	 * Copy constructor for GameEvent. The new instance has its own unique ID.
	 *
	 * @param other The GameEvent to be copied
	 */
	GameEvent(const GameEvent & other) ;
	
	
	/**
	 * Creates an event with the given UTF-8 symbol (preferably just
	 * one character) as its icon
	 *
	 * @param symbol The icon to be used by this GameEvent
	 */
	GameEvent(string symbol) ;
	
	/**
	 * Destructor for GameEvent
	 */
	virtual ~GameEvent() ;
	
	/**
	 * Assignment operator overload for GameEvent. The object copied
	 * to will have its own unique ID.
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	GameEvent & operator=(const GameEvent & rhs) ;
	
	virtual void operator()() ;
	
	virtual void notify() ;
	
	string draw() ;
  
};

#endif /* defined(__GameWorld__GameEvent__) */
