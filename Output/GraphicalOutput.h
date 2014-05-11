//
//  WindowOutput.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__WindowOutput__
#define __GameWorld__WindowOutput__

#include <unistd.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <SDL2_image/SDL_image.h>

#include "../Util/Debug.h"
#include "../Util/Position.hpp"

#include "../GameWorld/GameData.h"
#include "../GameWorld/GameObject.h"



class GraphicalOutput {
	
protected:
	
	static struct SDL_Window * window ;
	static SDL_Renderer * renderer ;
	
	static void addSprites(const Position<long> & pos, SDL_Surface * surface) ;
	
		
public:
	
	GraphicalOutput() ;
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;

#endif /* defined(__GameWorld__WindowOutput__) */
