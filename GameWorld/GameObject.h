//
//  GameObject.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameObject__
#define __GameWorld__GameObject__

#include <iostream>

#include "BaseInterface.h"
#include "Message.h"

/*
 * The base class from which all other classes in the world (thought not the world
 * itself) will inherit
 */
class GameObject : public BaseInterface {
	
protected:
	
	

public:
	void notify() ;
	
	/**
	 * A GameObject or any other class can implement this function to pass messages to another.
	 */
	virtual void passMessage(Message *, GameObject & recipient) ;
	
	virtual void operator()() ;
	
	void operator()(GameObject & sentObject) ;
	
} ;

#endif /* defined(__GameWorld__GameObject__) */
