//
//  Size.h
//  GameWorld
//
//  Created by Adam James Leuer on 5/13/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Size__
#define __GameWorld__Size__

#include <iostream>
#include <sstream>

#include "Position.hpp"

template<typename N>
struct Size : public Position<N> {
	int w = this->Position<N>::x ;

	Size(N w_, N h_) ;
} ;

#endif /* defined(__GameWorld__Size__) */
