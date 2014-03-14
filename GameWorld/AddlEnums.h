//
//  AddlEnums.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/14/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__AddlEnums__
#define __GameWorld__AddlEnums__

#include <iostream>

enum class GameOutputState {
	
	/**
	 * Game is output to std::cout (std output) in realtime
	 */
	realtimeStdIO = 0,
	
	/**
	 * Game is recorded to an output stream
	 */
	outputStream = 1
	
};

#endif /* defined(__GameWorld__AddlEnums__) */
