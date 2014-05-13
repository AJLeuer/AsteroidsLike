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
#include <thread>
#include <chrono>

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
	
	/**
	 * Adds textures to the current renderer. Gets a list of current GameObjects from
	 * SharedGameData, then gets information from each game object to decide what texture
	 * to send to output.
	 */
	static void renderSprites() ;
	
	static void renderSprite(const Position<long> * pos, const float sizeModifier, SDL_Texture * texture) ;
	
	GraphicalOutput() ; //private to prevent instantiation
	
public:
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;

#endif /* defined(__GameWorld__WindowOutput__) */
