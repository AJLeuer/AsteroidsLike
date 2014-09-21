//
//  WindowOutput.h
//  World
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__WindowOutput__
#define __SpriteFight__WindowOutput__

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

#include "GraphicsData.hpp"
#include "TextOutput.hpp"
#include "GameColor.h"


#include "../World/GameState.hpp"
#include "../World/GameObject.h"

#include "../Control/Configuration.h"

using namespace std ;

/**
 * Controller class for all graphics output.
 */
class GraphicalOutput {
	
protected:
	
	static Window * window ;
	static Renderer * renderer ;
	static RenderInfo renderInfo ;
	
	/**
	 * Handles rendering.
	 */
	static void render() ;
	
	/**
	 * Renders the given texture at the desired position and size.
	 *
	 * @param texture The Texture to render
	 * @param size The desired size of the texture on the screen
	 * @param pos The onscreen coordinates representing where this texture should be rendered
	 */
	template<typename M, typename N>
	static void render(GraphicsData<M, N> * output) ;
	
	
	GraphicalOutput() ; //private to prevent instantiation
	
public:
	
	static void init() ;
	static void update() ;
	static void exit() ;
	
} ;


template<typename M, typename N>
void GraphicalOutput::render(GraphicsData<M, N> * output) {
	
	auto tempShape = convertToSDL_Rect(output->getPosition(), output->getSize()) ;
	
	int sdlrend_error = 0 ;
	
	/* texture will sometimes be null, e.g. when game objects are invisible their
	   getTexture() method returns a nullptr */
	if ((output != nullptr) && (output->isVisible())) {
		if (output->getTexture() != nullptr) {
			sdlrend_error = SDL_RenderCopyEx(renderer, output->getTexture(), NULL, &tempShape, output->getOrientation()->val_const(), NULL, SDL_FLIP_NONE) ;
		}
	}
	
	if (sdlrend_error == -1) {
		stringstream ss ;
		ss << "SDL_RenderCopy() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
}

#endif /* defined(__SpriteFight__WindowOutput__) */
