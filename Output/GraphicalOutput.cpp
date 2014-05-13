//
//  GraphicalOutput.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//



#include "GraphicalOutput.h"


SDL_Window * GraphicalOutput::window = nullptr ;

SDL_Renderer * GraphicalOutput::renderer = nullptr ;


void GraphicalOutput::init() {
	auto n = SDL_Init(SDL_INIT_EVERYTHING) ;
	
	if (n != 0) {
		//Do something? or not...
	}
	
	GraphicalOutput::window = SDL_CreateWindow("SDL 2 window",
														   SDL_WINDOWPOS_CENTERED,     // x position, centered
														   SDL_WINDOWPOS_CENTERED,     // y position, centered
														   (int)GLOBAL_MAX_X,                        // width, in pixels
														   (int)GLOBAL_MAX_Y,                        // height, in pixels
														   (SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN) // flags
														   ) ;
	
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)) ;
	
	float x_ = 0 ;
	float y_ = 0 ;
	float * x = &x_ ;
	float * y = &y_ ;

	SDL_RenderGetScale(renderer, x, y) ;
	
	//SDL_RenderSetLogicalSize(renderer, 1024, 768);
	
	SDL_RenderGetScale(renderer, x, y) ;
	
	//init all GameObjects with textures and rectangles (rects hold size info)
	
	initGameObjects() ;
	renderSprites() ;
}

void GraphicalOutput::initGameObjects() {
	for (auto i = 0 ; i < GameObject::getAllGameObjects()->size() ; i++) {
		
		GameObject * temp = SharedGameData::getGameObjects()->at(i) ;
		
		//set texture
		Texture * texture = AssetFileIO::getTextureFromFilename(renderer, temp->getImageFile(), temp->getType()) ;
		temp->setTexture(texture) ;
		
		//set size
		Size * size = static_cast<SDL_Rect *>(malloc(sizeof(*size))) ;
		Uint32 * ignored1 = 0 ;
		int * ignored2 = 0 ;
		SDL_QueryTexture(texture, ignored1, ignored2, &(size->w), &(size->h)) ; //init local size with size of texture
		temp->setSize(size) ; //assign size to this GameObject
	}
}

void GraphicalOutput::renderSprites() {
	for (auto i = 0 ; i < SharedGameData::getGameObjects()->size() ; i++) {
		GameObject * temp = SharedGameData::getGameObjects()->at(i) ;
		renderSprite(temp) ;
	}
}

void GraphicalOutput::renderSprite(GameObject * gameObject) {
	SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, gameObject->getSize()) ;
}

void GraphicalOutput::update() {
	usleep(eight_milliseconds) ;
	SDL_RenderClear(renderer);
	renderSprites() ;
	SDL_RenderPresent(renderer) ;
}

void GraphicalOutput::exit() {
	//todo (assuming there's anything at all for us to do here)
}




/**
 * Other useful functions that may come in handy:
 
 SDL_RenderGetScale() ;
 
 SDL_RenderSetLogicalSize();
 
 SDL_GetRendererOutputSize(renderer, &(tempValueStorage.x), &(tempValueStorage.y))
 
 *
 */












