//
//  TextOutput.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TextOutput.h"

using namespace std ;

TTF_Font * TextOutput::gameFont = nullptr ;


void TextOutput::init() {
	int sdlinit_error = TTF_Init() ;
	
	if (sdlinit_error == -1) {
		stringstream ss ;
		ss << "TTF_Init() failed. Outputting error:" << '\n' ;
		ss << TTF_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
    
    gameFont = TTF_OpenFont("/Assets/Fonts/Roboto-Regular.ttf", 18) ;
}

Size<unsigned> TextOutput::getSizeOfText(const string & str) {
    
    int w, h ;
    
    TTF_SizeUTF8(gameFont, str.c_str(), &w, &h) ;
    
    Size<unsigned> textSize(w, h) ;
    
    return textSize ;
}

void TextOutput::exit() {
    TTF_CloseFont(gameFont) ;
	TTF_Quit();
}


TextOutput::TextOutput(const string * text, const Position<float> * pos, const Size<int> * sz, GameColor * foreground, GameColor * background) :
	data(nullptr, nullptr, nullptr), foreground(foreground), background(background)
{
	Surface * surface = TTF_RenderUTF8_Shaded(gameFont, text->c_str(), foreground->convertToSDL_Color(), background->convertToSDL_Color()) ;
	
	texture = SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface) ;
	
	SDL_FreeSurface(surface) ;
	
	data = {texture, pos, sz} ;
	
	dataListReference = GameState::addlOutputStorage->insert(GameState::addlOutputStorage->begin(), &data) ;
}

void TextOutput::update() {
	data.texture = nullptr ;
	SDL_DestroyTexture(texture) ;
	
	Surface * surface = TTF_RenderUTF8_Shaded(gameFont, text->c_str(), foreground->convertToSDL_Color(), background->convertToSDL_Color()) ;
	texture = SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface) ;
	SDL_FreeSurface(surface) ;
	
	data.texture = texture ;
}





