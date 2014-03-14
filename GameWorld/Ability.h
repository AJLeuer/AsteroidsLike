//
//  Ability.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Ability__
#define __GameWorld__Ability__

#include <iostream>

#include "GameEvent.h"

/**
 * A generic base class that will usually serve as the return type
 * for any complex action that a character can perform, such as using magic attacks
 * or healing
 */
class Ability : public GameEvent {
	

protected:
  

public:
	virtual void notify() ;
	virtual void operator()() ;
  
};

#endif /* defined(__GameWorld__Ability__) */
