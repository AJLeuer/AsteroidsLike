//
//  GraphicalOutput.cpp
//  World
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "GraphicalOutput.h"

using namespace std ;


Window * GraphicalOutput::window = NULL ;

Renderer * GraphicalOutput::renderer = NULL ;


void GraphicalOutput::init() {
    
	int sdlinit_error = SDL_InitSubSystem(SDL_INIT_VIDEO) ;
	
	
	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_InitSubSystem(SDL_INIT_VIDEO) failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	
	sdlinit_error = IMG_Init(IMG_INIT_PNG|IMG_INIT_TIF|IMG_INIT_JPG) ;
	
	if (sdlinit_error == 0) {
		stringstream ss ;
		ss << "IMG_Init() failed. Outputting error:" << '\n' ;
		ss << IMG_GetError() << '\n' ;
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
							  RESOLUTION_X_BASE_VALUE,			// width, in logical (but not necessarily actual) pixels
							  RESOLUTION_Y_BASE_VALUE,			// height, in logical (but not necessarily actual) pixels
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
	for (auto i = 0 ; i < OutputData<float, int>::getOutputData()->size() ; i++) {
		auto outputData = OutputData<float, int>::getOutputData() ; //debug variable, delete this
		render(OutputData<float, int>::getOutputData()->at(i)) ;
	}
}



void GraphicalOutput::update() {
	SDL_RenderClear(renderer) ;
	render() ;
	SDL_RenderPresent(renderer) ;
}

void GraphicalOutput::exit() {
    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    
    IMG_Quit() ;
    
	SDL_QuitSubSystem(SDL_INIT_VIDEO) ; /* call SDL_QuitSubSystem() for each subsystem we initialized */
}


/**
 * Other useful functions that may come in handy:
 
 SDL_RenderGetScale() ;
 
 SDL_RenderSetLogicalSize();
 
 SDL_GetRendererOutputSize(renderer, &(tempValueStorage.x), &(tempValueStorage.y))
 
 *
 */












