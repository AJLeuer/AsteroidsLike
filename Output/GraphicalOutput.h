//
//  WindowOutput.h
//  World
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
#include <SDL2_ttf/SDL_ttf.h>

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"

#include "OutputData.hpp"
#include "TextOutput.h"
#include "GameColor.h"


#include "../World/GameState.h"
#include "../World/GameObject.h"

using namespace std ;

/**
 * Controller class for all graphics output.
 */
class GraphicalOutput {
	
protected:
	
	static Window * window ;
	static Renderer * renderer ;

	
	/**
	 * Handles rendering.
	 */
	static void render() ;
	
	/** 
	 * A convenience function for rendering GameObjects.
	 * For more info, see render(Texture *, const Position<N> &, const Size<int> *)
	 *
	 * @param object The object to be rendered
	 */
	static void render(const GameObject * object) ;
	
	static void render(OutputData * output) ;
    
	/**
	 * Renders the given texture at the desired position and size.
	 *
	 * @param texture The Texture to render
	 * @param size The desired size of the texture on the screen
	 * @param pos The onscreen coordinates representing where this texture should be rendered
	 */
	template<typename N>
	static void render(Texture * texture, const Position<N> * pos, const Size<int> * size) ;
    
	
	GraphicalOutput() ; //private to prevent instantiation
	
public:
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;


template<typename N>
void GraphicalOutput::render(Texture * texture, const Position<N> * pos, const Size<int> * size) {
	
	auto scrnPos = translateToWindowCoords(*pos) ;
	
	auto tempShape = convertToSDL_Rect(scrnPos, size) ;
	
	int sdlrend_error = 0 ;
	
	/* texture will sometimes be null, e.g. when game objects are invisible their
	   getTexture() method returns a nullptr */
	if (texture != nullptr) {
		sdlrend_error = SDL_RenderCopy(renderer, texture, NULL, &tempShape) ;
	}
	
	/* Debug code */
	cerr << "Checking for SDL errors after render... " << SDL_GetError() << endl ;
	
	if (sdlrend_error == -1) {
		stringstream ss ;
		ss << "SDL_RenderCopy() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
}

#endif /* defined(__GameWorld__WindowOutput__) */
