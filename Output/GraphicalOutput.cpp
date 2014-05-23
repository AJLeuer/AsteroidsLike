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
	
	int sdlinit_error = SDL_Init(SDL_INIT_EVERYTHING) ;

	/* debug code */
	stringstream aa ;
	aa << "Checking for SDL errors after SDL_Init(): " << SDL_GetError() << '\n' ;
	DebugOutput << aa.rdbuf() ;
	/* end debug code */

	if (sdlinit_error != 0) {
		stringstream ss ;
		ss << "SDL_Init() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

	window = SDL_CreateWindow("SDL 2 window", SDL_WINDOWPOS_CENTERED,     // x position, centered
											  SDL_WINDOWPOS_CENTERED,     // y position, centered
											  (int)GLOBAL_MAX_X,                        // width, in pixels
											  (int)GLOBAL_MAX_Y,                        // height, in pixels
											  (/*SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL |*/ SDL_WINDOW_SHOWN)) ;

	/* debug code */
	stringstream ab ;
	ab << "Checking for SDL errors after SDL_CreateWindow(): " << SDL_GetError() << '\n' ;
	DebugOutput << ab.rdbuf() ;
	/* end debug code */

	if (window == 0) {
		stringstream ss ;
		ss << "Window creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}

	int num = SDL_GetNumRenderDrivers() ;
	num = num ;


	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_TARGETTEXTURE)*/)) ;

	/* debug code */
	stringstream ac ;
	ac << "Checking for SDL errors after SDL_CreateRenderer(): " << SDL_GetError() << '\n' ;
	DebugOutput << ac.rdbuf() ;
	/* end debug code */
	
	if (renderer == NULL) {
		stringstream ss ;
		ss << "Renderer creation failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	
	initGameObjects() ;
	renderTextures() ;
}

void GraphicalOutput::initGameObjects() {
	for (auto i = 0 ; i < GameObject::getAllGameObjects()->size() ; i++) {
		
		GameObject * temp = SharedGameData::getGameObjects()->at(i) ;
		
		//set texture
		SDL_Texture * tex = nullptr ;

		tex = AssetFileIO::getTextureFromFilename(renderer, temp->getImageFile(), temp->getType()) ;

		if (tex == nullptr) {
			stringstream ss ;
			ss << "Load texture failed." << '\n' ;
			ss << SDL_GetError() << '\n' ;
			DebugOutput << ss.rdbuf() ;
			throw exception() ;
		}

		temp->setTexture(tex) ;

		//set size
		Size * size = static_cast<SDL_Rect *>(malloc(sizeof(*size))) ;
		Uint32 * ignored1 = 0 ;
		int * ignored2 = 0 ;

		SDL_QueryTexture(tex, ignored1, ignored2, &(size->w), &(size->h)) ; //init local size with size of texture
		size->x = (int) temp->getPosition()->x ;
		size->y = (int) temp->getPosition()->y ;
		temp->setSize(size) ; //assign size to this GameObject
	}
}

void GraphicalOutput::renderTextures() {
	for (auto i = 0 ; i < SharedGameData::getGameObjects()->size() ; i++) {
		GameObject * temp = SharedGameData::getGameObjects()->at(i) ;
		renderTexture(temp) ;
	}
}

void GraphicalOutput::renderTexture(GameObject * gameObject) {
	/* temp debug code, remove this -> */ int sdlrend_error = SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, gameObject->getSize()) ;
	/* uncomment this-> */ //int sdlrend_error = SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, gameObject->getSize()) ;

	/* debug code */
	stringstream ab ;
	ab << "Checking for SDL errors after SDL_RenderCopy(): " << SDL_GetError() << '\n' ;
	DebugOutput << ab.rdbuf() ;
	/* end debug code */

	if (sdlrend_error == -1) {
		stringstream ss ;
		ss << "SDL_RenderCopy() failed." << '\n' ;
		ss << SDL_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		throw exception() ;
	}
}

void GraphicalOutput::update() {
	usleep(eight_milliseconds) ;
	SDL_RenderClear(renderer);
	renderTextures() ;
	SDL_RenderPresent(renderer) ;
}

void GraphicalOutput::exit() {
	//textures were destroyed already by WorldController::exit()
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}




/**
 * Other useful functions that may come in handy:
 
 SDL_RenderGetScale() ;
 
 SDL_RenderSetLogicalSize();
 
 SDL_GetRendererOutputSize(renderer, &(tempValueStorage.x), &(tempValueStorage.y))
 
 *
 */












