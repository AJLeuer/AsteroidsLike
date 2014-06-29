//
//  Speed.hpp
//  SpriteFight
//
//  Created by Adam James Leuer on 6/29/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef SpriteFight_Speed_hpp
#define SpriteFight_Speed_hpp

#include <chrono>

template<typename N>
struct Speed {

#define Distance N
#define Timer std::chrono::nanoseconds
	
protected:
	Distance distance ;
	Timer totalTimeElapsed ;
};

#endif
