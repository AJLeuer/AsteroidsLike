//
//  GraphicalOutput.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "GraphicalOutput.h"


SDL_Window * GraphicalOutput::window = NULL ;

SDL_Renderer * GraphicalOutput::renderer = NULL ;


void GraphicalOutput::init() {
    
	int sdlinit_error = SDL_InitSubSystem(SDL_INIT_VIDEO) ;
	
	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_InitSubSystem(SDL_INIT_VIDEO) failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

	window = SDL_CreateWindow("SDL 2 window", SDL_WINDOWPOS_CENTERED,     // x position, centered
											  SDL_WINDOWPOS_CENTERED,     // y position, centered
											  (int)(GLOBAL_MAX_X),        // width, in pixels (/2 for highdpi)
											  (int)(GLOBAL_MAX_Y),        // height, in pixels (/2 for highdpi)
											  (SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)) ;
	{
	/* debug code */
	#ifdef DEBUG_MODE
	stringstream ab ;
	ab << "Checking for SDL errors after SDL_CreateWindow(): " << SDL_GetError() << '\n' ;
	DebugOutput << ab.rdbuf() ;
	#endif
	/* end debug code */
	}

	if (window == NULL) {
		stringstream ss ;
		ss << "Window creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) ;

	{
	/* debug code */
	#ifdef DEBUG_MODE
	stringstream ac ;
	ac << "Checking for SDL errors after SDL_CreateRenderer(): " << SDL_GetError() << '\n' ;
	DebugOutput << ac.rdbuf() ;
	#endif
	/* end debug code */
	}
	
	if (renderer == NULL) {
		stringstream ss ;
		ss << "Renderer creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

	SharedGameData::initGraphics(renderer) ;
}


void GraphicalOutput::renderTextures() {
	for (auto i = 0 ; i < SharedGameData::getGameObjects()->size() ; i++) {
		GameObject * temp = SharedGameData::getGameObjects()->at(i) ;
		renderObject(temp) ;
	}
}

void GraphicalOutput::renderObject(GameObject * gameObject) {
	auto tempShape = convertToSDL_Rect(*(gameObject->getPosition()), gameObject->getSize()) ;
	int sdlrend_error = SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, & tempShape) ;

	if (sdlrend_error == -1) {
		stringstream ss ;
		ss << "SDL_RenderCopy() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}
}

void GraphicalOutput::update() {
	//usleep(eight_milliseconds) ;
	this_thread::sleep_for(eight_milliseconds) ;
	SDL_RenderClear(renderer);
	renderTextures() ;
	SDL_RenderPresent(renderer) ;
}

void GraphicalOutput::exit() {
	//textures were destroyed already by WorldController::exit()
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}


/**
 * Other useful functions that may come in handy:
 
 SDL_RenderGetScale() ;
 
 SDL_RenderSetLogicalSize();
 
 SDL_GetRendererOutputSize(renderer, &(tempValueStorage.x), &(tempValueStorage.y))
 
 *
 */












