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

RenderInfo GraphicalOutput::renderInfo ; // = (RenderInfo *)malloc(sizeof(RenderInfo)) ;

unsigned GraphicalOutput::framesRendered = 0 ;


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
	
	SDL_GetRendererInfo(renderer, &renderInfo) ;
	
	GameState::initGraphics(window, renderer) ;
}


void GraphicalOutput::render() {
	
	auto gData = GraphicsData<float, int>::getOutputData() ; //debug variable, delete this
	
	for (auto i = 0 ; i < GraphicsData<float, int>::getOutputData()->size() ; i++) {
        if (GraphicsData<float, int>::getOutputData()->at(i) != nullptr) {
            render(GraphicsData<float, int>::getOutputData()->at(i)) ;
        }
	}
	
	framesRendered++ ;
}

void GraphicalOutput::update() {
	GraphicsData<float, int>::updateAll() ;
	SDL_RenderClear(renderer) ;
	render() ;
	SDL_RenderPresent(renderer) ;
}

void GraphicalOutput::drawFPS() {
	
	static auto startTime = chrono::system_clock::now()  ;
	static chrono::seconds elapsedTime = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - startTime) ;// ;
	
	static unsigned framesLastCount = framesRendered ;
	static unsigned framesCurrentCount = framesRendered ;
	static unsigned frames = framesCurrentCount - framesLastCount ;
	
	auto updateFPS = [&]() -> string {
		
		auto & sT = startTime ; //debug variables, can delete
		auto & elapsed = elapsedTime ;
		unsigned & fs = frames ;
		unsigned & curCount = framesCurrentCount ;
		unsigned & lastCt = framesLastCount ;
		
		framesCurrentCount = framesRendered ;
		frames = framesCurrentCount - framesLastCount ;//get the frames rendered since the last time we ran
		framesLastCount = framesCurrentCount ;
		
		elapsedTime = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - startTime) ; //and the time elapsed
		startTime = chrono::system_clock::now() ;
		
		static float FPS = 0 ;
		
		if (frames > 0) {
			FPS = (float)frames / (float)elapsedTime.count() ; //no divide by zero errors here!
		}
		//else don't update FPS, just display it's most recent value
		
		stringstream fpsText ;
		
		fpsText << "FPS: " << FPS ;
		
		return fpsText.str() ;
		
	} ;
	
	TextOutput<float, int>::displayContinuousText(updateFPS, chrono::seconds(1), {300, 15}, Angle(0), GameColor(55, 255, 0, 0), GameColor(50, 40, 43, 0)) ;
}

void GraphicalOutput::exit() {
	
	for (auto i = 0 ; i > texturesToDestroy.size() ; i++) {
		SDL_DestroyTexture(texturesToDestroy.at(i)) ;
	}
	
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












