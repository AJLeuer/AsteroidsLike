//
//  WindowOutput.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__WindowOutput__
#define __GameWorld__WindowOutput__

#include <iostream>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "../Util/Position.hpp"

#endif /* defined(__GameWorld__WindowOutput__) */

class WindowOutput {
protected:
	static struct SDL_Window * window ;
	static SDL_Renderer * renderer ;
	static SDL_Surface * surface ;
	static SDL_Texture * texture ;
	
public:
	static void init() ;
	
	
} ;

