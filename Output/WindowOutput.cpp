//
//  WindowOutput.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/26/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "WindowOutput.h"


SDL_Window * WindowOutput::window = nullptr ;
SDL_Renderer * WindowOutput::renderer = nullptr ;
SDL_Surface * WindowOutput::surface = nullptr ;
SDL_Texture * WindowOutput::texture = nullptr ;

void WindowOutput::init() {
	auto n = SDL_Init(SDL_INIT_EVERYTHING) ;
	WindowOutput::window = SDL_CreateWindow("SDL 2 window",
														   SDL_WINDOWPOS_CENTERED,     // x position, centered
														   SDL_WINDOWPOS_CENTERED,     // y position, centered
														   640,                        // width, in pixels
														   480,                        // height, in pixels
														   SDL_WINDOW_OPENGL           // flags
														   ) ;
	
	
	SDL_RWops * bitmapFile = SDL_RWFromFile("Pixel Art.png", "r") ;
	surface = SDL_LoadBMP_RW(bitmapFile, 1) ;
	
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)) ;
	texture = SDL_CreateTextureFromSurface(renderer, surface) ;
	SDL_SetRenderTarget(renderer, texture) ;
	SDL_RenderPresent(renderer) ;
	usleep(1e8) ;
	

}