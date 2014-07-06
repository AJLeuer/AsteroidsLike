//
//  TextOutput.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TextOutput.h"

using namespace std ;

vector<TextOutput *> TextOutput::allTextOutput ; //= vector<TextOutput *>() ;

TTF_Font * TextOutput::gameFont = nullptr ;

bool TextOutput::hasNulls() {
	if (text == nullptr) {
		return true ;
	}
	if ((data.position == nullptr) || (data.size == nullptr)) {
		return true ;
	}
	if ((foreground == nullptr) || (background == nullptr)) {
		return true ;
	}
	else {
		return false ;
	}
}

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
	text(text), data(), foreground(foreground), background(background), texture(nullptr)
{
	data.setAll(texture, pos, sz) ;
	
	allTextOutput.push_back(this) ;
}

//must be called from main thread
void TextOutput::update() {
	
	data.texture = nullptr ;
	
	if (texture != nullptr) {
		//SDL_DestroyTexture(texture) ;
	}
	
	if (hasNulls()) {
		delete this ;
	}
	
	else {
		
		const char * cstr = text->c_str() ;
		//strcpy(cstr, text->c_str()) ;
		
		Surface * surface = TTF_RenderUTF8_Shaded(gameFont, cstr, foreground->convertToSDL_Color(), background->convertToSDL_Color()) ;
		
		/* debug code */
		if (mainGameLoopCount < 2) {
			stringstream ss ;
			ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
			DebugOutput << ss.rdbuf() ;
		}
		/* end debug code */
		
		texture = SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface) ;
		
		SDL_FreeSurface(surface) ;
		
		data.texture = texture ;
	}
}





