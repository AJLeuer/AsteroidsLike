//
//  GraphicalOutput.cpp
//  World
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
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	
	/* debug code - delete */
	auto winX = windowSizeX() ;
	auto winY = windowSizeY() ;
	/* end debug code */

	window = SDL_CreateWindow("T^2",
							  SDL_WINDOWPOS_CENTERED,   // x position, centered
							  SDL_WINDOWPOS_CENTERED,   // y position, centered
							  LOGICAL_WINDOW_SIZE_X,			// width, in logical (but not necessarily actual) pixels
							  LOGICAL_WINDOW_SIZE_Y,			// height, in logical (but not necessarily actual) pixels
							  WINDOW_ARGS) ;
	
	{
		
	/* debug code */
	stringstream ab ;
	ab << "Checking for SDL errors after SDL_CreateWindow(): " << SDL_GetError() << '\n' ;
	DebugOutput << ab.rdbuf() ;
	/* end debug code */
	}

	if (window == NULL) {
		stringstream ss ;
		ss << "Window creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}

	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) ;

	{
	/* debug code */
	stringstream ac ;
	ac << "Checking for SDL errors after SDL_CreateRenderer(): " << SDL_GetError() << '\n' ;
	DebugOutput << ac.rdbuf() ;
	/* end debug code */
	}
	
	if (renderer == NULL) {
		stringstream ss ;
		ss << "Renderer creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}

	GameState::initGraphics(window, renderer) ;
}


void GraphicalOutput::render() {
	for (auto i = 0 ; i < GameState::getGameObjects()->size() ; i++) {
		GameObject * temp = GameState::getGameObjects()->at(i) ;
		render(temp) ;
	}
}

void GraphicalOutput::render(const GameObject * object) {
	
	/* we will occasionally get null pointers */
	if (object != nullptr) {
		
		/* Don't render if invisible */
		if (object->isVisible()) {
			
			/* translate from world coordinates to screen coordinates */
			auto objWorldPosition(*object->getPosition()) ;
            
            /* important: don't forget to translate position from world coords to display coords! */
			
            auto objScreenPosition = translateToWindowCoords(objWorldPosition) ; // <-DEBUG: uncomment this
			//auto objScreenPosition = objWorldPosition ;   // <-DEBUG: remove this
			
			/* render to output */
			render(object->getTexture(), objScreenPosition, object->getSize()) ;
		}
	}
}

void GraphicalOutput::update() {
	SDL_RenderClear(renderer);
	render() ;
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












